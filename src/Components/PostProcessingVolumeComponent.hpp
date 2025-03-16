#pragma once

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
};

}  // namespace shkyera
