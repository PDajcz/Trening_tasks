#include <iostream>
#include <array>
#include "utest.h"

UTEST_STATE();


template<typename Tp, std::size_t Nm>
struct array_traits {
    typedef Tp Type[Nm];
    typedef std::is_swappable<Tp> is_swappable;
    typedef std::is_nothrow_swappable<Tp> is_nothrow_swappable;

    static constexpr Tp &
    S_ref(const Type &t, std::size_t n) noexcept { return const_cast<Tp &>(t[n]); }

    static constexpr Tp *
    S_ptr(const Type &t) noexcept { return const_cast<Tp *>(t); }
};

template<class T, std::size_t N>
struct array_PD {
    using value_type = T;
    static constexpr std::size_t size_type = N;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = value_type *;
    using const_iterator = const value_type *;
//    T data[N];
    typedef std::ptrdiff_t difference_type;


    typedef array_traits<T, N> AT_Type;
    typename AT_Type::Type elements;


    pointer data() noexcept {
        return AT_Type::S_ptr(elements);
    }

    const_pointer data() const noexcept {
        return AT_Type::S_ptr(elements);
    }

    iterator begin() noexcept {
        return iterator(data());
    }

    const_iterator begin() const noexcept {
        return const_iterator(data());
    }

    void fill(const value_type &u) {
        std::fill_n(begin(), size(), u);
    }

    void swap(array_PD &other)
    noexcept(AT_Type::is_nothrow_swappable::value) { std::swap_ranges(begin(), end(), other.begin()); }


    iterator
    end() noexcept {
        return iterator(data() + N);
    }

    const_iterator
    end() const noexcept {
        return const_iterator(data() + N);
    }

    // Capacity.
    constexpr std::size_t
    size() const noexcept { return N; }

    constexpr std::size_t
    max_size() const noexcept { return N; }

    constexpr bool
    empty() const noexcept { return size() == 0; }

    reference back() noexcept {
        return N ? *(end() - 1) : *end();
    }

    constexpr const_reference back() const noexcept {
        return N ? AT_Type::S_ref(elements, N - 1)
                 : AT_Type::S_ref(elements, 0);
    }

    reference at(std::size_t n) {
        if (n <= N) {
            return AT_Type::S_ref(elements, n);
        }
        //error
        return AT_Type::S_ref(elements, 0);
    }

    constexpr const_reference at(size_t n) const {
        if (n < N) {
            return AT_Type::S_ref(elements, n);
        }
        //error
        return AT_Type::S_ref(elements, 0);
    }

    reference front() noexcept { return *begin(); }

    constexpr const_reference front() const noexcept {
        return AT_Type::S_ref(elements, 0);
    }

    ///////////////
    // Element access.
    _GLIBCXX17_CONSTEXPR reference
    operator[](std::size_t n) noexcept {
        __glibcxx_requires_subscript(n);
        return AT_Type::S_ref(elements, n);
    }

    constexpr const_reference
    operator[](std::size_t n) const noexcept {
#if __cplusplus >= 201402L
        __glibcxx_requires_subscript(n);
#endif
        return AT_Type::S_ref(elements, n);
    }
    ///////////
};


int main(int argc, const char *const argv[]) {
    // do your own thing
    return utest_main(argc, argv);
}

#define size_arrays 3
UTEST(array_test, test) {
    ASSERT_EQ(2, 2);
    std::array<int, size_arrays> a1{1, 2, 3};
    array_PD<int, size_arrays> b1{1, 2, 3};
    for (int i = 0; i < size_arrays; ++i) {
        ASSERT_EQ(b1.at(i), a1.at(i));
    }
    auto siz = b1.size();
    auto emp = b1.empty();
    ASSERT_EQ(2, 2);

}


UTEST(array_test, testfloat) {
    ASSERT_EQ(2, 2);
    std::array<float, size_arrays> a1{1.1, 2.1, 3.1};
    array_PD<float, size_arrays> b1{1.1, 2.1, 3.1};
    for (int i = 0; i < size_arrays; ++i) {
        ASSERT_EQ(b1.at(i), a1.at(i));
    }
    ASSERT_EQ(a1.size(),b1.size());
    ASSERT_FALSE(a1.empty());
    ASSERT_FALSE(b1.empty());

}

UTEST(array_test, testBool) {
    ASSERT_EQ(2, 2);
    std::array<bool, size_arrays> a1{true, false, true};
    array_PD<bool, size_arrays> b1{false, true, false};
    for (int i = 0; i < size_arrays; ++i) {
        ASSERT_NE(b1.at(i), a1.at(i));
    }
    ASSERT_EQ(a1.size(),b1.size());
    ASSERT_FALSE(a1.empty());
    ASSERT_FALSE(b1.empty());

}

UTEST(array_test, testEmpty) {
    ASSERT_EQ(2, 2);
    std::array<bool, 0> a1{};
    array_PD<bool, 0> b1{};

    ASSERT_EQ(a1.size(),b1.size());
    ASSERT_TRUE(a1.empty());
    ASSERT_TRUE(b1.empty());

}
