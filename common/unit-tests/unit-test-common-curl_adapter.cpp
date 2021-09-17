#include <gtest/gtest.h>
#include <common/curl_adapter.hpp>

using namespace TB_NS;

TEST(CurlAdapter, Simple1) {
    MakeCurlEasy();
    EXPECT_TRUE(true);
}