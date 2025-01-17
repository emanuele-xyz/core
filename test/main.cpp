#include <core/core.h>

#include <stdlib.h>

void* myalloc(core::sz size)
{
    return calloc(1, static_cast<size_t>(size));
}

void myfree(void* p)
{
    return free(p);
}

int main(void)
{
    core::alloc_strat alloc_strat{ .alloc = myalloc, .free = myfree };
    core::mem mem0{ core_sizeof(core::i32) * 5, alloc_strat };
    core::mem mem1{ core::move(mem0) };
    core::mem_view v0{ mem1.view() };
    core::view<core::i32> i32s{ v0 };
    core::sz i32s_count{ i32s.count() };
    core::sz i32s_size{ i32s.size() };
    core::view<float> floats{ v0.start(), v0.end() };
    core::sz floats_count{ floats.count() };
    core::sz floats_size{ floats.size() };
    i32s[0] = 0;
    i32s[1] = 1;
    i32s[2] = 2;
    i32s[3] = 3;
    i32s[4] = 4;
    floats[0] = 0.0f;
    floats[1] = 1.0f;
    floats[2] = 2.0f;
    floats[3] = 3.0f;
    floats[4] = 4.0f;

    return 0;
}
