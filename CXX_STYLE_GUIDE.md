# Jack's C++ Style Guide

This is a collection of rules and guidelines I employ for writing C++, adapted from my C style guide. It inherits the same philosophy: readable, consistent, and opinionated. This is not a comprehensive guide and does not serve to teach C++ — it assumes you already know the language deeply.

> [!WARNING]
> This guide does not cover best practices or how to write C++. It covers how to structure, format, and style your code in a specific opinionated way.

---

## Compiler Setup

- Standard: C++17 minimum, C++20 preferred.
- Preferred compilers: GCC or Clang. *Never MSVC.*

### Compiler Flags

#### Release Build

```
-std=c++20 -O2 -Wall -Wextra -Wpedantic -Wshadow -Wcast-align \
  -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast \
  -flto -D_FORTIFY_SOURCE=2 -fstack-protector-strong -march=native
```

#### Development Build

```
-std=c++20 -O1 -g -Wall -Wextra -Wpedantic -Wshadow -Wcast-align \
  -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast \
  -fsanitize=address,undefined -fno-omit-frame-pointer
```

> `-Wnon-virtual-dtor` catches the single most common silent UB source in C++: a polymorphic base class with a non-virtual destructor.

> `-Wold-style-cast` is added over the C guide. C-style casts (`(int)x`) are banned in C++ — see [Casting](#casting).

### Optimization Flags

Same policy as the C guide. `-O2` for release, `-O1` for dev builds. Avoid `-O3`.

---

## Shared Types

Use a thin alias layer via `using` declarations. No macro soup. This lives in `shared.hpp`.

```cpp
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
#include <memory>
#include <string>
#include <string_view>
#include <functional>
#include <optional>
#include <variant>
#include <cstdio>
#include <type_traits>
#include <utility>

// --- Primitive types ---
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
using Utf8    = char;
using Bool    = bool;
using Size    = std::size_t;
using UPtr    = std::uintptr_t;
using Void    = void;
using Any     = void*;
using CFile   = std::FILE;

// --- String types ---
using StrView = std::string_view;   // Preferred for read-only string parameters
using Str     = std::string;        // Preferred for owned strings

// --- Smart pointer aliases ---
template<typename T>
using UniqPtr = std::unique_ptr<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T>
using WeakPtr = std::weak_ptr<T>;

// --- Optional / sum types ---
template<typename T>
using Opt = std::optional<T>;

template<typename... Ts>
using Variant = std::variant<Ts...>;

// --- Function wrapper alias ---
template<typename Sig>
using Fn = std::function<Sig>;

// --- Factory helpers ---
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

// --- Monostate (empty variant sentinel) ---
using Monostate = std::monostate;

// --- Constants ---
constexpr Float64 PI = 3.14159265358979323846;

// --- Convenience ---
#define print(...)   std::fprintf(stdout, __VA_ARGS__)
#define println(...) do { print(__VA_ARGS__); print("\n"); } while(0)
```

> `Opt<T>` replaces nullable pointers for value types that may or may not be present. Prefer `Opt<T>` over `T*` when expressing "this value might not exist" and no heap allocation is needed.

> `Variant<Ts...>` replaces manual `union` + enum discriminant. Prefer it whenever a value can be one of several distinct types.

> `Fn<Sig>` is for stored callables. For non-stored callback parameters, prefer a constrained template parameter — it avoids the heap allocation and type-erasure overhead of `std::function`.

---

## Naming Conventions

`camelCase` for variables and functions. `PascalCase` for types. `SCREAMING_SNAKE_CASE` for macros and enum members. Same as the C guide.

### Local Variables

```cpp
Int32 myVariable = 0;   // OK
Int32 my_variable = 0;  // BAD
Int32 MyVariable = 0;   // BAD
```

> Applies to function parameters and lambda captures.

### Global / Static Scope Variables

`camelCase`. No Hungarian Notation (`g_` prefix).

```cpp
static Int32 myGlobalVar = 0;
Int32 myOtherGlobal = 0;
```

### Member Variables

`camelCase`. No `m_` prefix.

```cpp
struct Vec2
{
    Float32 x;
    Float32 y;
};
```

### Free Functions and Member Functions

`camelCase`. Member functions do not need a type prefix — the class is the namespace.

```cpp
Int32 computeSum(Int32 a, Int32 b);   // free function, OK

struct Vec2
{
    Float32 magnitude() const;        // OK
    Float32 vec2Magnitude() const;    // BAD, redundant prefix
};
```

### Types

All structs, classes, unions, enums, `using` aliases, and template type parameters: `PascalCase`.

```cpp
struct MyStruct { };
using MyAlias = Int32;
enum class Direction { };
```

### Enum Members

`SCREAMING_SNAKE_CASE` with the enum type name as a prefix.

```cpp
enum class OperatingSystem
{
    OPERATING_SYSTEM_WINDOWS,
    OPERATING_SYSTEM_LINUX,
    OPERATING_SYSTEM_MACOS,
};
```

> Always use `enum class`. Plain `enum` allows implicit integral conversions and leaks member names into the enclosing scope.

### Template Parameters

Type parameters: `PascalCase`. Non-type parameters: `SCREAMING_SNAKE_CASE`.

```cpp
template<typename ValueType, Size BUFFER_SIZE>
struct RingBuffer { };
```

### Concept Names

`PascalCase`, same as types. A concept is a named constraint — treat it like a type name.

```cpp
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<typename T>
concept Serializable = requires(T t) { t.serialize(); };
```

### Namespaces

All lowercase. Short and descriptive.

```cpp
namespace myproject { }   // OK
namespace MyProject  { }  // BAD
namespace my_project { }  // BAD, but tolerable for STL-style library naming
```

---

## Formatting

### Indentation

No strict rule. Be consistent within a project.

### Macro Indentation

Do not indent nested macros.

```cpp
#ifdef SOME_FLAG
#include "somefile.hpp"
#define OK 1
#else
#include "anotherfile.hpp"
#endif
```

### Brace Placement

Allman style throughout.

```cpp
Int32 main()
{
    if(condition)
    {
        // code
    }
    else
    {
        // code
    }
}
```

Empty bodies: braces on separate lines.

```cpp
Void doNothing()
{
}
```

### Spacing in Expressions

Maximize spacing around binary operators. No space for unary operators or adjacent parentheses.

```cpp
Int32 result = a + ~c * (ab / -c);   // OK
Int32 result = a+~c*(ab/c);          // BAD
```

### Spacing in Control Structures

No space between keyword and opening parenthesis.

```cpp
if(condition) { }   // OK
if (condition) { }  // BAD
for(...)  { }       // OK
```

`while` of a `do-while` rests on the same line as the closing brace.

```cpp
do
{
    // code
} while(condition);
```

---

## Declaration Styles

### Variables

One declaration per line.

```cpp
Int32 a;
Int32 b;

Int32 a, b;  // BAD
```

### Pointers and References

`*` and `&` bind to the type, not the identifier.

```cpp
Int32* a;    // OK
Int32 *a;    // NOT PREFERRED

Vec2& ref;   // OK
Vec2 &ref;   // NOT PREFERRED
```

### `const` Placement

1. Pointer to constant: `const TypeName* variableName`
2. Constant pointer: `TypeName* const variableName`
3. `const` method: `ReturnType methodName() const`

Prefer `constexpr` over `const` for all compile-time values.

```cpp
constexpr Float32 GRAVITY = 9.81f;  // preferred
const Float32 gravity = 9.81f;      // weaker, runtime const
```

### Arrays

Avoid VLAs. Prefer `std::array` for fixed-size, `std::vector` for dynamic.

```cpp
std::array<Int32, 10> arr;   // OK, preferred
Int32 arr[10];               // tolerable for POD local buffers
Int32 arr[n];                // BAD, VLA
```

### Structs and Classes

Use `struct` for data-focused types with no enforced invariants. Use `class` when encapsulation is required. Always declare access specifiers explicitly in the order: `public`, `protected`, `private`.

```cpp
struct Vec2
{
    Float32 x;
    Float32 y;
};

class Engine
{
public:
    Engine();
    ~Engine();

private:
    Int32 tickRate;
};
```

### Enums

Always `enum class`.

```cpp
enum class Direction
{
    DIRECTION_NORTH,
    DIRECTION_SOUTH,
    DIRECTION_EAST,
    DIRECTION_WEST,
};
```

### Initialization

Prefer brace initialization `{}` — it prevents narrowing conversions and avoids the most vexing parse.

```cpp
Int32 x{ 5 };                    // OK, preferred
Int32 x = 5;                     // OK, fine for primitives
std::vector<Int32> v{ 1, 2, 3 }; // OK

Engine e();   // BAD — this is a function declaration, not an object
Engine e{};   // OK — default construction
```

> The exception: when a type has an `std::initializer_list` constructor overload and you do NOT want it, use parentheses explicitly.

```cpp
std::vector<Int32> v(10);    // 10 default-constructed elements
std::vector<Int32> v{ 10 };  // 1 element with value 10
```

### Casting

C-style casts are banned. `-Wold-style-cast` enforces this at compile time. Always use named casts.

```cpp
Float32 f = (Float32)x;                  // BAD
Float32 f = static_cast<Float32>(x);     // OK

// Named cast reference:
// static_cast       — well-defined conversions between related types
// reinterpret_cast  — raw bit reinterpretation; use sparingly, always document why
// const_cast        — removing const; only for legacy C API interop
// dynamic_cast      — runtime-checked downcast on polymorphic types; prefer design alternatives
```

> Frequent `dynamic_cast` usage is a design smell. Prefer `std::variant` + `std::visit` or a virtual dispatch redesign.

### Function Definitions

- `inline` replaces the C guide's `simple` macro for header-defined functions.
- Always specify a return type.
- Use `[[nodiscard]]` on functions whose return value must not be silently discarded.
- Mark `noexcept` on functions that genuinely cannot throw — the compiler can optimize call sites.

```cpp
[[nodiscard]] UniqPtr<Engine> createEngine();
[[nodiscard]] inline Bool isValidIndex(Size i, Size len) noexcept;
```

### Smart Pointers

Prefer `UniqPtr<T>` by default. Use `SharedPtr<T>` only when shared ownership is genuinely required. Raw `new`/`delete` should not appear in application code.

Pass smart pointers by reference only to transfer or share ownership. For non-owning access, pass a raw pointer or reference to the underlying object.

```cpp
Void process(const Engine& engine);          // OK, non-owning read access
Void take(UniqPtr<Engine> engine);           // OK, transfers ownership
Void observe(const UniqPtr<Engine>& ptr);    // BAD, exposes ownership detail unnecessarily
```

---

## Namespaces

Use `namespace` blocks. Do not indent the contents when the namespace spans an entire file.

```cpp
namespace myproject
{

struct Vec2
{
    Float32 x;
    Float32 y;
};

}  // namespace myproject
```

Always close a namespace with a comment: `} // namespace name`.

Never `using namespace` in headers. It is tolerable in `.cpp` files for specific, targeted names only.

```cpp
using myproject::Vec2;       // OK, targeted
using namespace myproject;   // BAD in headers, tolerable in .cpp
```

### Inline Namespaces

Use inline namespaces for library versioning. The inline version is accessible at the parent namespace level; older versions remain accessible explicitly.

```cpp
namespace myproject
{
inline namespace v2
{
    struct Api { };
}
namespace v1
{
    struct Api { };
}
}

myproject::Api a;     // resolves to v2::Api
myproject::v1::Api b; // explicit v1
```

### Anonymous Namespaces

Use anonymous namespaces instead of `static` for translation-unit-local linkage in C++.

```cpp
namespace
{
    Void helperFunction() { }  // only visible in this translation unit
}

static Void helperFunction() { }  // C-style, still valid but not preferred
```

---

## Null Comparisons

Always compare explicitly. Do not rely on implicit bool conversion.

```cpp
UniqPtr<Engine> engine = ...;
if(engine != nullptr)  // OK
{
}

if(engine)  // BAD
{
}
```

---

## Templates

### Basic Syntax

Place `template<...>` on its own line above the declaration.

```cpp
template<typename T>
T clamp(T value, T lo, T hi)
{
    return value < lo ? lo : (value > hi ? hi : value);
}
```

### Full Specialization

`template<>` on its own line.

```cpp
template<>
Void myFunction<Int32>(Int32 value);
```

### Partial Specialization

Valid for class/struct templates only, not free functions.

```cpp
template<typename T>
struct IsPointer
{
    static constexpr Bool value = false;
};

template<typename T>
struct IsPointer<T*>
{
    static constexpr Bool value = true;
};
```

### Variadic Templates

Name the parameter pack descriptively. Prefer fold expressions over recursive expansion — they produce no recursive instantiations and are easier to read.

```cpp
// Old recursive style — avoid
template<typename T>
Void printAll(T&& t) { print("%s", t); }

template<typename T, typename... Rest>
Void printAll(T&& t, Rest&&... rest)
{
    print("%s", t);
    printAll(std::forward<Rest>(rest)...);
}

// Preferred fold expression style (C++17)
template<typename... Args>
Void printAll(Args&&... args)
{
    (print("%s", std::forward<Args>(args)), ...);
}
```

### Fold Expressions

There are four fold forms. Know all of them.

```cpp
// Unary right fold: (pack op ...)
(args + ...)             // a + (b + (c + d))

// Unary left fold: (... op pack)
(... + args)             // ((a + b) + c) + d

// Binary right fold: (pack op ... op init)
(args + ... + 0)         // a + (b + (c + 0))

// Binary left fold: (init op ... op pack)
(0 + ... + args)         // ((0 + a) + b) + c
```

### Template Template Parameters

`PascalCase`, same as type parameters.

```cpp
template<template<typename> typename Container, typename T>
Void fillWith(Container<T>& c, T value)
{
    std::fill(c.begin(), c.end(), value);
}
```

### Explicit Template Instantiation

For reducing compile times in large projects, use explicit instantiation declarations in headers and definitions in exactly one `.cpp` file.

```cpp
// header.hpp
extern template class std::vector<Int32>;

// impl.cpp
template class std::vector<Int32>;
```

### CRTP

CRTP is acceptable for static polymorphism when virtual dispatch overhead is unacceptable. Name CRTP base classes with a `Base` suffix.

```cpp
template<typename Derived>
struct SerializableBase
{
    Void serialize() const
    {
        static_cast<const Derived*>(this)->serializeImpl();
    }
};

struct MyData : SerializableBase<MyData>
{
    Void serializeImpl() const;
};
```

> C++23's deducing `this` (explicit object parameter) supersedes CRTP for new code targeting C++23.

```cpp
// C++23 deducing this — preferred over CRTP when available
struct SerializableBase
{
    Void serialize(this auto const& self)
    {
        self.serializeImpl();
    }
};
```

### `typename` vs `class`

Prefer `typename` over `class` for all type template parameters. They are semantically identical in this context, but `typename` is more explicit that any type is accepted, not just class types.

```cpp
template<typename T>   // OK, preferred
template<class T>      // tolerable, but inconsistent
```

### Dependent Names

In a template, when accessing a member or type from a dependent type (a type that depends on a template parameter), use `typename` for types and `template` for template members.

```cpp
template<typename Container>
Void process(Container& c)
{
    typename Container::iterator it = c.begin();  // 'typename' required — dependent type

    c.template emplace<Int32>(value);  // 'template' required — dependent template member
}
```

> Forgetting `typename` on a dependent type is a common compile error with confusing messages. If the compiler says something is "not a type", add `typename`.

---

## Concepts and Constraints

Always constrain template parameters when the requirement is not "accept any type". Unconstrained templates produce notoriously difficult error messages.

```cpp
// Unconstrained — bad error messages, accepts nonsense types
template<typename T>
T add(T a, T b) { return a + b; }

// Constrained — clear intent, good errors
template<typename T>
    requires std::is_arithmetic_v<T>
T add(T a, T b) { return a + b; }
```

### Constraint Placement Style

Put the `requires` clause on its own line, indented once under the template declaration.

```cpp
template<typename T>
    requires std::is_arithmetic_v<T>
T lerp(T a, T b, Float32 t);
```

Use the shorthand concept syntax when the constraint is a single named concept:

```cpp
template<Numeric T>
T lerp(T a, T b, Float32 t);   // preferred when there is one named concept
```

Use the full `requires` form when combining multiple constraints.

```cpp
template<typename T>
    requires Numeric<T> && std::copyable<T>
T clamp(T value, T lo, T hi);
```

### Compound Concepts

```cpp
template<typename T>
concept Container = requires(T c)
{
    { c.begin() } -> std::input_iterator;
    { c.end()   } -> std::input_iterator;
    { c.size()  } -> std::convertible_to<Size>;
};
```

### `requires` Expressions

A `requires` expression evaluates at compile time to `true` or `false`. Use it to check for member function existence, associated types, and expression validity.

```cpp
template<typename T>
concept Printable = requires(T t)
{
    { t.toString() } -> std::convertible_to<Str>;
};

template<typename T>
concept HasValueType = requires
{
    typename T::ValueType;  // checks that T::ValueType names a type
};
```

### `if constexpr` vs Overloads

Prefer constrained overloads for fundamentally different behaviors. Use `if constexpr` for minor type-specific branching within a single function body.

```cpp
// Prefer overloads for fundamentally different behavior
template<Numeric T>
Void process(T value) { /* numeric path */ }

template<Container T>
Void process(T& value) { /* container path */ }

// if constexpr for minor branching within one function
template<typename T>
Void serialize(const T& value)
{
    if constexpr(std::is_trivially_copyable_v<T>)
    {
        // fast memcpy path
    }
    else
    {
        // general path
    }
}
```

---

## Metaprogramming

### Type Traits

Prefer the `_v` and `_t` helper aliases over `::value` and `::type`.

```cpp
std::is_same_v<T, Int32>                  // OK, preferred
std::is_same<T, Int32>::value             // BAD, verbose

std::remove_reference_t<T>               // OK, preferred
typename std::remove_reference<T>::type  // BAD, verbose
```

### Custom Type Traits

Follow the same `_v` / `_t` convention.

```cpp
template<typename T>
struct IsOwningPtr : std::false_type {};

template<typename T>
struct IsOwningPtr<UniqPtr<T>> : std::true_type {};

template<typename T>
struct IsOwningPtr<SharedPtr<T>> : std::true_type {};

template<typename T>
constexpr Bool IsOwningPtr_v = IsOwningPtr<T>::value;
```

### `std::void_t` for Detection (C++17)

Use `std::void_t` to test for member existence or expression validity in C++17. Prefer `requires` in C++20.

```cpp
// C++17 detection idiom
template<typename T, typename = std::void_t<>>
struct HasSerialize : std::false_type {};

template<typename T>
struct HasSerialize<T, std::void_t<decltype(std::declval<T>().serialize())>>
    : std::true_type {};

// C++20 equivalent — always prefer this when available
template<typename T>
concept HasSerialize = requires(T t) { t.serialize(); };
```

### `std::declval`

Use `std::declval<T>()` inside `decltype` and `requires` expressions to reference a type without constructing it. Never use it in an evaluated context.

```cpp
// OK — unevaluated context
using ReturnType = decltype(std::declval<MyClass>().compute());

// BAD — evaluated context, undefined behavior
auto val = std::declval<MyClass>().compute();
```

### Tag Dispatch

Tag dispatch selects function overloads at compile time via dummy tag types. Prefer `if constexpr` or concepts in C++20 — tag dispatch is a C++17-and-earlier technique.

```cpp
struct FastTag {};
struct SafeTag {};

template<typename T>
Void processImpl(T value, FastTag) { /* fast path */ }

template<typename T>
Void processImpl(T value, SafeTag) { /* safe path */ }

template<typename T>
Void process(T value)
{
    using Tag = std::conditional_t<std::is_trivial_v<T>, FastTag, SafeTag>;
    processImpl(value, Tag{});
}
```

### `consteval` and `constinit`

- `consteval` — function must be evaluated at compile time. Use for functions that only make sense at compile time.
- `constinit` — variable is initialized at compile time but remains mutable at runtime. Use on global/`static` variables to prevent the static initialization order fiasco.

```cpp
consteval Int32 squareOf(Int32 n) { return n * n; }

constinit static Int32 instanceCount = 0;  // guaranteed compile-time init, mutable
```

### `std::integral_constant`

Use `std::integral_constant` to pass compile-time values as types in TMP chains.

```cpp
using TrueType  = std::integral_constant<Bool, true>;
using FalseType = std::integral_constant<Bool, false>;
// Or just use the standard aliases:
// std::true_type, std::false_type
```

---

## Move Semantics and Value Categories

### Value Categories

- **lvalue** — has a name or addressable location. You can take its address.
- **rvalue** — temporary or expiring. You cannot take its address.
- **xvalue** — expiring value; result of `std::move`. Has a location but is about to be moved from.
- **prvalue** — pure rvalue; a temporary with no location (e.g. the literal `5`, or a function returning by value).
- **glvalue** — generalized lvalue: lvalue or xvalue.

```cpp
Int32 x = 5;
Int32& lref  = x;              // lvalue reference
Int32&& rref = std::move(x);   // rvalue reference binding to xvalue
Int32&& rref2 = 5;             // rvalue reference binding to prvalue
```

### `std::move`

`std::move` is a cast to `T&&`. The actual move happens in the move constructor or move assignment. After moving from a variable, treat it as indeterminate — do not read it.

```cpp
UniqPtr<Engine> a = makeUniq<Engine>();
UniqPtr<Engine> b = std::move(a);
// 'a' is now null — do not use it
```

> Never `std::move` a local variable in a return statement. It disables NRVO (Named Return Value Optimization) and forces a move where the compiler would have elided the operation entirely.

```cpp
Str buildMessage()
{
    Str result = "hello";
    return result;              // OK — NRVO applies, no copy or move
    return std::move(result);   // BAD — disables NRVO, forces a move instead
}
```

### `std::forward` and Perfect Forwarding

Use `std::forward<T>` inside a template that takes a forwarding reference (`T&&`) to preserve the argument's value category.

```cpp
template<typename T>
Void wrapper(T&& value)
{
    target(std::forward<T>(value));
}
```

Never use `std::forward` on a concrete type. It is only meaningful in a deduced `T&&` context.

```cpp
Void bad(Int32&& value)
{
    target(std::forward<Int32>(value));   // BAD — just use std::move
    target(std::move(value));             // OK
}
```

### Move Constructor and Assignment

Always mark move operations `noexcept`. The standard library checks `noexcept` during reallocation (e.g. `std::vector::push_back`) — if your move constructor can throw, the library will copy instead of move.

```cpp
struct Buffer
{
    Buffer(Buffer&& other) noexcept;
    Buffer& operator=(Buffer&& other) noexcept;
};
```

### Forwarding References vs Rvalue References

They look identical but are not.

```cpp
template<typename T>
Void f(T&& x);        // forwarding reference — T deduced, binds to lvalues and rvalues

Void g(Int32&& x);    // rvalue reference — only binds to rvalues of Int32
```

A forwarding reference only arises when `T` is a deduced template parameter. `const T&&` is not a forwarding reference — it is an rvalue reference.

---

## Object-Oriented Programming

### Methods

Member functions are scoped by their class. No type prefix.

```cpp
struct Vec2
{
    Float32 magnitude() const;
    Void normalize();
    Void scale(Float32 scalar);
};
```

### Getters and Setters

Use `const` on the method itself. Avoid trivial getters/setters on `struct`-style types where public fields are more natural. Use getters when values are derived or class invariants must be maintained.

```cpp
Float32 magnitude() const;   // read-only
Void scale(Float32 scalar);  // mutating
```

### Constructors

Use member initializer lists. Format with each member on its own line, colon on the first, commas leading.

```cpp
struct Vec2
{
    Float32 x;
    Float32 y;

    Vec2(Float32 x, Float32 y)
        : x{ x }
        , y{ y }
    {
    }
};
```

Use `explicit` on all single-argument constructors to prevent silent implicit conversions.

```cpp
struct Radius
{
    explicit Radius(Float32 value);   // OK — prevents Radius r = 3.0f; silently
};
```

### Destructors

Polymorphic base class: `virtual ~Base() = default`.
Non-polymorphic class with resources: define a destructor that releases them.
Non-polymorphic class without resources: do not declare one — let the compiler generate it.

### Rule of Zero / Three / Five

If you define any of the five special members, define all five — or explicitly `= default` / `= delete` each one. This is the single most common source of subtle memory bugs in C++.

```cpp
struct Buffer
{
    Buffer(Size size);
    ~Buffer();
    Buffer(const Buffer& other);
    Buffer& operator=(const Buffer& other);
    Buffer(Buffer&& other) noexcept;
    Buffer& operator=(Buffer&& other) noexcept;
};
```

For non-copyable types, explicitly delete:

```cpp
struct NonCopyable
{
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) noexcept = default;
    NonCopyable& operator=(NonCopyable&&) noexcept = default;
};
```

### Polymorphism

Use `virtual` + `override`. Do not write `virtual` on the overriding function — `override` implies it. Use `final` to prevent further derivation.

```cpp
struct Shape
{
    virtual Void draw() const = 0;
    virtual ~Shape() = default;
};

struct Circle : Shape
{
    Void draw() const override;          // OK — override only
    // virtual Void draw() const override;  // BAD — redundant virtual
};

struct OptimizedCircle final : Shape
{
    Void draw() const override;
};
```

### Abstract Interfaces

Define interfaces as pure-virtual structs with no data members. Keep them narrow — one responsibility per interface.

```cpp
struct IDrawable
{
    virtual Void draw() const = 0;
    virtual ~IDrawable() = default;
};
```

### `[[no_unique_address]]`

Use `[[no_unique_address]]` on empty member types so they consume no storage.

```cpp
struct MyAllocator {};

struct Container
{
    [[no_unique_address]] MyAllocator allocator;   // zero-size if MyAllocator is empty
    Int32* data;
    Size size;
};
```

### Virtual Dispatch in Constructors and Destructors

During construction and destruction, virtual dispatch resolves to the type being constructed/destroyed, not the most-derived type. Never call a virtual function in a constructor or destructor if you expect derived behavior.

```cpp
struct Base
{
    Base() { init(); }          // BAD — calls Base::init(), not Derived::init()
    virtual Void init() { }
};
```

---

## Operator Overloading

Only overload operators when the semantics are genuinely natural and unsurprising for the type.

### Symmetric Binary Operators

Implement as free functions (or `friend`) to allow implicit conversions on both sides. Take the left operand by value.

```cpp
friend Vec2 operator+(Vec2 lhs, const Vec2& rhs) noexcept
{
    lhs += rhs;
    return lhs;
}
```

### Compound Assignment Operators

Member functions returning `*this` by reference.

```cpp
Vec2& operator+=(const Vec2& rhs) noexcept
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}
```

### Comparison Operators

In C++20, define `operator<=>` to auto-generate all six comparison operators.

```cpp
struct Version
{
    Int32 major;
    Int32 minor;

    auto operator<=>(const Version&) const = default;
};
```

For custom ordering, return the appropriate comparison category type.

```cpp
std::strong_ordering operator<=>(const MyType& other) const noexcept
{
    if(auto cmp = a <=> other.a; cmp != 0) return cmp;
    return b <=> other.b;
}
```

### Subscript Operator

Provide both `const` and non-`const` overloads.

```cpp
T& operator[](Size index) noexcept;
const T& operator[](Size index) const noexcept;
```

### Conversion Operators

Mark `explicit` unless the implicit conversion is genuinely safe.

```cpp
explicit operator Bool() const noexcept;   // OK
operator Float32() const noexcept;         // BAD unless this type IS a float
```

### Stream Operators

Always free functions — never member functions.

```cpp
std::ostream& operator<<(std::ostream& os, const Vec2& v)
{
    return os << "Vec2(" << v.x << ", " << v.y << ")";
}
```

---

## Lambdas

### Capture Style

Never use default capture modes (`[=]` or `[&]`) for lambdas that outlive the calling scope. Always capture explicitly.

```cpp
Int32 x = 5;
auto good = [x]() { return x; };          // OK — explicit copy
auto bad  = [=]() { return x; };          // BAD — unclear what is captured

// [&] is tolerable for immediately-passed short-lived lambdas:
std::sort(v.begin(), v.end(), [&](const T& a, const T& b) { return cmp(a, b); });
```

### Lambda Naming

Stored lambdas: `camelCase`, same as variables.

```cpp
auto computeArea = [](Float32 r) { return PI * r * r; };
```

### Generic Lambdas

Use `auto` parameters for generic lambdas (C++14), or explicit template syntax in C++20 when the type name is needed inside the body.

```cpp
auto print = [](auto value) { println("%s", value); };              // C++14

auto printTyped = []<typename T>(T value) { println("%s", value); }; // C++20
```

### `mutable` Lambdas

A lambda's `operator()` is `const` by default — captured-by-value variables cannot be modified. Use `mutable` to allow modification of captured copies.

```cpp
Int32 counter = 0;
auto increment = [counter]() mutable { return ++counter; };
// The outer 'counter' is unchanged — only the captured copy changes
```

### Immediately Invoked Lambdas

Use immediately invoked lambdas to initialize `const` variables that require complex logic.

```cpp
const Str message = [&]()
{
    if(isError) return Str{ "error occurred" };
    return Str{ "all good" };
}();
```

---

## Constexpr and Compile-Time Computation

### `constexpr` Functions

A `constexpr` function is evaluated at compile time when called with constant arguments, and at runtime otherwise. Mark pure computation functions `constexpr` by default.

```cpp
constexpr Int32 factorial(Int32 n)
{
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr Int32 val = factorial(5);   // compile time
Int32 runtimeVal = factorial(n);      // runtime
```

### `consteval`

Forces compile-time evaluation only. The compiler errors if called with a non-constant argument.

```cpp
consteval Int32 squareOf(Int32 n) { return n * n; }

constexpr Int32 a = squareOf(4);  // OK
Int32 b = squareOf(x);            // error — x is not a constant expression
```

### `if constexpr`

Compile-time branching inside templates. The discarded branch is not instantiated.

```cpp
template<typename T>
Void printValue(T value)
{
    if constexpr(std::is_floating_point_v<T>)
    {
        println("%.6f", static_cast<Float64>(value));
    }
    else
    {
        println("%lld", static_cast<Int64>(value));
    }
}
```

### `static_assert`

Always include a descriptive message.

```cpp
template<typename T>
struct Buffer
{
    static_assert(
        std::is_trivially_copyable_v<T>,
        "Buffer<T>: T must be trivially copyable for memcpy correctness"
    );
};
```

---

## Type Deduction

### `auto`

Use `auto` when the type is obvious from the right-hand side, verbose, or irrelevant to the reader. Do not use `auto` when the type conveys important information.

```cpp
auto it = myMap.find(key);             // OK — iterator types are verbose
auto engine = makeUniq<Engine>();      // OK — type is clear from makeUniq
auto x = compute();                    // BAD — what does compute() return?
```

### Return Type Deduction

Prefer explicit return types for public API functions. Use `auto` return deduction only for short internal helpers and lambdas.

```cpp
[[nodiscard]] Float32 magnitude() const;   // OK — explicit, self-documenting
auto magnitude() const { return ...; }     // BAD for public API
```

### `decltype(auto)`

Use `decltype(auto)` as a return type to perfectly forward the value category of a forwarded expression.

```cpp
template<typename Container>
decltype(auto) getFirst(Container&& c)
{
    return std::forward<Container>(c)[0];
}
```

### AAA (Almost Always Auto)

This guide does not mandate AAA style. Explicitness is preferred when it aids readability. `auto` is a tool, not a policy.

---

## Structured Bindings

Use structured bindings to unpack pairs, tuples, structs, and arrays. Prefer them over `std::get<N>`.

```cpp
auto [name, score] = getResult();   // OK

auto result = getResult();
auto name = result.first;           // BAD, verbose
auto score = result.second;         // BAD, verbose
```

For structs, bindings follow declaration order.

```cpp
struct Point { Float32 x; Float32 y; };
auto [px, py] = somePoint;
```

Name bindings descriptively. `[a, b]` is only acceptable for very short, self-evident unpacking.

Bind by reference when the values are large or you need to modify them.

```cpp
auto& [key, value] = *myMap.find(someKey);  // binds by reference
```

---

## Attributes

Use standard C++ attributes. Do not use compiler-specific syntax (`__attribute__((...))`) in non-guarded code — wrap them in macros with standard fallbacks.

```cpp
[[nodiscard]]                      // return value must not be discarded
[[nodiscard("reason")]]            // with reason (C++20)
[[maybe_unused]]                   // suppress unused warnings
[[deprecated]]                     // emits warning on use
[[deprecated("use X instead")]]
[[noreturn]]                       // function never returns
[[likely]]                         // branch is likely taken (C++20)
[[unlikely]]                       // branch is unlikely taken (C++20)
[[fallthrough]]                    // intentional switch fallthrough
```

For attributes without a standard equivalent, wrap in a macro:

```cpp
#ifdef __GNUC__
#define HOT     __attribute__((hot))
#define FLATTEN __attribute__((flatten))
#else
#define HOT
#define FLATTEN
#endif
```

### `[[fallthrough]]` in Switch Statements

Any intentional fallthrough in a `switch` must be marked `[[fallthrough]]`. Unmarked fallthrough is a bug.

```cpp
switch(state)
{
    case STATE_INIT:
        initialize();
        [[fallthrough]];   // intentional
    case STATE_READY:
        run();
        break;
    default:
        break;
}
```

---

## Exception Policy

Decide upfront. Commit. Document it.

1. **Use exceptions**: Standard C++ style. Constructors and operations throw on failure. Mark non-throwing functions `noexcept`. Catch at appropriate boundaries.
2. **Disable exceptions**: Compile with `-fno-exceptions`. Return `Opt<T>`, error codes, or `std::expected<T, E>` (C++23) from fallible functions.

Mixing both creates unmaintainable code.

### `noexcept`

Mark `noexcept` on:
- Move constructors and move assignment operators (always — affects standard library behavior)
- Destructors (they are `noexcept` by default — do not throw from them)
- Swap functions
- Any leaf function provably unable to throw

```cpp
Void swap(Buffer& other) noexcept;
Buffer(Buffer&& other) noexcept;
Buffer& operator=(Buffer&& other) noexcept;
```

> Destructors that throw are UB if the destructor is called during stack unwinding. Never throw from a destructor.

---

## Memory Model and Concurrency

### Atomics

Use `std::atomic<T>` for shared state accessed from multiple threads without a mutex. Do not default to `std::memory_order_seq_cst` everywhere — choose the weakest ordering that is correct.

```cpp
std::atomic<Int32> counter{ 0 };

counter.fetch_add(1, std::memory_order_relaxed);   // no synchronization needed
```

Memory ordering reference:

- `relaxed` — atomicity only, no synchronization. For independent counters.
- `acquire` — prevents loads/stores from moving before the acquire. For consuming a value produced by another thread.
- `release` — prevents loads/stores from moving after the release. For publishing a value to another thread.
- `acq_rel` — both acquire and release. For read-modify-write operations in the middle of a chain.
- `seq_cst` — full sequential consistency. The slowest. Use only when global ordering across all threads is required.

### Mutexes

Wrap with `std::lock_guard` or `std::unique_lock`. Never lock/unlock manually — automatic unlock in the destructor handles exceptions.

```cpp
std::mutex myMutex;

{
    std::lock_guard<std::mutex> lock{ myMutex };
    // critical section
}  // unlocked here automatically
```

Use `std::scoped_lock` for locking multiple mutexes simultaneously without deadlock.

```cpp
std::scoped_lock lock{ mutexA, mutexB };
```

### Thread Safety Documentation

Document the thread safety contract of every class used across threads.

```cpp
// Thread-safe: all public methods are safe to call concurrently.
class EventQueue { };

// Not thread-safe: external synchronization required.
class RingBuffer { };
```

---

## RAII and Resource Management

Destructors fire on scope exit — RAII. The `goto`-based DEFER pattern from the C guide is obsolete.

For resources without existing RAII wrappers, write a minimal wrapper struct.

```cpp
struct FileHandle
{
    explicit FileHandle(const char* path)
        : handle{ std::fopen(path, "rb") }
    {
    }

    ~FileHandle()
    {
        if(handle != nullptr)
        {
            std::fclose(handle);
        }
    }

    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    CFile* handle;
};
```

### Scope Guards

For one-off deferred cleanup not worth a full RAII class, use a scope guard.

```cpp
template<typename F>
struct ScopeGuard
{
    explicit ScopeGuard(F&& fn)
        : fn{ std::forward<F>(fn) }
    {
    }

    ~ScopeGuard() { fn(); }

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

private:
    F fn;
};

template<typename F>
ScopeGuard<F> makeScopeGuard(F&& fn)
{
    return ScopeGuard<F>{ std::forward<F>(fn) };
}

// Usage
auto guard = makeScopeGuard([&]() { cleanupResource(); });
```

---

## Undefined Behavior Awareness

### Signed Integer Overflow

Signed overflow is UB. The compiler assumes it never happens and will optimize accordingly, silently removing overflow checks written with the assumption it wraps.

```cpp
// This check can be silently removed — the compiler assumes no UB
if(x + y < x) { /* overflow */ }   // BAD

// Check before adding
if(x > std::numeric_limits<Int32>::max() - y) { /* overflow */ }   // OK
```

### Strict Aliasing

Accessing an object through a pointer to a different type is UB in almost all cases. The safe exceptions are `char*` and `UInt8*`.

```cpp
Float32 f = 3.14f;
Int32* p = reinterpret_cast<Int32*>(&f);
Int32 bits = *p;   // BAD — strict aliasing violation

// Correct type punning:
Int32 bits;
std::memcpy(&bits, &f, sizeof(f));   // OK

// C++20 preferred:
Int32 bits = std::bit_cast<Int32>(f);   // OK
```

### Use After Move

After `std::move`, the source is in a valid but indeterminate state. Do not read from it.

```cpp
UniqPtr<Engine> a = makeUniq<Engine>();
UniqPtr<Engine> b = std::move(a);
a->tick();   // BAD — a is null
```

### Iterator Invalidation

Never insert or erase from a container while iterating it.

```cpp
for(auto& item : myVector)
{
    myVector.push_back(item);   // BAD — invalidates iterators mid-loop
}
```

### Virtual Dispatch in Constructors and Destructors

Virtual dispatch resolves to the currently-being-constructed type, not the most-derived type. Never call a virtual function in a constructor or destructor expecting derived behavior.

```cpp
struct Base
{
    Base() { init(); }          // BAD — calls Base::init(), not Derived::init()
    virtual Void init() { }
};
```

### Uninitialized Memory

Always initialize variables. In C++, uninitialized variables (especially POD types in a `struct` without a constructor) contain indeterminate values, and reading them is UB.

```cpp
struct Foo
{
    Int32 x;   // indeterminate if Foo is default-constructed without initialization
};

Foo f;
print("%d", f.x);   // BAD — UB, x is uninitialized

struct FooSafe
{
    Int32 x{ 0 };   // OK — explicitly zero-initialized
};
```

---

## Include Conventions

1. Standard library headers (`<cstdint>`, `<memory>`, `<vector>`, etc.)
2. Third-party library headers
3. First-party project headers (`"myproject/thing.hpp"`)
4. Local/sibling headers (`"thing.hpp"`)

Use `.hpp` for C++ headers and `.cpp` for implementation files. Use `.h` only for headers that must be compatible with both C and C++.

---

## Header Guard Conventions

Prefer `#pragma once`. Universally supported by GCC, Clang, and MSVC.

```cpp
#pragma once
```

Fallback if unavailable: `MY_PROJECT_FILE_HPP`.

---

## Macros

Macros are a last resort in C++. `constexpr`, templates, and concepts replace almost everything macros were used for in C. Always parenthesize macro parameters and the full body.

```cpp
#define MAX(a, b) ((a) > (b) ? (a) : (b))
```

Prefer:

```cpp
template<typename T>
constexpr T max(T a, T b) noexcept { return a > b ? a : b; }
```

---

## Meta

Adapted from Jack's C Style Guide.
Updated: March 2026

:)
