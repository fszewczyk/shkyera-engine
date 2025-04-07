#include <Systems/ParticleSystem.hpp>

#include <Components/TransformComponent.hpp>

#include <Common/Clock.hpp>
#include <Common/Profiler.hpp>
#include <Common/Random.hpp>
#include <Common/SIMD.hpp>

namespace shkyera {

ParticleSystem::ParticleSystem(std::shared_ptr<Registry> registry) : _registry(std::move(registry)) {}

void ParticleSystem::update() {
  SHKYERA_PROFILE("ParticleSystem::update");

  std::vector<std::thread> updateThreads;
  for (const auto& [entity, constEmitter] : _registry->getComponentSet<ParticleEmitterComponent>()) {
    if (constEmitter.enabled) {
      const auto transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
      auto& emitter = _registry->getComponent<ParticleEmitterComponent>(entity);
      updateThreads.emplace_back(std::thread([&]() { updateParticles(emitter, transformMatrix); }));
    }
  }

  for (auto& t : updateThreads) {
    t.join();
  }
}

void ParticleSystem::updateParticles(ParticleEmitterComponent& emitter, const glm::mat4& transformMatrix) {
  updateLifetimes(emitter);
  updateEndToStartProgress(emitter);
  updateVelocities(emitter);
  updatePositions(emitter);
  updateSizes(emitter);
  updateTransparencies(emitter);
  ensureSufficientStateContainerSize(emitter);
  spawnParticles(emitter, transformMatrix);
}

void ParticleSystem::ensureSufficientStateContainerSize(ParticleEmitterComponent& emitter) {
  const auto maxLifetime = emitter.lifetime * (1 + emitter.lifetimeVariance);
  const auto maxEmittedPerSecond = emitter.emittedPerSecond * (1 + emitter.emittedPerSecondVariance);
  const auto maxAliveParticles = static_cast<size_t>(std::ceil(maxLifetime * maxEmittedPerSecond));
  const auto currentAvailableSlots = emitter.state.lifetimes.size();

  if (currentAvailableSlots < maxAliveParticles)  //< We need to reserve more slots
  {
    emitter.state.lifetimes.resize(maxAliveParticles, -1);
    emitter.state.initialLifetimes.resize(maxAliveParticles);
    emitter.state.endToStartProgress.resize(maxAliveParticles);
    emitter.state.sizes.resize(maxAliveParticles);
    emitter.state.initialSizes.resize(maxAliveParticles);
    emitter.state.endSizes.resize(maxAliveParticles);
    emitter.state.transparencies.resize(maxAliveParticles);
    emitter.state.initialTransparencies.resize(maxAliveParticles);
    emitter.state.endTransparencies.resize(maxAliveParticles);
    emitter.state.positions.resize(maxAliveParticles);
    emitter.state.velocities.resize(maxAliveParticles);
  } else if (maxAliveParticles * 6 < currentAvailableSlots)  //< We will save memory of unused slots
  {
    std::vector<size_t> validIndices;
    validIndices.reserve(maxAliveParticles);
    for (size_t i = 0; i < currentAvailableSlots && validIndices.size() < maxAliveParticles; ++i) {
      if (emitter.state.lifetimes[i] > 0) {
        validIndices.push_back(i);
      }
    }
    size_t nextFreeIndex = validIndices.size();

    auto shrinkVector = [maxAliveParticles, &validIndices](auto& vec, const auto& defaultValue) {
      using T = typename std::decay_t<decltype(vec)>::value_type;
      std::vector<T> newVec(maxAliveParticles, defaultValue);
      for (size_t j = 0; j < validIndices.size(); ++j) {
        newVec[j] = vec[validIndices[j]];
      }
      vec = std::move(newVec);
    };

    shrinkVector(emitter.state.lifetimes, -1.0f);
    shrinkVector(emitter.state.endToStartProgress, float{});
    shrinkVector(emitter.state.initialLifetimes, float{});
    shrinkVector(emitter.state.sizes, float{});
    shrinkVector(emitter.state.initialSizes, float{});
    shrinkVector(emitter.state.endSizes, float{});
    shrinkVector(emitter.state.transparencies, float{});
    shrinkVector(emitter.state.initialTransparencies, float{});
    shrinkVector(emitter.state.endTransparencies, float{});
    shrinkVector(emitter.state.positions, glm::vec3(0));
    shrinkVector(emitter.state.velocities, glm::vec3(0));

    emitter.state.nextFreeIndex = nextFreeIndex % maxAliveParticles;
  }
}

void ParticleSystem::spawnParticles(ParticleEmitterComponent& emitter, const glm::mat4& transformMatrix) {
  const auto getUniformSampler = [](auto center, auto varAsFraction) {
    const auto var = center * varAsFraction;
    const auto min = std::max(static_cast<decltype(center)>(0), center - var);
    const auto max = center + var;
    return random::Uniform<decltype(center)>(min, max);
  };

  auto emissionSampler = getUniformSampler(static_cast<double>(emitter.emittedPerSecond),
                                           static_cast<double>(emitter.emittedPerSecondVariance));

  const auto emitterPerSecondAtTick = emissionSampler();
  const auto emittedInTick = emitterPerSecondAtTick * clock::Game.deltaTime;

  auto& state = emitter.state;
  state.accumulatedEmittedParticles += emittedInTick;

  if (state.accumulatedEmittedParticles >= 1.0) {
    const auto particlesToSpawn = static_cast<size_t>(std::floor(state.accumulatedEmittedParticles));

    const auto availableParticleSlots = state.lifetimes.size();
    auto lifetimeSampler = getUniformSampler(emitter.lifetime, emitter.lifetimeVariance);
    auto initialTransparencySampler =
        getUniformSampler(emitter.initialTransparency, emitter.initialTransparencyVariance);
    auto endTransparencySampler = getUniformSampler(emitter.endTransparency, emitter.endTransparencyVariance);
    auto initialSizeSampler = getUniformSampler(emitter.initialParticleSize, emitter.initialParticleSizeVariance);
    auto endSizeSampler = getUniformSampler(emitter.endParticleSize, emitter.endParticleSizeVariance);

    const auto centerPosition = glm::vec3{transformMatrix[3]};
    auto positionAngleSampler = random::Uniform<float>(-M_PI, M_PI);
    auto positionDistSampler = random::Uniform<float>(0, emitter.emittedAtRadius);

    const auto defaultVelocityDirection = glm::normalize(glm::mat3{transformMatrix} * glm::vec3{0, 1, 0});
    auto velocitySampler = getUniformSampler(emitter.initialVelocity, emitter.initialVelocityVariance);
    auto velocityDisperionSampler = random::Uniform<float>(std::min(0.0f, emitter.initialVelocityDispersion),
                                                           std::max(0.0f, emitter.initialVelocityDispersion));
    for (size_t i = 0; i < particlesToSpawn; ++i) {
      // Lifetime
      state.lifetimes[state.nextFreeIndex] = lifetimeSampler();
      state.initialLifetimes[state.nextFreeIndex] = state.lifetimes[state.nextFreeIndex];

      // Transparency
      state.transparencies[state.nextFreeIndex] = initialTransparencySampler();
      state.initialTransparencies[state.nextFreeIndex] = state.transparencies[state.nextFreeIndex];
      state.endTransparencies[state.nextFreeIndex] = endTransparencySampler();

      // Sizes
      state.sizes[state.nextFreeIndex] = initialSizeSampler();
      state.initialSizes[state.nextFreeIndex] = state.sizes[state.nextFreeIndex];
      state.endSizes[state.nextFreeIndex] = endSizeSampler();

      // Position
      const auto positionAngle = positionAngleSampler();
      const auto positionRadius = positionDistSampler();
      const auto positionProjected = glm::vec3{std::cos(positionAngle), 0, std::sin(positionAngle)};
      const auto positionOffset = positionRadius * glm::normalize(glm::mat3{transformMatrix} * positionProjected);
      const glm::vec3 particlePosition = centerPosition + positionOffset;
      state.positions.at(state.nextFreeIndex) = particlePosition;

      // Velocity
      const glm::vec3 radial = glm::normalize(particlePosition - centerPosition);
      const glm::vec3 desiredDirection = (emitter.initialVelocityDispersion >= 0.f) ? radial : -radial;

      glm::vec3 rotationAxis = glm::cross(defaultVelocityDirection, desiredDirection);
      if (glm::length(rotationAxis) < 0.0001f) {
        rotationAxis = glm::vec3(0, 0, 1);
      } else {
        rotationAxis = glm::normalize(rotationAxis);
      }
      float dispersionFactor = std::abs(velocityDisperionSampler());

      const float rotationAngle = (M_PI_2)*dispersionFactor;
      glm::mat3 velocityRotation = glm::mat3(glm::rotate(rotationAngle, rotationAxis));
      glm::vec3 finalVelocityDirection = velocityRotation * defaultVelocityDirection;
      state.velocities.at(state.nextFreeIndex) = velocitySampler() * finalVelocityDirection;

      // Ring-Buffer Behavior
      ++state.nextFreeIndex;
      if (state.nextFreeIndex >= availableParticleSlots) {
        state.nextFreeIndex = 0;
      }
    }

    // Cleanup
    state.accumulatedEmittedParticles -= particlesToSpawn;
  }
}

void ParticleSystem::updateLifetimes(ParticleEmitterComponent& emitter) {
  simd::add(emitter.state.lifetimes, emitter.state.lifetimes, -clock::Game.deltaTime);
}

void ParticleSystem::updateEndToStartProgress(ParticleEmitterComponent& emitter) {
  simd::divide(emitter.state.lifetimes, emitter.state.initialLifetimes, emitter.state.endToStartProgress);
}

void ParticleSystem::updateVelocities(ParticleEmitterComponent& emitter) {
  for (auto& vel : emitter.state.velocities) {
    vel[1] += clock::Game.deltaTime * emitter.gravity;
  }
}

void ParticleSystem::updatePositions(ParticleEmitterComponent& emitter) {
  for (size_t i = 0; i < emitter.state.positions.size(); ++i) {
    emitter.state.positions[i] += clock::Game.deltaTime * emitter.state.velocities[i];
  }
}

void ParticleSystem::updateSizes(ParticleEmitterComponent& emitter) {
  simd::mix(emitter.state.endSizes, emitter.state.initialSizes, emitter.state.endToStartProgress, emitter.state.sizes);
}

void ParticleSystem::updateTransparencies(ParticleEmitterComponent& emitter) {
  simd::mix(emitter.state.endTransparencies, emitter.state.initialTransparencies, emitter.state.endToStartProgress,
            emitter.state.transparencies);
}

}  // namespace shkyera