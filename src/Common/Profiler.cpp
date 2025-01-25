#include <Common/Profiler.hpp>

namespace shkyera {

ProfileBlock& ProfileBlock::operator+=(const ProfileBlock& other)
{
    totalLengthInNanoSeconds += other.totalLengthInNanoSeconds;
    numberOfCalls += other.numberOfCalls;
    return *this;
}

ProfileGuard::ProfileGuard(std::string&& name) : mProfileName(std::move(name)), mStartTime(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch())) {}

ProfileGuard::~ProfileGuard()
{
    const auto endTime = std::chrono::high_resolution_clock::now();
    const auto guardDuration = endTime - mStartTime;
    const auto guardDurationInNanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(guardDuration);
    Profiler::getInstance().addBlock(std::move(mProfileName), guardDurationInNanoSeconds);
}

Profiler& Profiler::getInstance()
{
    static Profiler p;
    return p;
}

void Profiler::clear()
{
    std::unique_lock lock(mMutex);
    mProfileBlocks.clear();
}

void Profiler::addBlock(std::string&& name, std::chrono::nanoseconds time)
{
    const auto threadId = std::this_thread::get_id();
    ProfileBlock newBlock { .totalLengthInNanoSeconds = static_cast<double>(time.count()), .numberOfCalls = 1 };

    std::unique_lock lock(mMutex);
    mProfileBlocks[threadId][name] += newBlock;
}

Profiler::BlocksPerThread Profiler::getProfiles()
{
    std::unique_lock lock(mMutex);
    return mProfileBlocks;
}


}
