#pragma once

#include <vector>

#include <ECS/RegistryViewer.hpp>
#include <JobSystem/JobSystem.hpp>

namespace shkyera::utils::jobs {

namespace {

void setJobAttributes(const Policy& policy, JobBuilder& builder, const std::vector<JobHandle>& dependsOn) {
  for (const auto& dep : dependsOn) {
    builder.dependsOn(dep);
  }

  for (const auto& readAccess : policy.read) {
    builder.readResource(readAccess);
  }

  for (const auto& readAccess : policy.write) {
    builder.writeResource(readAccess);
  }

  if (policy.onlyMainThread || policy.onlyMainThread) {
    builder.useMainThread();
  }
}

}  // namespace

template <typename System, typename Result>
JobHandle scheduleSystem(System& system, Result& result, const std::vector<JobHandle>& dependsOn = {}) {
  auto builder = JobBuilder([&] { result = system.update(); });
  setJobAttributes(system.getPolicy(), builder, dependsOn);
  return builder.submit();
}

template <typename System>
JobHandle scheduleSystem(System& system, const std::vector<JobHandle>& dependsOn = {}) {
  auto builder = JobBuilder([&] { system.update(); });
  setJobAttributes(system.getPolicy(), builder, dependsOn);
  return builder.submit();
}

}  // namespace shkyera::utils::jobs
