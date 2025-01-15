#include <core/core.h>

#include <stdlib.h>

void* myalloc(core::sz size)
{
    return malloc(static_cast<size_t>(size));
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

    return 0;
}
