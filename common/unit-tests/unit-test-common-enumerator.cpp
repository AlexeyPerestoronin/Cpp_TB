#include <gtest/gtest.h>

#include <common/enumerator.hpp>

#include <algorithm>
#include <ranges>
#include <array>

namespace {
    using namespace TB_NS;

    TEST(Enumerator, PositiveTests) {
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