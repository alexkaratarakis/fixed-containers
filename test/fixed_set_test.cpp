#include "fixed_containers/fixed_set.hpp"

#include "mock_testing_types.hpp"
#include "test_utilities_common.hpp"

#include "fixed_containers/concepts.hpp"
#include "fixed_containers/consteval_compare.hpp"

#include <gtest/gtest.h>
#include <range/v3/iterator/concepts.hpp>
#include <range/v3/view/filter.hpp>

#include <algorithm>
#include <cmath>

namespace fixed_containers
{
namespace
{
using ES_1 = FixedSet<int, 10>;
static_assert(TriviallyCopyable<ES_1>);
static_assert(NotTrivial<ES_1>);
static_assert(StandardLayout<ES_1>);
static_assert(TriviallyCopyAssignable<ES_1>);
static_assert(TriviallyMoveAssignable<ES_1>);

static_assert(ranges::bidirectional_iterator<ES_1::iterator>);
static_assert(ranges::bidirectional_iterator<ES_1::const_iterator>);

}  // namespace

TEST(Utilities, FixedSet_DefaultCtor)
{
    constexpr FixedSet<int, 10> s1{};
    static_assert(s1.empty());
}

TEST(Utilities, FixedSet_Initializer)
{
    constexpr FixedSet<int, 10> s1{2, 4};
    static_assert(s1.size() == 2);

    constexpr FixedSet<int, 10> s2{3};
    static_assert(s2.size() == 1);
}

TEST(Utilities, FixedSet_Find_TransparentComparator)
{
    constexpr FixedSet<MockAComparableToB, 3, std::less<>> s{};
    constexpr MockBComparableToA b{5};
    static_assert(s.find(b) == s.end());
}

TEST(Utilities, FixedSet_Contains)
{
    constexpr FixedSet<int, 10> s1{2, 4};
    static_assert(s1.size() == 2);

    static_assert(!s1.contains(1));
    static_assert(s1.contains(2));
    static_assert(!s1.contains(3));
    static_assert(s1.contains(4));
}

TEST(Utilities, FixedSet_Contains_TransparentComparator)
{
    constexpr FixedSet<MockAComparableToB, 5, std::less<>> s{
        MockAComparableToB{1}, MockAComparableToB{3}, MockAComparableToB{5}};
    constexpr MockBComparableToA b{5};
    static_assert(s.contains(b));
}

TEST(Utilities, FixedSet_Count_TransparentComparator)
{
    constexpr FixedSet<MockAComparableToB, 5, std::less<>> s{
        MockAComparableToB{1}, MockAComparableToB{3}, MockAComparableToB{5}};
    constexpr MockBComparableToA b{5};
    static_assert(s.count(b) == 1);
}

TEST(Utilities, FixedSet_MaxSize)
{
    constexpr FixedSet<int, 10> s1{2, 4};
    static_assert(s1.max_size() == 10);
}

TEST(Utilities, FixedSet_EmptySizeFull)
{
    constexpr FixedSet<int, 10> s1{2, 4};
    static_assert(s1.size() == 2);
    static_assert(!s1.empty());

    constexpr FixedSet<int, 10> s2{};
    static_assert(s2.size() == 0);
    static_assert(s2.empty());

    constexpr FixedSet<int, 2> s3{2, 4};
    static_assert(s3.size() == 2);
    static_assert(s3.full());

    constexpr FixedSet<int, 5> s4{2, 4};
    static_assert(s4.size() == 2);
    static_assert(!s4.full());
}

TEST(Utilities, FixedSet_Insert)
{
    constexpr auto s1 = []()
    {
        FixedSet<int, 10> s{};
        s.insert(2);
        s.insert(4);
        return s;
    }();

    static_assert(s1.size() == 2);
    static_assert(!s1.contains(1));
    static_assert(s1.contains(2));
    static_assert(!s1.contains(3));
    static_assert(s1.contains(4));
}

TEST(Utilities, FixedSet_InsertMultipleTimes)
{
    constexpr auto s1 = []()
    {
        FixedSet<int, 10> s{};
        {
            auto [it, was_inserted] = s.insert(2);
            assert(was_inserted);
            assert(2 == *it);
        }
        {
            auto [it, was_inserted] = s.insert(4);
            assert(was_inserted);
            assert(4 == *it);
        }
        {
            auto [it, was_inserted] = s.insert(2);
            assert(!was_inserted);
            assert(2 == *it);
        }
        {
            auto [it, was_inserted] = s.insert(4);
            assert(!was_inserted);
            assert(4 == *it);
        }
        return s;
    }();

    static_assert(s1.size() == 2);
    static_assert(!s1.contains(1));
    static_assert(s1.contains(2));
    static_assert(!s1.contains(3));
    static_assert(s1.contains(4));
}

TEST(Utilities, FixedSet_InsertInitializer)
{
    constexpr auto s1 = []()
    {
        FixedSet<int, 10> s{};
        s.insert({2, 4});
        return s;
    }();

    static_assert(s1.size() == 2);
    static_assert(!s1.contains(1));
    static_assert(s1.contains(2));
    static_assert(!s1.contains(3));
    static_assert(s1.contains(4));
}

TEST(Utilities, FixedSet_Insert_Iterators)
{
    constexpr auto s1 = []()
    {
        FixedSet<int, 10> s{};
        std::array<int, 2> a{2, 4};
        s.insert(a.begin(), a.end());
        return s;
    }();

    static_assert(s1.size() == 2);
    static_assert(!s1.contains(1));
    static_assert(s1.contains(2));
    static_assert(!s1.contains(3));
    static_assert(s1.contains(4));

    static_assert(std::is_same_v<decltype(*s1.begin()), const int&>);

    FixedSet<int, 10> s_non_const{};
    static_assert(std::is_same_v<decltype(*s_non_const.begin()), const int&>);
}

TEST(Utilities, FixedSet_Clear)
{
    constexpr auto s1 = []()
    {
        FixedSet<int, 10> s{2, 4};
        s.clear();
        return s;
    }();

    static_assert(s1.empty());
}

TEST(Utilities, FixedSet_Erase)
{
    constexpr auto s1 = []()
    {
        FixedSet<int, 10> s{2, 4};
        auto removed_count = s.erase(2);
        assert(removed_count == 1);
        removed_count = s.erase(3);
        assert(removed_count == 0);
        return s;
    }();

    static_assert(s1.size() == 1);
    static_assert(!s1.contains(1));
    static_assert(!s1.contains(2));
    static_assert(!s1.contains(3));
    static_assert(s1.contains(4));
}

TEST(Utilities, FixedSet_EraseIterator)
{
    constexpr auto s1 = []()
    {
        FixedSet<int, 10> s{2, 3, 4};
        {
            auto it = s.begin();
            auto next = s.erase(it);
            assert(*next == 3);
        }

        {
            auto it = s.cbegin();
            auto next = s.erase(it);
            assert(*next == 4);
        }
        return s;
    }();

    static_assert(s1.size() == 1);
    static_assert(!s1.contains(1));
    static_assert(!s1.contains(2));
    static_assert(!s1.contains(3));
    static_assert(s1.contains(4));
}

TEST(Utilities, FixedSet_EraseIterator_InvalidIterator)
{
    FixedSet<int, 10> s{2, 4};
    {
        auto it = s.begin();
        std::advance(it, 2);
        EXPECT_DEATH(s.erase(it), "");
    }
}

TEST(Utilities, FixedSet_EraseRange)
{
    {
        constexpr auto s1 = []()
        {
            FixedSet<int, 10> s{2, 3, 4};
            auto from = s.begin();
            std::advance(from, 1);
            auto to = s.begin();
            std::advance(to, 2);
            auto next = s.erase(from, to);
            assert(*next == 4);
            return s;
        }();

        static_assert(consteval_compare::equal<2, s1.size()>);
        static_assert(!s1.contains(1));
        static_assert(s1.contains(2));
        static_assert(!s1.contains(3));
        static_assert(s1.contains(4));
    }
    {
        constexpr auto s1 = []()
        {
            FixedSet<int, 10> s{2, 4};
            auto from = s.begin();
            auto to = s.begin();
            auto next = s.erase(from, to);
            assert(*next == 2);
            return s;
        }();

        static_assert(consteval_compare::equal<2, s1.size()>);
        static_assert(!s1.contains(1));
        static_assert(s1.contains(2));
        static_assert(!s1.contains(3));
        static_assert(s1.contains(4));
    }
    {
        constexpr auto s1 = []()
        {
            FixedSet<int, 10> s{1, 4};
            auto from = s.begin();
            auto to = s.end();
            auto next = s.erase(from, to);
            assert(next == s.end());
            return s;
        }();

        static_assert(consteval_compare::equal<0, s1.size()>);
        static_assert(!s1.contains(1));
        static_assert(!s1.contains(2));
        static_assert(!s1.contains(3));
        static_assert(!s1.contains(4));
    }
}

TEST(Utilities, FixedSet_IteratorBasic)
{
    constexpr FixedSet<int, 10> s1{1, 2, 3, 4};

    static_assert(std::distance(s1.cbegin(), s1.cend()) == 4);

    static_assert(*s1.begin() == 1);
    static_assert(*std::next(s1.begin(), 1) == 2);
    static_assert(*std::next(s1.begin(), 2) == 3);
    static_assert(*std::next(s1.begin(), 3) == 4);

    static_assert(*std::prev(s1.end(), 1) == 4);
    static_assert(*std::prev(s1.end(), 2) == 3);
    static_assert(*std::prev(s1.end(), 3) == 2);
    static_assert(*std::prev(s1.end(), 4) == 1);
}

TEST(Utilities, FixedSet_Iterator_OffByOneIssues)
{
    constexpr FixedSet<int, 10> s1{{1, 4}};

    static_assert(std::distance(s1.cbegin(), s1.cend()) == 2);

    static_assert(*s1.begin() == 1);
    static_assert(*std::next(s1.begin(), 1) == 4);

    static_assert(*std::prev(s1.end(), 1) == 4);
    static_assert(*std::prev(s1.end(), 2) == 1);
}

TEST(Utilities, FixedSet_Iterator_EnsureOrder)
{
    constexpr auto s1 = []()
    {
        FixedSet<int, 10> s{};
        s.insert(3);
        s.insert(4);
        s.insert(1);
        return s;
    }();

    static_assert(std::distance(s1.cbegin(), s1.cend()) == 3);

    static_assert(*s1.begin() == 1);
    static_assert(*std::next(s1.begin(), 1) == 3);
    static_assert(*std::next(s1.begin(), 2) == 4);

    static_assert(*std::prev(s1.end(), 1) == 4);
    static_assert(*std::prev(s1.end(), 2) == 3);
    static_assert(*std::prev(s1.end(), 3) == 1);
}

TEST(Utilities, FixedSet_ReverseIteratorBasic)
{
    constexpr FixedSet<int, 10> s1{1, 2, 3, 4};

    static_assert(consteval_compare::equal<4, std::distance(s1.crbegin(), s1.crend())>);

    static_assert(*s1.rbegin() == 4);
    static_assert(*std::next(s1.rbegin(), 1) == 3);
    static_assert(*std::next(s1.crbegin(), 2) == 2);
    static_assert(*std::next(s1.rbegin(), 3) == 1);

    static_assert(*std::prev(s1.rend(), 1) == 1);
    static_assert(*std::prev(s1.crend(), 2) == 2);
    static_assert(*std::prev(s1.rend(), 3) == 3);
    static_assert(*std::prev(s1.rend(), 4) == 4);
}

TEST(Utilities, FixedSet_Equality)
{
    constexpr FixedSet<int, 10> s1{{1, 4}};
    constexpr FixedSet<int, 10> s2{{4, 1}};
    constexpr FixedSet<int, 10> s3{{1, 3}};
    constexpr FixedSet<int, 10> s4{1};

    static_assert(s1 == s2);
    static_assert(s2 == s1);

    static_assert(s1 != s3);
    static_assert(s3 != s1);

    static_assert(s1 != s4);
    static_assert(s4 != s1);
}

TEST(Utilities, FixedSet_Ranges)
{
    FixedSet<int, 10> s1{1, 4};
    auto f = s1 | ranges::views::filter([](const auto& v) -> bool { return v == 4; });

    EXPECT_EQ(1, ranges::distance(f));
    EXPECT_EQ(4, *f.begin());
}

}  // namespace fixed_containers