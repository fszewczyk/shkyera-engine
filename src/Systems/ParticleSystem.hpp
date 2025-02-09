#pragma once

#include <memory>

#include <ECS/Registry.hpp>
#include <Components/ParticleEmitterComponent.hpp>

namespace shkyera {

class ParticleSystem {
public:
    ParticleSystem(std::shared_ptr<Registry> registry);
    void update();

private:
    void updateParticles(ParticleEmitterComponent& emitter, const glm::mat4& transformMatrix);

    void ensureSufficientStateContainerSize(ParticleEmitterComponent& emitter);
    void spawnParticles(ParticleEmitterComponent& emitter, const glm::mat4& transformMatrix);
    void updateLifetimes(ParticleEmitterComponent& emitter);
    void updateVelocities(ParticleEmitterComponent& emitter);
    void updatePositions(ParticleEmitterComponent& emitter);
    void updateSizes(ParticleEmitterComponent& emitter);
    void updateTransparencies(ParticleEmitterComponent& emitter);

    std::shared_ptr<Registry> _registry;
};

}
