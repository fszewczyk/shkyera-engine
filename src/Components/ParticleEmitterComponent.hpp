#pragma once

#include <glm/glm.hpp>

#include <AssetManager/Asset.hpp>
#include <AssetManager/Material.hpp>
#include <Common/Serialization.hpp>
#include <cereal/cereal.hpp>

namespace shkyera {

struct ParticleEmitterComponent {
    bool enabled{true};

    float gravity{0.1f};

    float emittedAtRadius{1.0f};
    float emittedPerSecond{30.0f};
    float emittedPerSecondVariance{0.1f};

    float initialVelocity{1.0f};
    float initialVelocityVariance{0.1f};
    float initialVelocityDispersion{-0.5f};

    float lifetime{1.0f};
    float lifetimeVariance{0.1f};

    float initialParticleSize{0.1f};
    float initialParticleSizeVariance{0.1f};
    float endParticleSize{0.0f};
    float endParticleSizeVariance{0.1f};

    HandleAndAsset<Material> material{};
    float initialTransparency{1.0f};
    float initialTransparencyVariance{0.1f};
    float endTransparency{0.1f};
    float endTransparencyVariance{0.1f};

    struct state {
        double accumulatedEmittedParticles{0.0};  //< Used for spawning at low emission rates
        size_t nextFreeIndex{0};                  //< Used for ring-buffer behavior

        std::vector<float> lifetimes{};
        std::vector<float> endToStartProgress{};
        std::vector<float> initialLifetimes{};

        std::vector<float> sizes{};
        std::vector<float> initialSizes{};
        std::vector<float> endSizes{};

        std::vector<float> transparencies{};
        std::vector<float> initialTransparencies{};
        std::vector<float> endTransparencies{};

        std::vector<glm::vec3> positions{};   //< World CS
        std::vector<glm::vec3> velocities{};  //< World CS
    } state;

    template <typename Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(enabled));
        archive(CEREAL_NVP(gravity));
        archive(CEREAL_NVP(emittedAtRadius));
        archive(CEREAL_NVP(emittedPerSecond));
        archive(CEREAL_NVP(emittedPerSecondVariance));
        archive(CEREAL_NVP(initialVelocity));
        archive(CEREAL_NVP(initialVelocityVariance));
        archive(CEREAL_NVP(initialVelocityDispersion));
        archive(CEREAL_NVP(lifetime));
        archive(CEREAL_NVP(lifetimeVariance));
        archive(CEREAL_NVP(initialParticleSize));
        archive(CEREAL_NVP(initialParticleSizeVariance));
        archive(CEREAL_NVP(endParticleSize));
        archive(CEREAL_NVP(endParticleSizeVariance));
        archive(CEREAL_NVP(material));
        archive(CEREAL_NVP(initialTransparency));
        archive(CEREAL_NVP(initialTransparencyVariance));
        archive(CEREAL_NVP(endTransparency));
        archive(CEREAL_NVP(endTransparencyVariance));
    }
};

}  // namespace shkyera
