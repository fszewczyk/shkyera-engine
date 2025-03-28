#pragma once

#include <atomic>

#include <Common/Profiler.hpp>

namespace shkyera {

template<typename ObjectType>
struct InstanceCounter
{
public:
    static inline std::atomic<size_t> Instantiated = 0;
    static inline std::atomic<size_t> Alive = 0;
    static inline std::atomic<size_t> Moved = 0;
    static inline std::atomic<size_t> Copied = 0;
    InstanceCounter()
    {
        ++Instantiated;
        ++Alive;
    }

    InstanceCounter(const InstanceCounter&)
    {
        ++Instantiated;
        ++Alive;
    }

    InstanceCounter(InstanceCounter&&) noexcept
    {
        ++Moved;
        ++Alive;
    }

    InstanceCounter& operator=(const InstanceCounter&)
    {
        ++Copied;
        return *this;
    }

    InstanceCounter& operator=(InstanceCounter&&) noexcept
    {
        ++Moved;
        return *this;
    }

protected:
    ~InstanceCounter()
    {
        --Alive;
    }
};

}

