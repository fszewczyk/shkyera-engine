#pragma once

#include "game/Component.hpp"

namespace shkyera {

typedef struct PublicFloat {
    std::string name;
    float value;
} PublicFloat;

typedef struct PublicInt {
    std::string name;
    int value;
} PublicInt;

typedef struct PublicString {
    std::string name;
    std::string value;
} PublicString;

typedef struct PublicVec3 {
    std::string name;
    glm::vec3 value;
} PublicVec3;

class ScriptComponent : public Component {};

} // namespace shkyera
