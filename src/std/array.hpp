#pragma once
//#include <iterator> // For std::forward_iterator_tag
#include <stddef.h>
#include <numeric>

template <typename T, size_t SIZE>
class array {
    using iterator_category = std::forward_iterator_tag;
    using pointer           = T*;
    using reference         = T&;
    using const_reference = const T&;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
public:
    class iterator: public std::iterator<
                        iterator_category,   
                        value_type,                      
                        difference_type,                     
                        pointer,               
                        reference                       
                                      >{
        pointer m_ptr;
    public:
        constexpr explicit iterator(pointer ptr) : m_ptr(ptr) {}
        constexpr iterator& operator++() { m_ptr++; return *this;}
        constexpr iterator operator++(int) {iterator retval = *this; ++(*this); return retval;}
        constexpr friend bool operator== (const iterator& a, const iterator& b) { return a.m_ptr == b.m_ptr; };
        constexpr friend bool operator!= (const iterator& a, const iterator& b) { return a.m_ptr != b.m_ptr; }; 
        constexpr reference operator*()  {return *m_ptr;}
        constexpr pointer operator->()  { return m_ptr; }
    };

    class const_iterator: public std::iterator<
                        iterator_category,   
                        value_type,                      
                        difference_type,                     
                        const pointer,               
                        reference                    
                                      >{
        T const* m_ptr;
    public:
        constexpr explicit const_iterator(T const* ptr) : m_ptr(ptr) {}
        constexpr const_iterator& operator++() { m_ptr++; return *this;}
        constexpr const_iterator operator++(int) {const_iterator retval = *this; ++(*this); return retval;}
        constexpr friend bool operator== (const const_iterator& a, const const_iterator& b) { return a.m_ptr == b.m_ptr; };
        constexpr friend bool operator!= (const const_iterator& a, const const_iterator& b) { return a.m_ptr != b.m_ptr; }; 
        constexpr T const& operator*() const {return *m_ptr;}
        constexpr T const* operator->() const { return m_ptr; }
    };

    template<typename Itr>
class reverse_iterator {
    Itr itr;
public:
    constexpr explicit reverse_iterator(Itr itr): itr(itr) {}
    constexpr auto& operator*() {
        return *std::prev(itr);
    }
    constexpr auto& operator++() {
        --itr;
        return *this;
    }
    constexpr friend bool operator!=(reverse_iterator<Itr> a, reverse_iterator<Itr> b) {
        return a.itr != b.itr;
    }
};

    constexpr iterator begin() noexcept {
        return iterator{ &m_array[0] };
    }

    constexpr iterator end() noexcept {
        return iterator{ &m_array[SIZE] };
    }

    constexpr const_iterator begin() const noexcept {
        return const_iterator{ &m_array[0] };
    }

    constexpr const_iterator end() const noexcept {
        return const_iterator{ &m_array[SIZE] };
    }

    constexpr const_iterator cbegin() const noexcept {
        return const_iterator{ &m_array[0] };
    }

    constexpr const_iterator cend() const noexcept {
        return const_iterator{ &m_array[SIZE] };
    }

    constexpr auto rbegin() noexcept {
        return reverse_iterator{ m_array + SIZE };
    }

    constexpr auto rend() noexcept {
        return reverse_iterator{ m_array };
    }

    constexpr auto rbegin() const noexcept {
        return reverse_iterator{ m_array + SIZE };
    }

    constexpr auto rend() const noexcept {
        return reverse_iterator{ m_array };
    }

    [[nodiscard]] constexpr bool empty() const noexcept;
    constexpr size_type size() const noexcept { return SIZE; }
    constexpr size_type max_size() const noexcept;

    constexpr reference       operator[](size_type n) { return m_array[n]; }
    constexpr const_reference operator[](size_type n) const { return m_array[n]; }

    template <size_type n>
    constexpr reference at() {
        if constexpr (n >= SIZE) {
            char buffer[150]{"array::at: __n (which is %u) >= _Nm (which is %u)"};
            snprintf(buffer, 149, buffer, n, SIZE); 
            throw std::out_of_range(buffer);
        }
        return m_array[n];
    }
    template <size_type n>
    constexpr const_reference at() const {
        static_assert (n < SIZE, "array: index out of bounds");
        return m_array[n];
    }
    constexpr reference       front() { return m_array[0]; }
    constexpr const_reference front() const { return m_array[0]; }
    constexpr reference       back() { return m_array[SIZE - 1]; }
    constexpr const_reference back() const { return m_array[SIZE - 1]; }
 
    constexpr T *       data() noexcept;
    constexpr const T * data() const noexcept;

    T m_array[SIZE];    
};
template<class T, class... U>
    array(T, U...) -> array<T, 1 + sizeof...(U)>;

