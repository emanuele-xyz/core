#pragma once

#define core_sizeof(a) static_cast<::core::sz>(sizeof(a))
#define core_alignof(t) static_cast<::core::sz>(alignof(t))

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

    template<typename T> struct remove_reference { typedef T type; };
    template<typename T> struct remove_reference<T&> { typedef T type; };
    template<typename T> struct remove_reference<T&&> { typedef T type; };
    template<typename T> using remove_reference_t = typename remove_reference<T>::type;
    template<typename T>
    constexpr remove_reference_t<T>&& move(T&& t) noexcept
    {
        return static_cast<typename remove_reference<T>::type&&>(t);
    }

    // Clamp val between low and high
    template <typename T>
    T clamp(T val, T lo, T hi)
    {
        return val < lo ? lo : val > hi ? hi : val;
    }

    // View on a contiguous chunk of memory
    class mem_view
    {
    public:
        inline mem_view(void* start, void* end) : m_start{ start }, m_end{ end } { /*TODO: assert for start <= end*/ }
    public:
        inline void* start() const noexcept { return m_start; }
        inline void* end() const noexcept { return m_end; }
        inline sz size() const noexcept { return static_cast<ptr>(m_end) - static_cast<ptr>(m_start); }
        inline bool is_empty() const noexcept { return size() == 0; }
        inline mem_view align(sz align, sz elem_size) const noexcept
        {
            ptr const start{ static_cast<ptr>(m_start) };
            ptr const end{ static_cast<ptr>(m_end) };
            sz const lpad{ (start - static_cast<ptr>(nullptr)) % align };
            ptr const aligned_start{ clamp(start + lpad, start, end) };
            sz const rpad{ (end - aligned_start) % elem_size };
            ptr const aligned_end{ clamp(end - rpad, aligned_start, end) };
            return { aligned_start, aligned_end };
        }
        template <typename T>
        inline mem_view align() const noexcept { return align(core_alignof(T), core_sizeof(T)); }
    private:
        void* m_start;
        void* m_end;
    };

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

    // Owning handle to a contiguous chunk of memory
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
        inline mem_view view() const noexcept { return { m_bytes, static_cast<ptr>(m_bytes) + m_size }; }
    private:
        sz m_size;
        alloc_strat m_alloc_strat;
        void* m_bytes;
    };

    // Owning handle to an array of instances of type T, instantiated over a contiguous chunk of memory
    template <typename T>
    class inst
    {
    public:
        inst(mem_view view) : m_view{ view }, m_instances{}
        {
            /*TODO: assert that the size of the raw view is a multiple of T*/ // (size % t_size) == 0
            /*TODO: align*/
        }
    private:
        mem_view m_view;
        T* m_instances;
    };

    // View on a contiguous chunk of memory, interpreted as an array of Ts
    template <typename T>
    class view
    {
    public:
        inline view(mem_view mview) : m_start{ static_cast<T*>(mview.start()) }, m_end{ static_cast<T*>(mview.end()) }
        {
            /*TODO: assert that the size of the raw view is a multiple of T*/
        }
        inline view(void* start, void* end) : view{ {start, end} }
        {
            /*TODO: assert that the size of the raw view is a multiple of T*/
        }
    public:
        T& operator[](sz i) { /*TODO: range check*/ return m_start[i]; }
        const T& operator[](sz i) const { /*TODO: range check*/ return m_start[i]; }
    public:
        inline T* start() const noexcept { return m_start; }
        inline T* end() const noexcept { return m_end; }
        inline sz size() const noexcept { return static_cast<ptr>(static_cast<void*>(m_end)) - static_cast<ptr>(static_cast<void*>(m_start)); }
        inline sz count() const noexcept { return m_end - m_start; }
    private:
        T* m_start;
        T* m_end;
    };
}
