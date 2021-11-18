#include <gtest/gtest.h>
#include <common/stdafx.hpp>
#include <common/aliases.hpp>

namespace {
    using namespace TB_NS;
    TEST(Testing_AliasFor, Test1_Int) {
        AliasFor<int> i{};
        ASSERT_EQ(0, i);
        ASSERT_EQ(0, i++);
        ASSERT_EQ(1, i);
        ASSERT_EQ(1, i++);
        ASSERT_EQ(2, i);

        ASSERT_EQ(2, i--);
        ASSERT_EQ(1, i--);
        ASSERT_EQ(0, i);

        ASSERT_EQ(5, i + 5);
        ASSERT_EQ(5, i += 5);
        ASSERT_EQ(10, i += 5);
        ASSERT_EQ(50, i * 5);
        ASSERT_EQ(50, i *= 5);
        ASSERT_EQ(50, i--);
        ASSERT_EQ(48, --i);
    }

    TEST(Testing_AliasFor, Test1_Double) {
        AliasFor<double> i{};
        ASSERT_EQ(0, i);
        ASSERT_EQ(0, i++);
        ASSERT_EQ(1, i);
        ASSERT_EQ(1, i++);
        ASSERT_EQ(2, i);

        ASSERT_EQ(2, i--);
        ASSERT_EQ(1, i--);
        ASSERT_EQ(0, i);

        ASSERT_EQ(5, i + 5.);
        ASSERT_EQ(5, i += 5);
        ASSERT_EQ(10, i += 5);
        ASSERT_EQ(50, i * 5.);
        ASSERT_EQ(50, i *= 5);
        ASSERT_EQ(50, i--);
        ASSERT_EQ(48, --i);
    }

    TEST(Testing_AliasFor, Test1_Map) {
        using DoubltToString = std::map<double, std::string>;
        AliasFor<DoubltToString> aliase{};
        ASSERT_EQ(aliase.size(), 0);
        aliase.emplace(4., "4");
        ASSERT_EQ(aliase.size(), 1);
        ASSERT_EQ(aliase[4], "4");
        aliase = DoubltToString{};
        ASSERT_EQ(aliase.size(), 0);
        aliase = DoubltToString{ { 3., "3" } };
        ASSERT_EQ(aliase.size(), 1);
    }
} // namespace