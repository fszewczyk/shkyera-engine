#pragma once

#include <Common/Serialization.hpp>
#include "cereal/cereal.hpp"

namespace shkyera {

struct PostProcessingVolumeComponent {
    bool global{false};

    float gamma{1.2};
    bool toneMapping{true};
    bool antiAliasing{true};

    float bloomThreshold{3.0};
    float bloomWeight{0.2};

    float ssaoStrength{1.0};
    float ssaoRadius{1.0};

    template <typename Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(global));
        archive(CEREAL_NVP(gamma));
        archive(CEREAL_NVP(toneMapping));
        archive(CEREAL_NVP(antiAliasing));
        archive(CEREAL_NVP(bloomThreshold));
        archive(CEREAL_NVP(bloomWeight));
        archive(CEREAL_NVP(ssaoStrength));
        archive(CEREAL_NVP(ssaoRadius));
    }
};

}  // namespace shkyera
