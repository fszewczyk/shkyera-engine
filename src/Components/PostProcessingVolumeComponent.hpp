#pragma once

namespace shkyera {

struct PostProcessingVolumeComponent
{
    float gamma{1.2};
    bool toneMapping{true};
    bool antiAliasing{true};
    bool bloom{true};
    bool global{false};
};

}
