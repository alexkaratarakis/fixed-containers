#pragma once

#include "fixed_containers/enum_map.hpp"
#include "fixed_containers/enum_utils.hpp"

#if __has_include(<magic_enum/magic_enum.hpp>)
#include <magic_enum/magic_enum.hpp>
#else
#include <magic_enum.hpp>
#endif

#include <array>
#include <cstddef>
#include <string_view>

namespace example
{
// DEFINITION
namespace detail
{
enum class ColorBackingEnum
{
    RED,
    YELLOW,
    BLUE,
    GREEN,
};
}  // namespace detail

// SkeletalRichEnum base class automatically provides all general-purpose functionality.
class Color : public fixed_containers::rich_enums::SkeletalRichEnum<Color, detail::ColorBackingEnum>
{
    friend SkeletalRichEnum::ValuesFriend;
    using SkeletalRichEnum::SkeletalRichEnum;

public:
    // Forwarders for the enum constants. Macro not strictly required, see subsequent examples.
    FIXED_CONTAINERS_RICH_ENUM_CONSTANT_GEN_HELPER(Color, RED)
    FIXED_CONTAINERS_RICH_ENUM_CONSTANT_GEN_HELPER(Color, YELLOW)
    FIXED_CONTAINERS_RICH_ENUM_CONSTANT_GEN_HELPER(Color, BLUE)
    FIXED_CONTAINERS_RICH_ENUM_CONSTANT_GEN_HELPER(Color, GREEN)

    // Custom member function
    [[nodiscard]] constexpr bool is_primary() const
    {
        return backing_enum() == detail::ColorBackingEnum::RED ||
               backing_enum() == detail::ColorBackingEnum::YELLOW ||
               backing_enum() == detail::ColorBackingEnum::BLUE;
    }
};

// USAGE
static_assert(fixed_containers::rich_enums::is_rich_enum<Color>);  // Type-trait `concept`
inline constexpr Color COLOR = Color::RED();                       // Note the parens
static_assert("RED" == COLOR.to_string());                         // auto-provided member
static_assert(COLOR.is_primary());                                 // Custom member
static_assert(COLOR == Color::value_of("RED").value());            // auto-provided
static_assert(4 == Color::count());                                // auto-provided
}  // namespace example

namespace fixed_containers::rich_enums
{
enum class TestEnum1
{
    ONE = 0,
    TWO,
    THREE,
    FOUR,
};

enum class EnumWithNoConstants
{

};

namespace detail
{
enum class TestRichEnum1BackingEnum : std::uint32_t
{
    C_ONE = 19,
    C_FOUR = 25,
    C_TWO = 21,
    C_THREE = 23,
};

// If we have data to associate with each enum constant, we can put them in a map here, then provide
// them with rich enum member functions.
struct TestRichEnum1Data
{
    std::size_t value;
    double double_value;
};

inline constexpr auto TEST_RICH_ENUM_1_DATA =
    EnumMap<TestRichEnum1BackingEnum, TestRichEnum1Data>::create_with_all_entries({
        {TestRichEnum1BackingEnum::C_ONE, {.value = 1, .double_value = 1.0}},
        {TestRichEnum1BackingEnum::C_TWO, {.value = 2, .double_value = 2.0}},
        {TestRichEnum1BackingEnum::C_THREE, {.value = 3, .double_value = 3.0}},
        {TestRichEnum1BackingEnum::C_FOUR, {.value = 4, .double_value = 4.0}},
    });

}  // namespace detail

class TestRichEnum1 : public SkeletalRichEnum<TestRichEnum1, detail::TestRichEnum1BackingEnum>
{
    friend SkeletalRichEnum::ValuesFriend;
    using SkeletalRichEnum::SkeletalRichEnum;

public:
    FIXED_CONTAINERS_RICH_ENUM_CONSTANT_GEN_HELPER(TestRichEnum1, C_ONE)
    FIXED_CONTAINERS_RICH_ENUM_CONSTANT_GEN_HELPER(TestRichEnum1, C_TWO)
    FIXED_CONTAINERS_RICH_ENUM_CONSTANT_GEN_HELPER(TestRichEnum1, C_THREE)
    FIXED_CONTAINERS_RICH_ENUM_CONSTANT_GEN_HELPER(TestRichEnum1, C_FOUR)

public:
    // Provide associated data with member functions.
    [[nodiscard]] constexpr std::size_t value() const
    {
        return detail::TEST_RICH_ENUM_1_DATA.at(backing_enum()).value;
    }
    [[nodiscard]] constexpr double double_value() const
    {
        return detail::TEST_RICH_ENUM_1_DATA.at(backing_enum()).double_value;
    }
};

// Example with infused data
namespace detail
{
enum class TestRichEnum2BackingEnum
{
    C_ONE,
    C_TWO,
    C_THREE,
    C_FOUR,
};

struct TestRichEnum2Data
{
    std::size_t value;
};

struct TestRichEnum2InfusedData
{
    using BE = TestRichEnum2BackingEnum;
    static constexpr auto VALUES = EnumMap<BE, TestRichEnum2Data>::create_with_all_entries({
        {BE::C_ONE, {1}},
        {BE::C_TWO, {2}},
        {BE::C_THREE, {3}},
        {BE::C_FOUR, {4}},
    });
};

}  // namespace detail

class TestRichEnum2
  : public NonDefaultConstructibleSkeletalRichEnum<TestRichEnum2, detail::TestRichEnum2BackingEnum>
{
    friend SkeletalRichEnum::ValuesFriend;
    using NonDefaultConstructibleSkeletalRichEnum::NonDefaultConstructibleSkeletalRichEnum;

public:
    FIXED_CONTAINERS_RICH_ENUM_CONSTANT_GEN_HELPER(TestRichEnum2, C_ONE)
    FIXED_CONTAINERS_RICH_ENUM_CONSTANT_GEN_HELPER(TestRichEnum2, C_TWO)
    FIXED_CONTAINERS_RICH_ENUM_CONSTANT_GEN_HELPER(TestRichEnum2, C_THREE)
    FIXED_CONTAINERS_RICH_ENUM_CONSTANT_GEN_HELPER(TestRichEnum2, C_FOUR)

public:
    [[nodiscard]] constexpr std::size_t value() const
    {
        return detail::TestRichEnum2InfusedData::VALUES.at(backing_enum()).value;
    }
};

// This class does not have ordinal() and count()
// Instead it has differently named functions
class NonConformingTestRichEnum1
{
    friend class NonConformingTestRichEnum1Values;

private:
    enum class BackingEnum
    {
        NC_ONE,
        NC_TWO,
    };

public:
    static constexpr std::size_t size() { return magic_enum::enum_count<BackingEnum>(); }

