#pragma once

#include <cereal/cereal.hpp>
#include <glm/glm.hpp>

namespace glm {

template <typename Archive>
void serialize(Archive& archive, glm::vec3& v) {
    archive(CEREAL_NVP(v.x));
    archive(CEREAL_NVP(v.y));
    archive(CEREAL_NVP(v.z));
}

}  // namespace glm

namespace cereal {
template <class Archive, typename T>
void save(Archive& archive, const std::optional<T>& opt) {
    bool has_value = opt.has_value();
    archive(CEREAL_NVP(has_value));
    if (has_value) {
        archive(CEREAL_NVP(*opt));
    }
}

template <class Archive, typename T>
void load(Archive& archive, std::optional<T>& opt) {
    bool has_value;
    archive(CEREAL_NVP(has_value));
    if (has_value) {
        T value;
        archive(CEREAL_NVP(value));
        opt = value;
    } else {
        opt.reset();
    }
}
}  // namespace cereal
