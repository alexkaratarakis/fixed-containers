#pragma once

#include "fixed_containers/assert_or_abort.hpp"
#include "fixed_containers/concepts.hpp"

#include <cassert>
#include <compare>
#include <concepts>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>

namespace fixed_containers::mock_testing_types_detail
{
constexpr void noop_constexpr_function_to_induce_non_triviality() {}
}  // namespace fixed_containers::mock_testing_types_detail

namespace fixed_containers
{
struct MockNonDefaultConstructible
{
    constexpr MockNonDefaultConstructible() = delete;
    constexpr MockNonDefaultConstructible(int /*unused*/) {}
    constexpr MockNonDefaultConstructible(const MockNonDefaultConstructible& other) noexcept =
        default;
    constexpr MockNonDefaultConstructible(MockNonDefaultConstructible&& other) noexcept = default;
    constexpr MockNonDefaultConstructible& operator=(
        const MockNonDefaultConstructible& other) noexcept = default;
    constexpr MockNonDefaultConstructible& operator=(
        MockNonDefaultConstructible&& /*other*/) noexcept = default;
};
static_assert(NotDefaultConstructible<MockNonDefaultConstructible>);

struct MockNonTrivialDestructible
{
    constexpr MockNonTrivialDestructible() = default;

    MockNonTrivialDestructible(const MockNonTrivialDestructible& other) noexcept = default;
    MockNonTrivialDestructible(MockNonTrivialDestructible&& other) noexcept = default;

    MockNonTrivialDestructible& operator=(const MockNonTrivialDestructible& other) noexcept =
        default;
    MockNonTrivialDestructible& operator=(MockNonTrivialDestructible&& other) noexcept = default;

    constexpr ~MockNonTrivialDestructible()
    {
        mock_testing_types_detail::noop_constexpr_function_to_induce_non_triviality();
    }
};

static_assert(NotTriviallyDestructible<MockNonTrivialDestructible>);
static_assert(NotTriviallyCopyable<MockNonTrivialDestructible>);

struct MockNonTrivialCopyAssignable
{
    constexpr MockNonTrivialCopyAssignable() = default;

    constexpr MockNonTrivialCopyAssignable(const MockNonTrivialCopyAssignable& other) noexcept =
        default;
    constexpr MockNonTrivialCopyAssignable(MockNonTrivialCopyAssignable&& other) noexcept = default;

    MockNonTrivialCopyAssignable& operator=(MockNonTrivialCopyAssignable&& other) noexcept =
        default;

    constexpr MockNonTrivialCopyAssignable& operator=(
        const MockNonTrivialCopyAssignable& /*other*/) noexcept
    {
        mock_testing_types_detail::noop_constexpr_function_to_induce_non_triviality();
        return *this;
    }
};

struct MockNonTrivialCopyConstructible
{
    constexpr MockNonTrivialCopyConstructible() = default;

    constexpr MockNonTrivialCopyConstructible(
        const MockNonTrivialCopyConstructible& /*other*/) noexcept
    {
        mock_testing_types_detail::noop_constexpr_function_to_induce_non_triviality();
    }
    constexpr MockNonTrivialCopyConstructible(MockNonTrivialCopyConstructible&& other) noexcept =
        default;

    constexpr MockNonTrivialCopyConstructible& operator=(
        MockNonTrivialCopyConstructible&& other) noexcept = default;

    constexpr MockNonTrivialCopyConstructible& operator=(
        const MockNonTrivialCopyConstructible& other) noexcept = default;
};

struct MockNonAssignable
{
    const int t = 5;
};

static_assert(MoveConstructible<MockNonAssignable>);
static_assert(TriviallyCopyable<MockNonAssignable>);

struct MockNonCopyAssignable
{
    constexpr MockNonCopyAssignable() = default;

    constexpr MockNonCopyAssignable(const MockNonCopyAssignable& other) noexcept = default;
    constexpr MockNonCopyAssignable(MockNonCopyAssignable&& other) noexcept = default;

    constexpr MockNonCopyAssignable& operator=(const MockNonCopyAssignable& other) noexcept =
        delete;
    constexpr MockNonCopyAssignable& operator=(MockNonCopyAssignable&& other) noexcept = default;
};

static_assert(DefaultConstructible<MockNonCopyAssignable>);
static_assert(NotCopyAssignable<MockNonCopyAssignable>);
static_assert(MoveAssignable<MockNonCopyAssignable>);
static_assert(TriviallyCopyable<MockNonCopyAssignable>);

struct MockNonTriviallyCopyAssignable
{
    constexpr MockNonTriviallyCopyAssignable() = default;

    MockNonTriviallyCopyAssignable(const MockNonTriviallyCopyAssignable& other) noexcept = default;
    MockNonTriviallyCopyAssignable(MockNonTriviallyCopyAssignable&& other) noexcept = default;

