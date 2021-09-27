#include <gtest/gtest.h>
#include <common/curl_adapter.hpp>

#include <cppcoro/generator.hpp>
#include <concepts>

namespace {
    using namespace TB_NS;

    void f(int(&&i_v)[]){};
    void f(int (&i_v)[1]){};

    struct Number {
        int64_t value;

        operator std::string() const& {
            std::cout << "\noperator std::string() const &";
            return std::to_string(value);
        }

        operator std::string() && {
            std::cout << "\noperator std::string() &&";
            return std::to_string(value);
        }
    };

    TEST(Test, _1) {
        Number number{};
        std::string str1 = number;
        std::string str2 = std::move(number);
    }

    TEST(Test, _2) {
        int arr[1];

        f(arr);      // calls `f(int (&)[1])`
        f({ 1, 2 }); // calls `f(int (&&)[])`
        int(&r)[] = arr;
        //f(r);
    }

    TEST(Test, _3) {
        volatile int x{};
        x++; // deprecated
    }

    template<typename T>
    concept ContainerType = requires(T t) {
        T::value_type;
        t.begin();
        t.end();
    };

    template<ContainerType Container>
    cppcoro::generator<std::tuple<size_t, typename Container::value_type&>> enumerator(Container& i_container) {
        for (size_t index{ 0 }; auto& value : i_container)
            co_yield { index++, value };
    }

    template<ContainerType Container>
    cppcoro::generator<std::tuple<size_t, const typename Container::value_type&>> cenumerator(const Container& i_container) {
        for (size_t index{ 0 }; const auto& value : i_container)
            co_yield { index++, value };
    }

    TEST(Test, _4) {
        std::vector<uint64_t> massive{ 0, 1, 2, 3, 4 };

        for (auto [i, element] : enumerator(massive)) {
            ASSERT_EQ(static_cast<uint64_t>(i), element);
            ++element;
        }

        for (auto [i, element] : enumerator(massive)) {
            ASSERT_EQ(static_cast<uint64_t>(i), element - 1);
            ++element;
        }

        for (auto [i, element] : cenumerator(massive)) {
            ASSERT_EQ(static_cast<uint64_t>(i), element - 2);
        }
    }

} // namespace