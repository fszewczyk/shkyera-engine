#pragma once

#include <chrono>
#include <thread>
#include <mutex>
#include <map>
#include <string>

namespace shkyera {

struct ProfileBlock {
    double totalLengthInNanoSeconds = 0;
    size_t numberOfCalls = 0;

    ProfileBlock& operator+=(const ProfileBlock& other);
};

class ProfileGuard {
    public:
        ProfileGuard(std::string&& name);
        ~ProfileGuard();

    private:
        std::string mProfileName;
        std::chrono::high_resolution_clock::time_point mStartTime;
};

class Profiler {
    public:
        using BlocksPerThread = std::map<std::thread::id, std::map<std::string, ProfileBlock>>;
        static Profiler& getInstance();

        void clear();
        void addBlock(std::string&& name, std::chrono::nanoseconds time);
        BlocksPerThread getProfiles();

    private:
        Profiler() = default;

        std::mutex mMutex;
        BlocksPerThread mProfileBlocks;
};

#define SHKYERA_PROFILE(name) ProfileGuard __SHKYERA_UNIQUE_NAME(__LINE__) (name)
#define SHKYERA_READ_PROFILE Profiler::getInstance().getProfiles()
#define SHKYERA_CLEAR_PROFILE Profiler::getInstance().clear()

#define __SHKYERA_UNIQUE_NAME(LINE) __SHKYERA_CONCAT(profileGuard_, LINE)
#define __SHKYERA_CONCAT(X, Y) __SHKYERA_CONCAT_IMPL(X, Y)
#define __SHKYERA_CONCAT_IMPL(X, Y) X##Y

}
