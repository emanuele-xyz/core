#include <core/core.h>

namespace core
{
    mem::mem(sz size, alloc_strat alloc_strat)
        : m_size{ size }
        , m_alloc_strat{ alloc_strat }
        , m_bytes{}
    {
        // TODO: if alloc_strat is {} then use system default
        // TODO: check for size to be > 0
        // TODO: check for alloc_strat.alloc not null
        m_bytes = m_alloc_strat.alloc(size);
    }
    mem::~mem() noexcept
    {
        if (m_alloc_strat.free && m_bytes)
        {
            m_alloc_strat.free(m_bytes);
        }
    }
    mem::mem(mem&& other) noexcept
        : m_size{}
        , m_alloc_strat{}
        , m_bytes{}
    {
        *this = move(other);
    }
    mem& mem::operator=(mem&& other) noexcept
    {
        if (this != &other)
        {
            this->~mem();

            m_size = other.m_size;
            m_alloc_strat = other.m_alloc_strat;
            m_bytes = other.m_bytes;

            other.m_size = 0;
            other.m_alloc_strat = {};
            other.m_bytes = nullptr;
        }
        return *this;
    }
}