    MockNonTriviallyCopyAssignable& operator=(
        const MockNonTriviallyCopyAssignable& /*other*/) noexcept
    {
        mock_testing_types_detail::noop_constexpr_function_to_induce_non_triviality();
        return *this;
    }
    MockNonTriviallyCopyAssignable& operator=(MockNonTriviallyCopyAssignable&& /*other*/) noexcept
    {
        return *this;
    }
};

static_assert(DefaultConstructible<MockNonTriviallyCopyAssignable>);
static_assert(CopyAssignable<MockNonTriviallyCopyAssignable>);
static_assert(NotTriviallyCopyable<MockNonTriviallyCopyAssignable>);

struct MockMoveableButNotCopyable
{
    constexpr MockMoveableButNotCopyable() = default;

    constexpr MockMoveableButNotCopyable(const MockMoveableButNotCopyable& other) noexcept = delete;
    constexpr MockMoveableButNotCopyable(MockMoveableButNotCopyable&& other) noexcept = default;

    constexpr MockMoveableButNotCopyable& operator=(
        const MockMoveableButNotCopyable& other) noexcept = delete;
    constexpr MockMoveableButNotCopyable& operator=(
        MockMoveableButNotCopyable&& /*other*/) noexcept = default;
};
// This still counts as trivially-copyable.
static_assert(TriviallyCopyable<MockMoveableButNotCopyable>);
static_assert(NotCopyAssignable<MockMoveableButNotCopyable>);
static_assert(NotCopyConstructible<MockMoveableButNotCopyable>);

// std::atomic<int> and std::mutex are examples of this
struct MockTriviallyCopyableButNotCopyableOrMoveable
{
    constexpr MockTriviallyCopyableButNotCopyableOrMoveable() = default;

    constexpr MockTriviallyCopyableButNotCopyableOrMoveable(
        const MockTriviallyCopyableButNotCopyableOrMoveable& other) noexcept = delete;
    constexpr MockTriviallyCopyableButNotCopyableOrMoveable(
        MockTriviallyCopyableButNotCopyableOrMoveable&& other) noexcept = delete;

    constexpr MockTriviallyCopyableButNotCopyableOrMoveable& operator=(
        const MockTriviallyCopyableButNotCopyableOrMoveable& other) noexcept = delete;
    constexpr MockTriviallyCopyableButNotCopyableOrMoveable& operator=(
        MockTriviallyCopyableButNotCopyableOrMoveable&& other) noexcept = delete;
};

#if defined(__clang__) || defined(__GNUC__)
static_assert(TriviallyCopyable<MockTriviallyCopyableButNotCopyableOrMoveable>);
#elif defined(_MSC_VER)
static_assert(NotTriviallyCopyable<MockTriviallyCopyableButNotCopyableOrMoveable>);
#endif
static_assert(NotCopyConstructible<MockTriviallyCopyableButNotCopyableOrMoveable>);
static_assert(NotMoveConstructible<MockTriviallyCopyableButNotCopyableOrMoveable>);
static_assert(NotCopyAssignable<MockTriviallyCopyableButNotCopyableOrMoveable>);
static_assert(NotMoveAssignable<MockTriviallyCopyableButNotCopyableOrMoveable>);
static_assert(TriviallyDestructible<MockTriviallyCopyableButNotCopyableOrMoveable>);

// Int, but no trivial constructors/destructor/assignment.
struct MockNonTrivialInt
{
    int value = 0;

    constexpr MockNonTrivialInt()
      : value{0}
    {
    }

    explicit(false) MockNonTrivialInt(int val)
      : value{val}
    {
    }

    constexpr MockNonTrivialInt(const MockNonTrivialInt& other) noexcept { value = other.value; }
    constexpr MockNonTrivialInt(MockNonTrivialInt&& other) noexcept { value = other.value; }

    constexpr MockNonTrivialInt& operator=(const MockNonTrivialInt& other) noexcept
    {
        mock_testing_types_detail::noop_constexpr_function_to_induce_non_triviality();
        value = other.value;
        return *this;
    }
    constexpr MockNonTrivialInt& operator=(MockNonTrivialInt&& other) noexcept
    {
        value = other.value;
        return *this;
    }

    constexpr ~MockNonTrivialInt()
    {
        mock_testing_types_detail::noop_constexpr_function_to_induce_non_triviality();
    }

    constexpr bool operator==(const MockNonTrivialInt& other) const { return value == other.value; }
};

struct MockBComparableToA
{
    int value;

    constexpr std::strong_ordering operator<=>(const MockBComparableToA& other) const
    {
        return value <=> other.value;
    }
};

struct MockAComparableToB
{
    int value;

    constexpr bool operator==(const MockAComparableToB& other) const
    {
        return value == other.value;
    }
    constexpr std::strong_ordering operator<=>(const MockAComparableToB& other) const
    {
        return value <=> other.value;
    }

