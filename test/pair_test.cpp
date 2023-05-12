
#include "fixed_containers/pair.hpp"

#include "fixed_containers/concepts.hpp"
#include "fixed_containers/optional_storage.hpp"

#include <gtest/gtest.h>

#include <utility>

namespace fixed_containers
{
namespace
{
using P = std::pair<int, int>;
static_assert(NotTriviallyDefaultConstructible<P>);
static_assert(TriviallyCopyConstructible<P>);
static_assert(NotTriviallyCopyAssignable<P>);
static_assert(TriviallyMoveConstructible<P>);
static_assert(NotTriviallyMoveAssignable<P>);
static_assert(TriviallyDestructible<P>);
static_assert(NotTriviallyCopyable<P>);
}  // namespace

TEST(FixedMap, OperatorBracket_EnsureNoUnnecessaryTemporaries)
{
    std::pair<const int, int> a;
    std::pair<const int, int> a2;
    a = a2;
    std::pair<const int, const int> b;

    std::pair<const int, int>& c = a;
    const std::pair<const int, int>& d = a;
    const std::pair<const int, const int>& e = a;

    Pair<const int, int> f = a;

    const std::pair<const int, int> a0;
    Pair<const int, int> f0 = a0;
    Pair<const int, int> f1 = a0;
    f0 = f1;
}

static_assert(TriviallyCopyable<Pair<int, int>>);
static_assert(TriviallyCopyable<Pair<const int, int>>);
static_assert(CopyAssignable<Pair<const int, int>>);
static_assert(TriviallyCopyable<optional_storage_detail::OptionalStorage<Pair<const int, int>>>);
static_assert(CopyAssignable<optional_storage_detail::OptionalStorage<Pair<const int, int>>>);

}  // namespace fixed_containers
