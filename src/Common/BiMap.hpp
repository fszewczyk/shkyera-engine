#pragma once

#include <unordered_map>

namespace shkyera::common {

template <typename First, typename Second, template <typename, typename> class Impl = std::unordered_map>
class BiMap {
   public:
    using ForwardMap = Impl<First, Second>;
    using BackwardMap = Impl<Second, First>;

    bool empty() const {
        return forward.empty();
    }

    void clear() {
        forward.clear();
        backward.clear();
    }

    const Second& at(const First& first) const {
        return forward.at(first);
    }

    const First& at(const Second& second) const {
        return backward.at(second);
    }

    void insert(First first, Second second) {
        forward[first] = second;
        backward[second] = first;
    }

    void remove(First first) {
        const auto& second = at(first);
        backward.erase(second);
        forward.erase(first);
    }

    void remove(Second second) {
        const auto& first = at(second);
        forward.erase(first);
        backward.erase(second);
    }

   private:
    ForwardMap forward;
    BackwardMap backward;
};

}  // namespace shkyera::common