    static constexpr const std::array<NonConformingTestRichEnum1,
                                      magic_enum::enum_count<BackingEnum>()>&
    all_values();

private:
    static constexpr NonConformingTestRichEnum1 at(const BackingEnum& backing_enum)
    {
        return all_values().at(magic_enum::enum_index(backing_enum).value());
    }

public:
    static constexpr NonConformingTestRichEnum1 NC_ONE()  // NOLINT(readability-identifier-naming)
    {
        return at(BackingEnum::NC_ONE);
    }
    static constexpr NonConformingTestRichEnum1 NC_TWO()  // NOLINT(readability-identifier-naming)
    {
        return at(BackingEnum::NC_TWO);
    }

private:
    BackingEnum backing_enum_;

    constexpr NonConformingTestRichEnum1(BackingEnum backing_enum)
      : backing_enum_(backing_enum)
    {
    }

public:
    [[nodiscard]] constexpr std::size_t index() const
    {
        return static_cast<std::size_t>(magic_enum::enum_integer(backing_enum_));
    }
    [[nodiscard]] constexpr std::string_view str() const
    {
        return magic_enum::enum_name(backing_enum_);
    }
};

class NonConformingTestRichEnum1Values
{
    friend class NonConformingTestRichEnum1;
    using BackingEnum = NonConformingTestRichEnum1::BackingEnum;
    static constexpr std::array<NonConformingTestRichEnum1, NonConformingTestRichEnum1::size()>
        VALUES{
            NonConformingTestRichEnum1{BackingEnum::NC_ONE},
            {BackingEnum::NC_TWO},
        };
};

constexpr const std::array<NonConformingTestRichEnum1, NonConformingTestRichEnum1::size()>&
NonConformingTestRichEnum1::all_values()
{
    return NonConformingTestRichEnum1Values::VALUES;
}

template <>
struct EnumAdapter<NonConformingTestRichEnum1>
{
    using Enum = NonConformingTestRichEnum1;
    static constexpr std::size_t count() { return NonConformingTestRichEnum1::size(); }

    static constexpr const std::array<NonConformingTestRichEnum1, 2>& values()
    {
        return NonConformingTestRichEnum1::all_values();
    }
    static constexpr std::size_t ordinal(const NonConformingTestRichEnum1& key)
    {
        return key.index();
    }
    static constexpr std::string_view to_string(const NonConformingTestRichEnum1& key)
    {
        return key.str();
    }
};

namespace detail
{
enum class TestRichEnumBoolBackingEnum : bool
{
    FALSE_VALUE = false,
    TRUE_VALUE = true,
};

}  // namespace detail

class TestRichEnumBool
  : public SkeletalRichEnum<TestRichEnumBool, detail::TestRichEnumBoolBackingEnum>
{
    friend SkeletalRichEnum::ValuesFriend;
    using SkeletalRichEnum::SkeletalRichEnum;

public:
    FIXED_CONTAINERS_RICH_ENUM_CONSTANT_GEN_HELPER(TestRichEnumBool, FALSE_VALUE)
    FIXED_CONTAINERS_RICH_ENUM_CONSTANT_GEN_HELPER(TestRichEnumBool, TRUE_VALUE)
};

// clang-format off
enum class TestEnum5 { ONE, TWO, THREE, FOUR, FIVE };

enum class TestEnum32 {
    V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15, V16,
    V17, V18, V19, V20, V21, V22, V23, V24, V25, V26, V27, V28, V29, V30, V31
};

enum class TestEnum64 {
    V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15,
    V16, V17, V18, V19, V20, V21, V22, V23, V24, V25, V26, V27, V28, V29, V30, V31,
    V32, V33, V34, V35, V36, V37, V38, V39, V40, V41, V42, V43, V44, V45, V46, V47,
    V48, V49, V50, V51, V52, V53, V54, V55, V56, V57, V58, V59, V60, V61, V62, V63
};

enum class TestEnum65 {
    V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15, V16, 
    V17, V18, V19, V20, V21, V22, V23, V24, V25, V26, V27, V28, V29, V30, V31, V32, 
    V33, V34, V35, V36, V37, V38, V39, V40, V41, V42, V43, V44, V45, V46, V47, V48, 
    V49, V50, V51, V52, V53, V54, V55, V56, V57, V58, V59, V60, V61, V62, V63, V64
};
// clang-format on

}  // namespace fixed_containers::rich_enums
