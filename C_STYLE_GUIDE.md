# Jack's C Style Guides

This is a collection of just general rules and guidelines that I employ for writing C to make it more readable and enjoyable for specifically myself. If you want to use this as a guide, feel free to do so, but please don't take it as gospel, and feel free to modify it to your liking. I also want to note that this is not a comprehensive style guide, and there are many things that I don't cover here, but I will try to cover the most important ones.

> [!WARNING]
> This guide does not serve to teach best practices or how to write C, it assumes that you already know most of the best practices and idioms of C. This document serves only as a guide on how to structure and format your code and write in a custom opinionated way.

## Compiler Setup

- Preferably Standard C17, but C11 is also acceptable.
- Preferably GCC or Clang. *Never MSVC*

### Compiler Flags

This section describes some necessary flags that can be helpful for compiling C code for when you are running either a release build or development build.

#### Release Build

```
-std=c17 -O2 -Wall -Wextra -Wpedantic -Wshadow -Wcast-align \ 
  -flto -D_FORTIFY_SOURCE=2 -fstack-protector-strong -march=native
```

> Additionally here in both release and development environments, `-Wshadow` is there to make sure that local variables are not shadowing any other global or file scope variables. 

#### Development Build

```
-std=c17 -O1 -g -Wall -Wextra -Wpedantic -Wshadow -Wcast-align \
  -fsanitize=address,undefined -fno-omit-frame-pointer
```

> In the presence of a proper address-sanitization system, the `-fno-stack-protector` flag can be omitted, as it is not necessary to disable stack protection when using address sanitization.
>
> Additionally, the following flags can also be added for debugging purposes:
> `-fsanitize=address -fsanitize=undefined -fsanitize-trap=all`

### Optimization Flags

This is based on the requirement of the project, often times `-O2` is a good choice, but using `-O0` is preferred for debug builds. Strictly avoid using `-O3` as it can lead to unexpected behavior and can make debugging difficult especially when linking with libraries that may not be compatible with the aggressive optimizations performed by `-O3` (e.g. `-fpredictive-commoning`, `-funroll-loops` which can break certain code and also increase binary size).

### Linking Libraries Flags

This section describes some necessary flags that can be helpful for linking standard libraries and other dependencies.

#### Linux

1. Math: `-lm`
2. Realtime: `-lrt`
3. OpenMP: `-fopenmp`
4. Dynamic Loading: `-ldl`

> Please note that Math and Realtime libraries are not directly linked by default on Linux.

#### Windows

None.

#### MacOS

None.

## Shared Types

