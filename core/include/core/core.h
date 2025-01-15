#pragma once

namespace core
{
    using i8 = char;
    using i16 = short;
    using i32 = int;
    using i64 = long long;
    using u8 = unsigned char;
    using u16 = unsigned short;
    using u32 = unsigned int;
    using u64 = unsigned long long;

    static_assert(sizeof(i8) == 1, "sizeof(i8) != 1");
    static_assert(sizeof(i16) == 2, "sizeof(i16) != 2");
    static_assert(sizeof(i32) == 4, "sizeof(i32) != 4");
    static_assert(sizeof(i64) == 8, "sizeof(i64) != 8");
    static_assert(sizeof(u8) == 1, "sizeof(u8) != 1");
    static_assert(sizeof(u16) == 2, "sizeof(u16) != 2");
    static_assert(sizeof(u32) == 4, "sizeof(u32) != 4");
    static_assert(sizeof(u64) == 8, "sizeof(u64) != 8");
}
