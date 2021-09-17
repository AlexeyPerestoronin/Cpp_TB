#include <gtest/gtest.h>

#include <common/base_exception.hpp>

#include <boost/throw_exception.hpp>

using namespace TB_NS;

namespace {
    void F1() {
        throw Exception() << Description("message: F1(...)") << Location(BOOST_CURRENT_LOCATION);
    }

    void F2() {
        try {
            F1();
        } catch (Exception& error) {
            throw Exception() << Description("message: F2(...)") << Location(BOOST_CURRENT_LOCATION) << Suberror(error);
        }
    }

    void F3() {
        try {
            F2();
        } catch (Exception& error) {
            throw Exception() << Description("message: F3(...)") << Location(BOOST_CURRENT_LOCATION) << Suberror(error);
        }
    }

    TEST(BaseException, Simple1) {
        try {
            F3();
            EXPECT_TRUE(false) << "unreachable code";
        } catch (Exception& error) {
            std::cout << error.what();
        }
    }
} // namespace