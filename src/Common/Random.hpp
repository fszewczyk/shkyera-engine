#pragma once

#include <glm/glm.hpp>
#include <random>
#include <vector>
#include <limits>

namespace shkyera::random {

template<typename SampledType>
class Uniform {
public:
    Uniform(SampledType min, SampledType max) : _dist(min, max), _rng(std::random_device{}()) {}

    SampledType operator()() {
        return _dist(_rng);
    }

private:
    std::mt19937_64 _rng;
    std::uniform_real_distribution<SampledType> _dist;
};

template<typename SampledType>
class Normal {
public:
    Normal(SampledType mean, SampledType stddev) : _dist(mean, stddev), _rng(std::random_device{}()) {}

    double operator()() {
        return _dist(_rng);
    }

private:
    std::mt19937_64 _rng;
    std::normal_distribution<SampledType> _dist;
};

class Sphere {
public:
    Sphere() : _uniformSampler(-1.0f, 1.0f), _rng(std::random_device{}()) {}

    glm::vec3 operator()() {
        glm::vec3 point;

        point.x = _uniformSampler();
        point.y = _uniformSampler();
        point.z = _uniformSampler();

        return glm::normalize(point);
    }

private:
    Uniform<float> _uniformSampler;
    std::mt19937_64 _rng;
};

}