To improve readability, I prefer to use a common set of types, functions, and macros in a single-header library. This library provides a consistent redefined type system that matches the [Coding Style](#coding-style) guidelines outlined in this document. This allows for easier readability and maintainability of code across different projects.

```c
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

#ifndef SHARED_H
#define SHARED_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

typedef int32_t Int32;
typedef int64_t Int64;
typedef uint32_t UInt32;
typedef uint64_t UInt64;
typedef int8_t Int8;
typedef uint8_t UInt8;
typedef char Utf8;
typedef uintptr_t UPtr;
typedef size_t Size;
typedef float Float32;
typedef double Float64;
typedef int16_t Int16;
typedef uint16_t UInt16;
typedef void Void;
typedef FILE CFile;
typedef bool Bool;
typedef Void* Any;

#define CharSeq const Utf8*
#define CharSeq16 const Utf16*
#define CharSeq32 const Utf32*

#if defined(_WIN32) || defined(_WIN64)
    #if defined(build)
        #define api __declspec(dllexport)
    #elif defined(link)
        #define api __declspec(dllimport)
    #else
        #define api
    #endif
#else
    #define api
#endif

#ifdef __cplusplus
    #define linkage extern "C"
#else
    #define linkage extern
#endif

#define expose linkage api

#ifdef __GNUC__
#define pure       __attribute__((pure))
#define hot        __attribute__((hot))
#define flatten    __attribute__((flatten))
#define constfx    __attribute__((const))
#define never      __attribute__((noreturn))
#define deprecated __attribute__((deprecated))
#define unused     __attribute__((unused))
#define packed     __attribute__((packed))
#define never         __attribute__((noreturn))
#else
#define pure
#define hot
#define flatten
#define constfx
#define never
#define deprecated
#define unused
#define packed
#define never
#endif

#define null NULL
#define simple static inline
#define PI 3.14159265358979323846f
#define sizeOf sizeof
#define true ((Bool)1)
#define false ((Bool)0)
#define METHOD(Type) Void Type##_
#define DEFER(label) if (true) { goto label; } else
#define print(...) fprintf(stdout, __VA_ARGS__)
#define println(...) do { print(__VA_ARGS__); print("\n"); } while(0)

simple Bool stringEquals(CharSeq a, CharSeq b)
{
    if(a == b)
    {
        return true;
    }
    if(a == null || b == null)
    {
        return false;
    }
    while(*a && *b)
    {
        if(*a != *b)
        {
            return false;
        }
        a++;
        b++;
    }
    return *a == *b;
}

#ifdef USE_DYNAMIC_LIBRARY_COMPAT_LAYER
#ifdef _WIN32
#include <windows.h>
#define RTLD_LAZY 0

typedef HMODULE DLHandle;

static Int8 dlerrorBuffer[256];

CharSeq dlerror()
{
    return dlerrorBuffer;
}

Any dlopen(CharSeq name, Int32 flags) {
    (Void) flags;
    HMODULE h = LoadLibraryA(name);
    if(!h)
    {
        DWORD err = GetLastError();
        sprintf(dlerrorBuffer, "LoadLibrary failed for %s: error %lu", name, err);
    }
    else
    {
        dlerrorBuffer[0] = '\0';
    }
    return h;
}

Any dlsym(Any handle, CharSeq symbol) {
    Any p = GetProcAddress((HMODULE)handle, symbol);
    if(!p)
    {
        DWORD err = GetLastError();
        sprintf(dlerrorBuffer, "GetProcAddress failed for %s: error %lu", symbol, err);
    }
    else
    {
        dlerrorBuffer[0] = '\0';
    }
    return p;
}

Int32 dlclose(Any handle) {
    if(FreeLibrary((HMODULE) handle))
    {
        dlerrorBuffer[0] = '\0';
        return 0;
    }
    else
    {
        DWORD err = GetLastError();
        sprintf(dlerrorBuffer, "FreeLibrary failed: error %lu", err);
        return -1;
    }
}

#else
#include <dlfcn.h>
typedef Any DLHandle;
#endif
#endif



#endif
```

> Going forward, all documentation will utilize symbols defined in this header for consistency

> There is a new `<stdnoreturn.h>` or through the usage of `_Noreturn` keyword, the `never` macro modifier will be changed in the future to reflect this.

> **Definition of `Int8` for `char`** is an unstable and risky implementation and requires another change if you are using this for ARM rather than on x86. 
> 
> By default, the above `shared.h` is meant for x86 machines where the base `char` type is a signed type. On the other hand, on ARM machines, the `char` type is often unsigned. Additionally, there is a plan to migrate to the following types:
> ```c
> typedef int8_t  Int8;   // Guaranteed signed 8-bit
> typedef uint8_t UInt8;  // Guaranteed unsigned 8-bit
> typedef char    Utf8;   // Specifically for text/strings
> #define CharSeq const Utf8*
> ```

## Naming Conventions

In general, `camelCase` is used for variables, functions, and named values, while `PascalCase` is used for types. While `SCREAMING_SNAKE_CASE` is used for macros and constants/enum members. 

If you need to namespace something, prefer using just a simple prefix following the style of that entity. For example, if you want to namespace a function named `foo` with your project name `bar` you would use `camelCase` to name it `barFoo`.

### Local Variables

Prefer `camelCase`.

```c
Int32 main(Void) 
{
    Int32 myVariable = 0; // OK
    Int32 my_variable = 0; // BAD
    Int32 MyVariable = 0; // BAD
}
```

> This rule also applies to function parameters.

### Global/Static Scope Variables

Prefer `camelCase`.

```c
static Int32 myGlobalVariable = 0; // OK
static Int32 my_global_variable = 0; // BAD
static Int32 MyGlobalVariable = 0; // BAD

Int32 myGlobalVariable2 = 0; // OK
Int32 my_global_variable2 = 0; // BAD
Int32 MyGlobalVariable2 = 0; // BAD
```

> Additionally, do not use Hungarian Notation (i.e. using `g_` prefix to mark global variables)


### Extern Variables

Prefer `camelCase`.

```c
extern Int32 myExternVariable; // OK
extern Int32 my_extern_variable; // BAD
extern Int32 MyExternVariable; // BAD
```

> Additionally, do not use Hungarian Notation (i.e. using `g_` prefix to mark global variables)

### Free Functions

Free functions are just normal functions that are not associated with any type (i.e. not trying to emulate methods in OOP/part of an object system).

Prefer `camelCase`.

```c
Int32 myFunction(Int32 arg) // OK
{
}

Int32 my_function(Int32 arg) // BAD
{
}

Int32 MyFunction(Int32 arg) // BAD
{
}
```

### Types

This rule applies to all identifiers for type names of:
- Structs
- Unions
- Enum (This rule does not apply to enum members)
- Typedefs
- Macro (Prefer `typedef` when possible)

Prefer `PascalCase`.

```c
typedef struct MyStruct // OK
{
    Int32 member;
} MyStruct;

typedef struct my_struct // BAD
{
    Int32 member;
} my_struct;
```

### Enum Members

Prefer `SCREAMING_SNAKE_CASE`. Additionally, the name of the enum member should be prefixed with the name of the enum type where you take the `camelCase` version of the enum type name and convert it to `SCREAMING_SNAKE_CASE` and use that as a prefix for all enum members. This is to avoid name collisions between enum members and also to make it easier to read and understand the code.

```c
typedef enum OperatingSystem
{
    OPERATING_SYSTEM_WINDOWS, // See here that the 'OPERATING_SYSTEM_' prefix is used for all enum members to align with the enum type name 'OperatingSystem'
    OPERATING_SYSTEM_LINUX,
    OPERATING_SYSTEM_MACOS,
    ...
} OperatingSystem;
```

### Function Like Macros

Prefer `SCREAMING_SNAKE_CASE`.

```c
#define MAX(a, b) ((a) > (b) ? (a) : (b)) // OK
#define max(a, b) ((a) > (b) ? (a) : (b)) // BAD
#define Max(a, b) ((a) > (b) ? (a) : (b)) // BAD
#define find_max(a, b) ((a) > (b) ? (a) : (b)) // BAD
```

### Type Like Macros

Prefer `PascalCase`.

```c
#define MyType Int32 // OK
#define my_type Int32 // BAD
```

> If it is possible, prefer to use `typedef` instead of `#define` for type-like macros to have better type safety and better debugging.

### Function Pointers (`typedef`)

Prefer `PascalCase`

```c
typedef Int32 (*Callback)(Int32 arg); // OK
typedef Int32 (*cb)(Int32 arg); // BAD - Although 'Cb' is fine, but it is not readable
```

## Formatting

This section describes how to structure your code in terms of indentation, braces placement, and other formatting rules.

### Indentation

There is no exact rule, prefer to use whatever your editor supports.

### Macros Indentation

Do not use identation for nested macros. As moving them around is very confusing as they are not directly related to the C language itself.

```c
#ifdef SOME_FLAG
#include "somefile.h"
#define OK 1
#else
#include "anotherfile.h"
#endif // OK

#ifdef SOME_FLAG
#   include "somefile.h"
#   define OK 1
#else
#   include "anotherfile.h"
#endif // BAD - this can make long nested macros seem like 'include', 'define' are normal C keywords or some other macros.

#ifdef SOME_FLAG
    #include "somefile.h"
    #define OK 1
#else
    #include "anotherfile.h"
#endif // BAD
```

### Braces Placement 

Prefer the Allman style for braces placement. This means that the opening brace is placed on a new line, aligned with the control statement or function declaration, and the closing brace is also placed on a new line, aligned with the opening brace.

```c
Int32 main(Void)
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

In the case where lets say the body is empty, there should be no spacing between the braces and they must be on separate lines.

```c
Int32 main(Void)
{
} // OK

Int32 main(Void) 
{} // BAD
```

The same goes for structs, place the opening brace on a new line and the closing brace on a new line.

```c
typedef struct MyStruct
{
    Int32 member;
} MyStruct;
```

> Note that the closing brace is on the same line as the type name.
>
> Enums should follow the same style as structs.

### Spacing in Expressions

Spacing within expressions should be maximized around binary operators while unary operators and parentheses do not require this.

```c
Int32 someNumber = a + ~c * (ab / -c); // OK
Int32 anotherNumber=a+~c*(ab/c); // BAD
```

### Spacing in Compound Rvalue Literals

When using compound literals or casted r‑value initializers, always include a space between the closing parenthesis of the cast and the opening brace of the literal. This improves readability and visually separates the cast from the initializer body.

```c
SomeType x = (SomeType) { ... }; // OK
SomeType x = (SomeType){ ... }; // BAD
SomeType x = (SomeType){...}; // BAD
```

### Spacing in Control Structures

1. All control structures should not have a space between the opening keyword and the opening parenthesis

```c
if(...) // OK
if (...) // BAD

for(...) // OK
for (...) // BAD

else if(...) // OK
else if (...) // BAD
```

2. In a `do-while` loop, the `while` must rest on the same line as the closing brace.

## Declaration Styles

### Variables

Avoid single line multi-declarations as in `Int8 a, b, c;`. Use a new line for each declaration to maximize clarity even if they are the same type.

### Pointers

The placement of the `*` can be confusing as it is not related to the type in C and is instead related to the variable name. This means
it is confusing that: `Int32* a, b, c;` is not the same as `Int32 *a, *b, *c;`. In order to mitigate this, refer to the fact that multiple variables should
not be declared in a single-line fashion.

Prefer to specify the `*` on the identifier itself:

```c
Int32* a; // GOOD
Int32 *b; // NOT PREFERRED
```

### Constants

With the aforementioned declaration style of variables and pointers, the placement of `const` is also trivial:

1. Pointer to Constant: `const <TypeName>* <variableName>`
2. Constant Pointer: `<TypeName>* const <variableName>`

### Function Declarations

Nothing changed, follow all other rules for naming and formatting.

### Arrays

- Avoid VLAs and prefer fixed-size arrays or dynamic allocation

```c
Int32 array[10]; // OK
Int32 array[] = (Int32[]) { 1, 3, 4 }; //OK 
Int32 array[n]; // BAD
```

### Structs

Prefer to declare structs in the following format:

```c
typedef struct <StructTag> 
{
    <MemberType> <memberName>;
    ...
} <TypeName>;
```

Here `<StructTag>` is not optional and it must match the `<TypeName>`. This is to allow for forward declarations and also to make it easier to read and understand the code.

> Note: It is not acceptable to use `struct` as a tagged-type directly without a typedef, always use `typedef` for structs to avoid having to write `struct` keyword everywhere and also to allow for a more consistent style.

### Enums

Prefer to declare enums in the following format:

```c
typedef enum <EnumTag>
{
    <ENUM_MEMBER_1>,
    <ENUM_MEMBER_2>,
    ...
} <TypeName>;
```

Here `<EnumTag>` is not optional and it must match the `<TypeName>`. This is to allow for forward declarations and also to make it easier to read and understand the code.

> Note: It is not acceptable to use `enum` as a tagged-type directly without a typedef, always use `typedef` for enums to avoid having to write `enum` keyword everywhere and also to allow for a more consistent style.

### Struct Initialization

- Preference to use member style initialization is optional, but strongly preferred to for structs that have more than $2$ members.
- Prefer to always specify the type of the rvalue compound literal (i.e. `MyStruct x = (MyStruct) { ... };`). This allows for long embedded structs to be easily readable.

### Function Definitions 

- Within a header file, defining functions should not require the `extern` keyword.
- Whenever possible, unless to create an `stb` style library, separate implementation into `.c` files.
- For simple definitions that preferably should live in the header, prefer to use the `simple` modifier macro.
- For a function definition with 2 or less parameters, it is not necessary to specify their names.
- For a function definition with no parameters (e.g. `main`), prefer to specify with `Void` (i.e. `main(Void)`) to be explicit.
- **Always declare a return type.** Some older versions of C will default to `int`/`Int32` as the base type. If it returns nothing use `Void` and if it never returns, prepend `never`. 

### Builtin Values

There are some builtin values such as `null`, `false`, and `true` which are macros that wrap their respective standard C equivalent. 

Prefer to use these over `NULL`, `0`/`FALSE`, `1`/`TRUE` respectively.

### Macros

- Always parenthesize macro parameters and the entire macro body: `#define MAX(a,b) ((a) > (b) ? (a) : (b))`

## Implicit Integer to Bool Conversions for Pointers

C allows for treating things like `((Void*)0)` as simply just `false` which means `if(someVar)` and `if(someVar != null)` will produce the same meaning. This is confusing at times, even with C not having native nullability. 

Always compare pointers directly if they are `null`:

```c
Int32* myVar = ...;
if(myVar != null) // OK
{
    ... 
}

if(myVar) // BAD
{
    ...
}
```

## Embrace `goto`

Although generally frowned upon, learn to manage it where necessary in order to avoid forgetting memory cleanup (i.e. simulating the `defer` keyword in languages like Go).

## Defer Macro Helper

The macro `DEFER(label)` simulates RAII/defer cleanup bahavior using `goto` internally.

```c
Void* data = malloc(n);
if(data == null)
{
  return;
}
DEFER(cleanup);
// work …
cleanup:
free(data);
data = null;
```

## Include Conventions

1. Standard & System libraries first using `<>` if preferable
2. First party headers (that the project uses) using `""`
3. Project headers using `""`
4. Miscenallous Non-C headers using `""`

## Header Guard Conventions

Prefer to use old-fashioned include guards over `#pragma once` even if supported:

```c
#ifdef MY_FILE_H
#define MY_FILE_H
//...
#endif
```

Here the naming of the macro `MY_FILE_H` should follow that you can append a prefix for your project or not followed by the actual file name in all caps and then the file extension (preferably `.h`).

## Object-Oriented Programming With Composition

OOP is fun and there is a decent way to emulate it with C through pure composition.

> **Why not polymorphism?** 
>
> C does not provide native polymorphism. While it is technically possible to emulate polymorphic behavior using struct embedding and function pointers, this pattern is verbose, fragile, and visually unclean. It should be avoided unless absolutely necessary (e.g., implementing a framework or plugin system).
> ```c
> typedef struct Shape
> {
>     Void (*draw)(struct Shape* self); // using struct tag here is very ugly and voids a stylistic rule.
> } Shape; 
> ```
>
> UB is foul here, for example to initialize the function pointer for a simple struct can lead to UB:
> ```c
> typedef struct Shape 
> {
>     Void (*draw)(struct Shape* self); 
> } Shape;
> typedef struct Circle 
> {
>     Shape base;
>     Float32 radius;
> } Circle;
> 
> Void circleDraw(Shape* s)
> {
>     Circle* self = (Circle*) s; // assumes s is a circle, even with proper naming (described below), it is not checked by the compiler 
>     self->radius += 1.0f; // UB if 's' is not a Circle
> }
> 
> Int32 main(Void) 
> {
>     Shape s = { .draw = circleDraw };
>     s.draw(&s); // UB
> }
> ```

### Methods

1. All methods/object functions must be named using `camelCase` and be prefixed with the struct's type name followed by in `PascalCase` the name of the method.

```c
typedef struct Vec2 { ... } Vec2;

Void vec2Add(Vec2* a, Vec* b, Vec* sum); // OK
Void addVec2(Vec2* a, Vec* b, Vec* sum); // BAD
```

> This is to allow for fast lookup within something like intellisense

2. All methods/object functions must accept a pointer as the first parameter. You can name this parameter `self`, but it is not mandatory.

```c
typedef struct Vec2 { ... } Vec2;

Void vec2ToString(Vec2*, Int8*); // OK
Void vec2ToString(Int8*, Vec2*); // BAD
```

> This is to make sure that all of the calls are consistent to signify that the first element is the 'self' reference.

### Getters and Setters

Although there is no direct getter and setter pattern, you can enforce a promise to the programmer by applying `const` to the first reference parameter. 

```c
Float32 vec2Magnitude(const Vec2* self); // promising that this method is readonly
Void vec2Scale(Vec2* self, Float32 scalar); // says that this method will modify the reference
```

### Constructors & Destructors

Constructors and destructors must follow naming schemes that allow them to be easily distinguished:

1. All constructor functions must be prefixed with `new` followed by the `PascalCase` struct type name. They should preferably return a pointer to the newly allocated object. 

```c
typedef struct Vec2 { ... } Vec2;

Vec2* newVec2(Float32, Float32); // OK
Vec2* createVec2(Float32, Float32); // BAD
Vec2* vec2(Float32, Float32); // BAD - Ill adivsed as there could be a macro that does the same thing for something else
```

2. All destructor functions must be prefixed with `delete` followed by the `PascalCase` struct type name. They should preferably return `Void`.

```c
typedef struct Vec2 { ... } Vec2;

Void deleteVec2(Vec2*); // OK
Void freeVec2(Vec2*); // BAD - Ill advised.
```

> Delete was chosen as it sounds better than free and looks similar to C++.

> **Make sure to also employ defensive measures against dangling pointers after calling the destructor by setting the original pointer to `null`.** (i.e. immediately after deleting, call `ptr = null;`) 

### Ownership

All memory allocation done by constructors must be handled by the caller respectively with the appropriate destructor call.


## Meta

Updated: February 17, 2026

:)