    constexpr bool operator==(const MockBComparableToA& other) const
    {
        return value == other.value;
    }
    constexpr std::strong_ordering operator<=>(const MockBComparableToA& other) const
    {
        return value <=> other.value;
    }
};

template <std::integral T>
class MockIntegralStream
{
    class MockInputIterator
    {
    public:
        using reference = const T&;
        using value_type = const T;
        using pointer = const T*;
        using iterator = MockInputIterator;
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;

    private:
        std::optional<T*> remaining_;

    public:
        constexpr MockInputIterator() noexcept
          : remaining_{}
        {
        }

        explicit constexpr MockInputIterator(T& remaining) noexcept
          : remaining_(std::addressof(remaining))
        {
        }

        constexpr reference operator*() const noexcept { return *remaining_.value(); }

        constexpr pointer operator->() const noexcept { return remaining_.value(); }

        constexpr MockInputIterator& operator++() noexcept
        {
            assert_or_abort(remaining_.has_value());
            auto& rem = *remaining_.value();
            assert_or_abort(rem > 0);
            rem--;
            if (rem == 0)
            {
                remaining_.reset();
            }
            return *this;
        }

        constexpr MockInputIterator operator++(int) & noexcept
        {
            MockInputIterator copy{*this};
            operator++();
            return copy;
        }

        constexpr bool operator==(const MockInputIterator& other) const noexcept = default;
    };

private:
    T remaining_;

public:
    explicit constexpr MockIntegralStream(const T stream_size)
      : remaining_{stream_size}
    {
    }

public:
    constexpr MockInputIterator begin() { return MockInputIterator{remaining_}; }

    constexpr MockInputIterator end() { return MockInputIterator{}; }
};

struct ImplicitlyConvertibleFromInt
{
    int value;

    explicit(false) constexpr ImplicitlyConvertibleFromInt(int value_in_ctor)
      : value{value_in_ctor}
    {
    }
};

struct ExplicitlyConvertibleFromInt
{
    int value;

    explicit constexpr ExplicitlyConvertibleFromInt(int value_in_ctor)
      : value{value_in_ctor}
    {
    }
};

struct TypeWithMultipleConstructorParameters
{
    ImplicitlyConvertibleFromInt implicit_int;
    ExplicitlyConvertibleFromInt explicit_int;

    constexpr TypeWithMultipleConstructorParameters(ImplicitlyConvertibleFromInt param_a0,
                                                    ExplicitlyConvertibleFromInt param_b0)
      : implicit_int{param_a0}
      , explicit_int{param_b0}
    {
    }
};

struct MockEmptyStruct
{
};

struct MockNonAggregate
{
    int field_1;

    constexpr MockNonAggregate() noexcept = default;
};
static_assert(NotAggregate<MockNonAggregate>);

class MockFailingAddressOfOperator
{
    using Self = MockFailingAddressOfOperator;

public:
    int value_;

public:
    constexpr MockFailingAddressOfOperator()
      : value_{}
    {
    }
    constexpr MockFailingAddressOfOperator(int value)
      : value_{value}
    {
    }

    // Makes some errors related to operator& compile-time whereas using something like int* or
    // void* might mask it (the former because many tests use int as a type)
    struct InaccessibleType
    {
    };

public:
    [[noreturn]] InaccessibleType* operator&() const noexcept  // NOLINT(google-runtime-operator)
    {
        std::abort();
    }

    void do_nothing() const {}

    [[nodiscard]] constexpr int get() const { return value_; }

    constexpr bool operator==(const Self& other) const { return value_ == other.value_; }
    constexpr std::strong_ordering operator<=>(const Self& other) const
    {
        return value_ <=> other.value_;
    }
};

struct alignas(64) MockAligned64
{
public:
    int value_;

public:
    constexpr MockAligned64()
      : value_{}
    {
    }
    constexpr MockAligned64(int value)
      : value_{value}
    {
    }

    [[nodiscard]] constexpr int get() const { return value_; }

    constexpr bool operator==(const MockAligned64& other) const = default;
};

static_assert(alignof(MockAligned64) == 64);
static_assert(sizeof(MockAligned64) == 64);

struct MockTypeWithConstAndNonConstFunctions
{
    constexpr void const_function() const {}

    constexpr void non_const_function() {}
};

}  // namespace fixed_containers

template <>
struct std::hash<fixed_containers::MockFailingAddressOfOperator>
{
    constexpr std::uint64_t operator()(
        const fixed_containers::MockFailingAddressOfOperator& val) const noexcept
    {
        return static_cast<std::uint64_t>(val.get());
    }
};

template <>
struct std::hash<fixed_containers::MockAligned64>
{
    std::size_t operator()(const fixed_containers::MockAligned64& val) const
    {
        return static_cast<std::uint64_t>(val.get());
    }
};
