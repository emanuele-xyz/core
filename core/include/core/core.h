#pragma once

#define core_sizeof(a) static_cast<::core::sz>(sizeof(a))

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

    // Signed size type
    using sz = decltype(static_cast<int*>(nullptr) - static_cast<int*>(nullptr));

    // Generic pointer type
    using ptr = char*;

    template<class T> struct remove_reference { typedef T type; };
    template<class T> struct remove_reference<T&> { typedef T type; };
    template<class T> struct remove_reference<T&&> { typedef T type; };
    template<class T> using remove_reference_t = typename remove_reference<T>::type;
    template< class T >
    constexpr remove_reference_t<T>&& move(T&& t) noexcept
    {
        return static_cast<typename remove_reference<T>::type&&>(t);
    }

    // Allocation function pointer
    using allocfn = void* (*)(sz);
    // Deallocation function pointer
    using freefn = void (*)(void*);

    // Allocation strategy
    struct alloc_strat
    {
        allocfn alloc; // Allocation function (required)
        freefn free; // Deallocation function (optional)
    };

    // View on a sequence of elements
    template <class T>
    class view
    {
    public:
        view(T* start, T* end) noexcept : start{ start }, end{ end } {}
    private:
        T* start;
        T* end;
    };

    // View on a raw chunk of memory
    using mem_view = view<void>;

    // Owning handle to a chunk of memory
    class mem
    {
    public:
        mem(sz size, alloc_strat alloc_strat = {});
        ~mem() noexcept;
        mem(const mem&) = delete;
        mem(mem&&) noexcept;
        mem& operator=(const mem&) = delete;
        mem& operator=(mem&&) noexcept;
    public:
        mem_view view() const noexcept { return { m_bytes, static_cast<ptr>(m_bytes) + m_size }; }
    private:
        sz m_size;
        alloc_strat m_alloc_strat;
        void* m_bytes;
    };
}
