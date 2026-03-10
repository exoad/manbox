/*
Copyright (c) 2026, Jiaming Meng (jackm@exoad.net)

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <climits>
#include <cassert>

#include <memory>
#include <string>
#include <string_view>
#include <functional>
#include <optional>
#include <variant>
#include <expected>
#include <span>
#include <array>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <stack>
#include <queue>
#include <bitset>
#include <tuple>
#include <pair>

#include <type_traits>
#include <concepts>
#include <utility>
#include <limits>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <ranges>
#include <compare>

#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <thread>
#include <future>
#include <semaphore>
#include <latch>
#include <barrier>

#include <chrono>
#include <ratio>

#include <stdexcept>
#include <exception>
#include <system_error>

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#include <filesystem>
#include <regex>
#include <random>
#include <complex>
#include <valarray>
#include <any>
#include <typeindex>
#include <typeinfo>
#include <source_location>
#include <bit>
#include <numbers>
#include <version>

using Int8    = int8_t;
using Int16   = int16_t;
using Int32   = int32_t;
using Int64   = int64_t;
using UInt8   = uint8_t;
using UInt16  = uint16_t;
using UInt32  = uint32_t;
using UInt64  = uint64_t;
using Float32 = float;
using Float64 = double;
using Float128 = long double;
using Utf8    = char;
using Utf16   = char16_t;
using Utf32   = char32_t;
using WChar   = wchar_t;
using Bool    = bool;
using Byte    = std::byte;
using Size    = std::size_t;
using PtrDiff = std::ptrdiff_t;
using UPtr    = std::uintptr_t;
using IPtr    = std::intptr_t;
using Void    = void;
using Any     = void*;
using CFile   = std::FILE;
using NullPtr = std::nullptr_t;
using Str      = std::string;
using StrView  = std::string_view;
using WStr     = std::wstring;
using WStrView = std::wstring_view;
using Str16    = std::u16string;
using Str32    = std::u32string;
using Str8     = std::u8string;
template<typename T>
using UniqPtr = std::unique_ptr<T>;
template<typename T>
using SharedPtr = std::shared_ptr<T>;
template<typename T>
using WeakPtr = std::weak_ptr<T>;
template<typename T>
using Opt = std::optional<T>;
template<typename... Ts>
using Variant = std::variant<Ts...>;
template<typename T, typename E>
using Expected = std::expected<T, E>;
template<typename T>
using Span = std::span<T>;
template<typename T, Size N>
using Arr = std::array<T, N>;
template<typename F, typename S>
using Pair = std::pair<F, S>;
template<typename... Ts>
using Tuple = std::tuple<Ts...>;
template<typename Sig>
using Fn = std::function<Sig>;
template<typename T>
using Complex = std::complex<T>;

template<typename T>
using ValArr = std::valarray<T>;

template<Size N>
using BitSet = std::bitset<N>;

using AnyVal   = std::any;
using TypeIdx  = std::type_index;
using TypeInfo = std::type_info;
using Monostate = std::monostate;
using SrcLoc   = std::source_location;
using ErrCode  = std::error_code;
using ErrCond  = std::error_condition;
using ErrCat   = std::error_category;

template<typename T>
using Vec = std::vector<T>;

template<typename T>
using Deque = std::deque<T>;

template<typename T>
using List = std::list<T>;

template<typename T>
using FwdList = std::forward_list<T>;

template<typename K, typename V>
using Map = std::map<K, V>;

template<typename K, typename V>
using MultiMap = std::multimap<K, V>;

template<typename K, typename V>
using HashMap = std::unordered_map<K, V>;

template<typename K, typename V>
using HashMultiMap = std::unordered_multimap<K, V>;

template<typename T>
using Set = std::set<T>;

template<typename T>
using MultiSet = std::multiset<T>;

template<typename T>
using HashSet = std::unordered_set<T>;

template<typename T>
using HashMultiSet = std::unordered_multiset<T>;

template<typename T>
using Stack = std::stack<T>;

template<typename T>
using Queue = std::queue<T>;

template<typename T>
using PrioQueue = std::priority_queue<T>;

template<typename T>
using InitList = std::initializer_list<T>;

template<typename Iterator>
using IterTraits = std::iterator_traits<Iterator>;

template<typename T>
using ReverseIter = std::reverse_iterator<T>;
using StrongOrdering  = std::strong_ordering;
using WeakOrdering    = std::weak_ordering;
using PartialOrdering = std::partial_ordering;
using Thread   = std::thread;
using JThread  = std::jthread;
using Mutex    = std::mutex;
using RMutex   = std::recursive_mutex;
using TMutex   = std::timed_mutex;
using RTMutex  = std::recursive_timed_mutex;
using SRMutex  = std::shared_mutex;
using SRTMutex = std::shared_timed_mutex;
using CondVar  = std::condition_variable;
using CondVarAny = std::condition_variable_any;
using OnceFlag = std::once_flag;
template<typename T>
using Atomic = std::atomic<T>;
template<typename T>
using Future = std::future<T>;
template<typename T>
using SharedFuture = std::shared_future<T>;

template<typename T>
using Promise = std::promise<T>;

template<typename T>
using PackagedTask = std::packaged_task<T>;

template<std::ptrdiff_t LEAST_MAX>
using CountingSem = std::counting_semaphore<LEAST_MAX>;

using BinarySem = std::binary_semaphore;
using Latch     = std::latch;

template<typename CompletionFn = std::ptrdiff_t>
using Barrier = std::barrier<CompletionFn>;

template<typename T>
using LockGuard = std::lock_guard<T>;

template<typename T>
using UniqLock = std::unique_lock<T>;

template<typename T>
using SharedLock = std::shared_lock<T>;

template<typename... Mutexes>
using ScopedLock = std::scoped_lock<Mutexes...>;
using SysClock    = std::chrono::system_clock;
using SteadyClock = std::chrono::steady_clock;
using HiResClock  = std::chrono::high_resolution_clock;

using Nanoseconds  = std::chrono::nanoseconds;
using Microseconds = std::chrono::microseconds;
using Milliseconds = std::chrono::milliseconds;
using Seconds      = std::chrono::seconds;
using Minutes      = std::chrono::minutes;
using Hours        = std::chrono::hours;
using Days         = std::chrono::days;
using Weeks        = std::chrono::weeks;
using Months       = std::chrono::months;
using Years        = std::chrono::years;

template<typename Rep, typename Period = std::ratio<1>>
using Duration = std::chrono::duration<Rep, Period>;

template<typename Clock, typename Duration = typename Clock::duration>
using TimePoint = std::chrono::time_point<Clock, Duration>;

using IStream   = std::istream;
using OStream   = std::ostream;
using IOStream  = std::iostream;
using IFStream  = std::ifstream;
using OFStream  = std::ofstream;
using FStream   = std::fstream;
using IStrStream = std::istringstream;
using OStrStream = std::ostringstream;
using StrStream  = std::stringstream;

using Path      = std::filesystem::path;
using DirEntry  = std::filesystem::directory_entry;
using DirIter   = std::filesystem::directory_iterator;
using RecDirIter = std::filesystem::recursive_directory_iterator;
using FileStatus = std::filesystem::file_status;
using SpaceInfo  = std::filesystem::space_info;
using FileTime   = std::filesystem::file_time_type;
using FsErrCode  = std::filesystem::filesystem_error;
using FileType   = std::filesystem::file_type;
using Perms      = std::filesystem::perms;
using CopyOpts   = std::filesystem::copy_options;

using RandDev      = std::random_device;
using Mt19937      = std::mt19937;
using Mt19937_64   = std::mt19937_64;

template<typename IntType = Int32>
using UniformIntDist = std::uniform_int_distribution<IntType>;

template<typename RealType = Float64>
using UniformRealDist = std::uniform_real_distribution<RealType>;

template<typename RealType = Float64>
using NormalDist = std::normal_distribution<RealType>;

template<typename RealType = Float64>
using BernoulliDist = std::bernoulli_distribution;

template<typename IntType = Int32>
using PoissonDist = std::poisson_distribution<IntType>;

using Regex      = std::regex;
using SMatch     = std::smatch;
using SSMatch    = std::ssub_match;
using RegexErr   = std::regex_error;
using RegexFlags = std::regex_constants::syntax_option_type;

using Exception        = std::exception;
using RuntimeErr       = std::runtime_error;
using LogicErr         = std::logic_error;
using RangeErr         = std::range_error;
using OverflowErr      = std::overflow_error;
using UnderflowErr     = std::underflow_error;
using OutOfRangeErr    = std::out_of_range;
using InvalidArgErr    = std::invalid_argument;
using LengthErr        = std::length_error;
using DomainErr        = std::domain_error;
using BadAlloc         = std::bad_alloc;
using BadCast          = std::bad_cast;
using BadTypeid        = std::typeid;
using BadOptAccess     = std::bad_optional_access;
using BadVariantAccess = std::bad_variant_access;
using BadAnyAccess     = std::bad_any_cast;
using BadWeakPtr       = std::bad_weak_ptr;
using BadFnCall        = std::bad_function_call;
using SysErr           = std::system_error;
using ExceptionPtr     = std::exception_ptr;
using NestedExc        = std::nested_exception;

template<typename T>
using RemoveRef = std::remove_reference_t<T>;

template<typename T>
using RemoveConst = std::remove_const_t<T>;

template<typename T>
using RemoveCV = std::remove_cv_t<T>;

template<typename T>
using RemoveCVRef = std::remove_cvref_t<T>;

template<typename T>
using RemovePtr = std::remove_pointer_t<T>;

template<typename T>
using RemoveExtent = std::remove_extent_t<T>;

template<typename T>
using RemoveAllExtents = std::remove_all_extents_t<T>;

template<typename T>
using AddConst = std::add_const_t<T>;

template<typename T>
using AddLRef = std::add_lvalue_reference_t<T>;

template<typename T>
using AddRRef = std::add_rvalue_reference_t<T>;

template<typename T>
using AddPtr = std::add_pointer_t<T>;

template<typename T>
using MakeSigned = std::make_signed_t<T>;

template<typename T>
using MakeUnsigned = std::make_unsigned_t<T>;

template<typename T>
using Decay = std::decay_t<T>;

template<typename T>
using Underlying = std::underlying_type_t<T>;

template<typename T>
using TypeIdentity = std::type_identity_t<T>;

template<Bool COND, typename T = void>
using EnableIf = std::enable_if_t<COND, T>;

template<Bool COND, typename T, typename F>
using Conditional = std::conditional_t<COND, T, F>;

template<typename... Ts>
using CommonType = std::common_type_t<Ts...>;

template<typename T>
using InvokeResult = std::invoke_result_t<T>;

template<typename... Ts>
using VoidT = std::void_t<Ts...>;

template<typename T>
using NumLimits = std::numeric_limits<T>;
using Nano  = std::nano;
using Micro = std::micro;
using Milli = std::milli;
using Centi = std::centi;
using Deci  = std::deci;
using Kilo  = std::kilo;
using Mega  = std::mega;
using Giga  = std::giga;
using Tera  = std::tera;
using Peta  = std::peta;
using Exa   = std::exa;
constexpr Float64 PI         = std::numbers::pi;
constexpr Float64 E          = std::numbers::e;
constexpr Float64 PHI        = std::numbers::phi;
constexpr Float64 SQRT2      = std::numbers::sqrt2;
constexpr Float64 SQRT3      = std::numbers::sqrt3;
constexpr Float64 LN2        = std::numbers::ln2;
constexpr Float64 LN10       = std::numbers::ln10;
constexpr Float64 LOG2E      = std::numbers::log2e;
constexpr Float64 LOG10E     = std::numbers::log10e;
constexpr Float64 INV_PI     = std::numbers::inv_pi;
constexpr Float64 INV_SQRT_PI = std::numbers::inv_sqrtpi;
constexpr Float64 EGAMMA     = std::numbers::egamma;
template<typename T, typename... Args>
UniqPtr<T> makeUniq(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
SharedPtr<T> staticPtrCast(const SharedPtr<auto>& ptr) noexcept
{
    return std::static_pointer_cast<T>(ptr);
}

template<typename T>
SharedPtr<T> dynamicPtrCast(const SharedPtr<auto>& ptr) noexcept
{
    return std::dynamic_pointer_cast<T>(ptr);
}

template<typename T>
SharedPtr<T> constPtrCast(const SharedPtr<auto>& ptr) noexcept
{
    return std::const_pointer_cast<T>(ptr);
}

template<typename T>
constexpr RemoveRef<T>&& mv(T&& t) noexcept
{
    return std::move(t);
}

template<typename T>
constexpr T&& fwd(RemoveRef<T>& t) noexcept
{
    return std::forward<T>(t);
}

template<typename T>
constexpr T&& fwd(RemoveRef<T>&& t) noexcept
{
    return std::forward<T>(t);
}

template<typename T>
constexpr Decay<T> decay(T&& t) noexcept(noexcept(Decay<T>(std::forward<T>(t))))
{
    return std::forward<T>(t);
}

template<typename T>
void swp(T& a, T& b) noexcept(std::is_nothrow_swappable_v<T>)
{
    std::swap(a, b);
}

template<typename T, typename U = T>
T exchange(T& obj, U&& newVal) noexcept(
    std::is_nothrow_move_constructible_v<T> &&
    std::is_nothrow_assignable_v<T&, U>)
{
    return std::exchange(obj, std::forward<U>(newVal));
}

template<typename T>
constexpr Opt<Decay<T>> some(T&& value)
{
    return std::make_optional<Decay<T>>(std::forward<T>(value));
}

constexpr std::nullopt_t none = std::nullopt;

template<typename T, typename... Ts>
constexpr Bool holdsAlt(const Variant<Ts...>& v) noexcept
{
    return std::holds_alternative<T>(v);
}

template<typename T, typename... Ts>
constexpr T& getAlt(Variant<Ts...>& v)
{
    return std::get<T>(v);
}

template<typename T, typename... Ts>
constexpr const T& getAlt(const Variant<Ts...>& v)
{
    return std::get<T>(v);
}

template<typename T, typename... Ts>
constexpr T* getIf(Variant<Ts...>* v) noexcept
{
    return std::get_if<T>(v);
}

template<typename T, typename... Ts>
constexpr const T* getIf(const Variant<Ts...>* v) noexcept
{
    return std::get_if<T>(v);
}

template<typename... Visitors>
struct Overload : Visitors...
{
    using Visitors::operator()...;
};

template<typename... Visitors>
Overload(Visitors...) -> Overload<Visitors...>;

template<typename Var, typename... Visitors>
decltype(auto) visit(Var&& var, Visitors&&... visitors)
{
    return std::visit(
        Overload{ std::forward<Visitors>(visitors)... },
        std::forward<Var>(var)
    );
}

template<typename... Ts>
constexpr Tuple<Decay<Ts>...> makeTuple(Ts&&... args)
{
    return std::make_tuple(std::forward<Ts>(args)...);
}

template<typename... Ts>
constexpr Tuple<Ts&...> tieAll(Ts&... args) noexcept
{
    return std::tie(args...);
}

template<typename... Tuples>
constexpr auto tupleCat(Tuples&&... tuples)
{
    return std::tuple_cat(std::forward<Tuples>(tuples)...);
}

template<Size I, typename Tup>
constexpr auto& tupleGet(Tup&& tup) noexcept
{
    return std::get<I>(std::forward<Tup>(tup));
}

template<typename F, typename S>
constexpr Pair<Decay<F>, Decay<S>> makePair(F&& first, S&& second)
{
    return std::make_pair(std::forward<F>(first), std::forward<S>(second));
}

template<typename T>
constexpr T absVal(T x) noexcept
{
    return std::abs(x);
}

template<typename T>
constexpr T minVal(T a, T b) noexcept
{
    return std::min(a, b);
}

template<typename T>
constexpr T maxVal(T a, T b) noexcept
{
    return std::max(a, b);
}

template<typename T>
constexpr T clampVal(T value, T lo, T hi) noexcept
{
    return std::clamp(value, lo, hi);
}

template<typename T>
constexpr Pair<T, T> minMaxVal(T a, T b) noexcept
{
    return std::minmax(a, b);
}

template<typename T>
constexpr T gcdVal(T a, T b) noexcept
{
    return std::gcd(a, b);
}

template<typename T>
constexpr T lcmVal(T a, T b) noexcept
{
    return std::lcm(a, b);
}

template<typename T>
constexpr T midpointVal(T a, T b) noexcept
{
    return std::midpoint(a, b);
}

template<typename T>
constexpr T lerpVal(T a, T b, T t) noexcept
{
    return std::lerp(a, b, t);
}

template<std::integral T>
constexpr Bool hasOneBit(T x) noexcept
{
    return std::has_single_bit(x);
}

template<std::integral T>
constexpr T bitCeil(T x) noexcept
{
    return std::bit_ceil(x);
}

template<std::integral T>
constexpr T bitFloor(T x) noexcept
{
    return std::bit_floor(x);
}

template<std::integral T>
constexpr Int32 bitWidth(T x) noexcept
{
    return std::bit_width(x);
}

template<std::integral T>
constexpr T rotl(T x, Int32 s) noexcept
{
    return std::rotl(x, s);
}

template<std::integral T>
constexpr T rotr(T x, Int32 s) noexcept
{
    return std::rotr(x, s);
}

template<std::integral T>
constexpr Int32 countl0(T x) noexcept
{
    return std::countl_zero(x);
}

template<std::integral T>
constexpr Int32 countl1(T x) noexcept
{
    return std::countl_one(x);
}

template<std::integral T>
constexpr Int32 countr0(T x) noexcept
{
    return std::countr_zero(x);
}

template<std::integral T>
constexpr Int32 countr1(T x) noexcept
{
    return std::countr_one(x);
}

template<std::integral T>
constexpr Int32 popCount(T x) noexcept
{
    return std::popcount(x);
}

template<typename To, typename From>
constexpr To bitCast(const From& from) noexcept
{
    return std::bit_cast<To>(from);
}

template<typename Range, typename T>
auto findIn(Range&& r, const T& value)
{
    return std::find(std::begin(r), std::end(r), value);
}

template<typename Range, typename Pred>
auto findIf(Range&& r, Pred&& pred)
{
    return std::find_if(std::begin(r), std::end(r), std::forward<Pred>(pred));
}

template<typename Range, typename Pred>
Bool anyOf(Range&& r, Pred&& pred)
{
    return std::any_of(std::begin(r), std::end(r), std::forward<Pred>(pred));
}

template<typename Range, typename Pred>
Bool allOf(Range&& r, Pred&& pred)
{
    return std::all_of(std::begin(r), std::end(r), std::forward<Pred>(pred));
}

template<typename Range, typename Pred>
Bool noneOf(Range&& r, Pred&& pred)
{
    return std::none_of(std::begin(r), std::end(r), std::forward<Pred>(pred));
}

template<typename Range, typename Fn>
Fn forEach(Range&& r, Fn&& fn)
{
    return std::for_each(std::begin(r), std::end(r), std::forward<Fn>(fn));
}

template<typename Range>
void sortRange(Range&& r)
{
    std::sort(std::begin(r), std::end(r));
}

template<typename Range, typename Cmp>
void sortRange(Range&& r, Cmp&& cmp)
{
    std::sort(std::begin(r), std::end(r), std::forward<Cmp>(cmp));
}

template<typename Range>
void stableSort(Range&& r)
{
    std::stable_sort(std::begin(r), std::end(r));
}

template<typename Range, typename Cmp>
void stableSort(Range&& r, Cmp&& cmp)
{
    std::stable_sort(std::begin(r), std::end(r), std::forward<Cmp>(cmp));
}

template<typename Range, typename T>
Bool binSearch(Range&& r, const T& value)
{
    return std::binary_search(std::begin(r), std::end(r), value);
}

template<typename Range, typename T>
auto lowerBound(Range&& r, const T& value)
{
    return std::lower_bound(std::begin(r), std::end(r), value);
}

template<typename Range, typename T>
auto upperBound(Range&& r, const T& value)
{
    return std::upper_bound(std::begin(r), std::end(r), value);
}

template<typename Range, typename T>
Size countIn(Range&& r, const T& value)
{
    return static_cast<Size>(std::count(std::begin(r), std::end(r), value));
}

template<typename Range, typename Pred>
Size countIf(Range&& r, Pred&& pred)
{
    return static_cast<Size>(std::count_if(std::begin(r), std::end(r), std::forward<Pred>(pred)));
}

template<typename Range>
auto minElem(Range&& r)
{
    return std::min_element(std::begin(r), std::end(r));
}

template<typename Range>
auto maxElem(Range&& r)
{
    return std::max_element(std::begin(r), std::end(r));
}

template<typename Range>
auto minMaxElem(Range&& r)
{
    return std::minmax_element(std::begin(r), std::end(r));
}

template<typename Range, typename T>
void fillRange(Range&& r, const T& value)
{
    std::fill(std::begin(r), std::end(r), value);
}

template<typename Range>
void reverseRange(Range&& r)
{
    std::reverse(std::begin(r), std::end(r));
}

template<typename Range>
void uniqueRange(Range&& r)
{
    std::unique(std::begin(r), std::end(r));
}

template<typename Range, typename T>
void removeFrom(Range&& r, const T& value)
{
    std::remove(std::begin(r), std::end(r), value);
}

template<typename Range, typename Pred>
void removeIf(Range&& r, Pred&& pred)
{
    std::remove_if(std::begin(r), std::end(r), std::forward<Pred>(pred));
}

template<typename Range, typename T>
T accumulate(Range&& r, T init)
{
    return std::accumulate(std::begin(r), std::end(r), init);
}

template<typename Range, typename T, typename BinOp>
T accumulate(Range&& r, T init, BinOp&& op)
{
    return std::accumulate(std::begin(r), std::end(r), init, std::forward<BinOp>(op));
}

template<typename Range, typename T>
T reduce(Range&& r, T init)
{
    return std::reduce(std::begin(r), std::end(r), init);
}

inline Str toStr(Int32 v)        { return std::to_string(v); }
inline Str toStr(Int64 v)        { return std::to_string(v); }
inline Str toStr(UInt32 v)       { return std::to_string(v); }
inline Str toStr(UInt64 v)       { return std::to_string(v); }
inline Str toStr(Float32 v)      { return std::to_string(v); }
inline Str toStr(Float64 v)      { return std::to_string(v); }
inline Str toStr(Float128 v)     { return std::to_string(v); }

inline Int32   toInt(const Str& s)     { return std::stoi(s); }
inline Int64   toLong(const Str& s)    { return std::stoll(s); }
inline UInt64  toULong(const Str& s)   { return std::stoull(s); }
inline Float32 toFloat(const Str& s)   { return std::stof(s); }
inline Float64 toDouble(const Str& s)  { return std::stod(s); }


inline void memCopy(Void* dst, const Void* src, Size n) noexcept
{
    std::memcpy(dst, src, n);
}

inline void memMove(Void* dst, const Void* src, Size n) noexcept
{
    std::memmove(dst, src, n);
}

inline void memSet(Void* dst, Int32 value, Size n) noexcept
{
    std::memset(dst, value, n);
}

inline Int32 memCmp(const Void* a, const Void* b, Size n) noexcept
{
    return std::memcmp(a, b, n);
}

template<typename T>
T* addressOf(T& ref) noexcept
{
    return std::addressof(ref);
}

template<typename T>
void destroyAt(T* ptr)
{
    std::destroy_at(ptr);
}

template<typename T, typename... Args>
T* constructAt(T* ptr, Args&&... args)
{
    return std::construct_at(ptr, std::forward<Args>(args)...);
}

template<typename T, typename U>
constexpr Bool isSame = std::is_same_v<T, U>;

template<typename T>
constexpr Bool isConst = std::is_const_v<T>;

template<typename T>
constexpr Bool isRef = std::is_reference_v<T>;

template<typename T>
constexpr Bool isLRef = std::is_lvalue_reference_v<T>;

template<typename T>
constexpr Bool isRRef = std::is_rvalue_reference_v<T>;

template<typename T>
constexpr Bool isPtr = std::is_pointer_v<T>;

template<typename T>
constexpr Bool isArr = std::is_array_v<T>;

template<typename T>
constexpr Bool isEnum = std::is_enum_v<T>;

template<typename T>
constexpr Bool isClass = std::is_class_v<T>;

template<typename T>
constexpr Bool isUnion = std::is_union_v<T>;

template<typename T>
constexpr Bool isIntegral = std::is_integral_v<T>;

template<typename T>
constexpr Bool isFloat = std::is_floating_point_v<T>;

template<typename T>
constexpr Bool isArithmetic = std::is_arithmetic_v<T>;

template<typename T>
constexpr Bool isSigned = std::is_signed_v<T>;

template<typename T>
constexpr Bool isUnsigned = std::is_unsigned_v<T>;

template<typename T>
constexpr Bool isVoid = std::is_void_v<T>;

template<typename T>
constexpr Bool isNullPtr = std::is_null_pointer_v<T>;

template<typename T>
constexpr Bool isTrivial = std::is_trivial_v<T>;

template<typename T>
constexpr Bool isTriviallyCopyable = std::is_trivially_copyable_v<T>;

template<typename T>
constexpr Bool isStdLayout = std::is_standard_layout_v<T>;

template<typename T>
constexpr Bool isAbstract = std::is_abstract_v<T>;

template<typename T>
constexpr Bool isFinal = std::is_final_v<T>;

template<typename T>
constexpr Bool isPolymorphic = std::is_polymorphic_v<T>;

template<typename T>
constexpr Bool isEmpty = std::is_empty_v<T>;

template<typename T>
constexpr Bool isDefaultConstructible = std::is_default_constructible_v<T>;

template<typename T>
constexpr Bool isCopyConstructible = std::is_copy_constructible_v<T>;

template<typename T>
constexpr Bool isMoveConstructible = std::is_move_constructible_v<T>;

template<typename T>
constexpr Bool isCopyAssignable = std::is_copy_assignable_v<T>;

template<typename T>
constexpr Bool isMoveAssignable = std::is_move_assignable_v<T>;

template<typename T>
constexpr Bool isDestructible = std::is_destructible_v<T>;

template<typename Base, typename Derived>
constexpr Bool isBaseOf = std::is_base_of_v<Base, Derived>;

template<typename From, typename To>
constexpr Bool isConvertible = std::is_convertible_v<From, To>;

template<typename T, typename U>
constexpr Bool isAssignable = std::is_assignable_v<T, U>;

template<typename Fn, typename... Args>
constexpr Bool isInvocable = std::is_invocable_v<Fn, Args...>;

template<typename T>
constexpr Bool isSwappable = std::is_swappable_v<T>;

template<typename T>
constexpr Bool isNoThrowMoveConstructible = std::is_nothrow_move_constructible_v<T>;

template<typename T>
constexpr Bool isNoThrowMoveAssignable = std::is_nothrow_move_assignable_v<T>;

template<typename T>
constexpr Bool isNoThrowDestructible = std::is_nothrow_destructible_v<T>;

template<typename T>
constexpr Bool isNoThrowSwappable = std::is_nothrow_swappable_v<T>;

template<typename T>
constexpr Bool isAggregate = std::is_aggregate_v<T>;

template<typename T>
constexpr Bool isScopedEnum = std::is_scoped_enum_v<T>;

template<typename Fn, typename... Args>
constexpr decltype(auto) invoke(Fn&& fn, Args&&... args)
    noexcept(std::is_nothrow_invocable_v<Fn, Args...>)
{
    return std::invoke(std::forward<Fn>(fn), std::forward<Args>(args)...);
}

template<typename Fn, typename... Args>
using InvokeResultT = std::invoke_result_t<Fn, Args...>;

[[noreturn]] inline void throwCurrent()
{
    std::rethrow_exception(std::current_exception());
}

inline ExceptionPtr currentException() noexcept
{
    return std::current_exception();
}

inline void rethrow(ExceptionPtr p)
{
    std::rethrow_exception(p);
}

template<typename E>
[[noreturn]] inline void throwAs(E&& e)
{
    throw std::forward<E>(e);
}

inline SrcLoc here(SrcLoc loc = SrcLoc::current()) noexcept
{
    return loc;
}

template<typename Mutex>
LockGuard<Mutex> lockGuard(Mutex& m)
{
    return LockGuard<Mutex>{ m };
}

template<typename Mutex>
UniqLock<Mutex> uniqLock(Mutex& m)
{
    return UniqLock<Mutex>{ m };
}

template<typename Mutex>
SharedLock<Mutex> sharedLock(Mutex& m)
{
    return SharedLock<Mutex>{ m };
}

template<typename... Mutexes>
ScopedLock<Mutexes...> scopedLock(Mutexes&... mutexes)
{
    return ScopedLock<Mutexes...>{ mutexes... };
}

template<typename Callable, typename... Args>
auto asyncCall(Callable&& fn, Args&&... args)
{
    return std::async(std::launch::async, std::forward<Callable>(fn), std::forward<Args>(args)...);
}

inline void callOnce(OnceFlag& flag, auto&& fn)
{
    std::call_once(flag, std::forward<decltype(fn)>(fn));
}

inline TimePoint<SteadyClock> now() noexcept
{
    return SteadyClock::now();
}

inline TimePoint<SysClock> sysNow() noexcept
{
    return SysClock::now();
}

template<typename Duration>
void sleepFor(Duration&& d)
{
    std::this_thread::sleep_for(std::forward<Duration>(d));
}

template<typename Clock, typename Duration>
void sleepUntil(const TimePoint<Clock, Duration>& tp)
{
    std::this_thread::sleep_until(tp);
}

inline void yieldThread() noexcept
{
    std::this_thread::yield();
}

inline Thread::id thisThreadId() noexcept
{
    return std::this_thread::get_id();
}

namespace fs = std::filesystem;

inline Bool pathExists(const Path& p) noexcept
{
    return fs::exists(p);
}

inline Bool isFile(const Path& p) noexcept
{
    return fs::is_regular_file(p);
}

inline Bool isDir(const Path& p) noexcept
{
    return fs::is_directory(p);
}

inline Bool isSymlink(const Path& p) noexcept
{
    return fs::is_symlink(p);
}

inline Bool mkDir(const Path& p)
{
    return fs::create_directory(p);
}

inline Bool mkDirs(const Path& p)
{
    return fs::create_directories(p);
}

inline void removePath(const Path& p)
{
    fs::remove(p);
}

inline void removeAll(const Path& p)
{
    fs::remove_all(p);
}

inline void copyPath(const Path& from, const Path& to)
{
    fs::copy(from, to);
}

inline void renamePath(const Path& from, const Path& to)
{
    fs::rename(from, to);
}

inline UInt64 fileSize(const Path& p)
{
    return fs::file_size(p);
}

inline Path currentDir()
{
    return fs::current_path();
}

inline void changeDir(const Path& p)
{
    fs::current_path(p);
}

inline Path absPath(const Path& p)
{
    return fs::absolute(p);
}

inline Path canonicalPath(const Path& p)
{
    return fs::canonical(p);
}

template<typename T>
Str stringify(const T& value)
{
    OStrStream oss;
    oss << value;
    return oss.str();
}

template<typename T>
T fromString(const Str& s)
{
    T result;
    IStrStream iss{ s };
    iss >> result;
    return result;
}

#define print(...)   std::fprintf(stdout, __VA_ARGS__)
#define println(...) do { print(__VA_ARGS__); print("\n"); } while(0)
#define eprint(...)  std::fprintf(stderr, __VA_ARGS__)
#define eprintln(...) do { eprint(__VA_ARGS__); eprint("\n"); } while(0)
