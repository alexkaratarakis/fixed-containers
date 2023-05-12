#pragma once

#include <utility>

namespace fixed_containers
{
template <typename First, typename Second>
struct Pair
{
    First first_;
    Second second_;

    constexpr Pair() = default;

    template <typename First0, typename Second0>
    constexpr Pair(First0&& first, Second0&& second)
      : first_(std::forward<First0>(first))
      , second_(std::forward<Second0>(second))
    {
    }

    constexpr Pair(const std::pair<First, Second>& p)
      : first_(p.first)
      , second_(p.second)
    {
    }
    constexpr Pair(std::pair<First, Second>&& p)
      : first_(std::move(p.first))
      , second_(std::move(p.second))
    {
    }

    constexpr const First& first() const { return first_; }
    constexpr First& first() { return first_; }

    constexpr const Second& second() const { return second_; }
    constexpr Second& second() { return second_; }

    constexpr operator std::pair<First, Second>() const { return {first(), second()}; }

    template <class U1, class U2>
    constexpr operator std::pair<U1, U2>() const
    {
        return {first(), second()};
    }
    template <class U1, class U2>
    constexpr operator std::pair<U1, U2>()
    {
        return {first(), second()};
    }

    [[nodiscard]] constexpr bool operator==(const Pair<First, Second>& other) const
    {
        return std::tie(first(), second()) == std::tie(other.first(), other.second());
    }
};
}  // namespace fixed_containers
