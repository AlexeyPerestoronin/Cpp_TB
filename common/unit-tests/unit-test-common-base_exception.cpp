#include <gtest/gtest.h>

#include <common/base_exception.hpp>

#include <nlohmann/json.hpp>

namespace {
    using namespace TB_NS::Error_NS;

    Exceptions& Ins{ Exceptions::GetIns() };

    void F1() {
        Exception::CR excption = Ins.RegistUnknowException("F1()", "F1()");
        throw excption << Location(BOOST_CURRENT_LOCATION);
    }

    void F2() {
        try {
            F1();
        } catch (Exception& error) {
            Exception::CR excption = Ins.RegistUnknowException("F2()", "F2()");
            throw excption << Location(BOOST_CURRENT_LOCATION) << Suberror(error);
        }
    }

    void F3() {
        try {
            F2();
        } catch (Exception& error) {
            Exception::CR excption = Ins.RegistUnknowException("F3()", "F3()");
            throw excption << Location(BOOST_CURRENT_LOCATION) << Suberror(error);
        }
    }

    TEST(BaseException, UnregisterExceptions) {
        try {
            F3();
            EXPECT_TRUE(false) << "unreachable code";
        } catch (Exception& error) {
            std::cout << error.what();
        }
    }

    TEST(BaseException, ParseFromFile) {
        Ins.LoadSettings(TB_GET_CURRENT_PATH / "test_exceptions.json");
        try {
            throw Ins["DB"]["connection"]["incorrect name"] << Location(BOOST_CURRENT_LOCATION);
            EXPECT_TRUE(false) << "unreachable code";
        } catch (Exception& error) {
            std::cout << error.what();
        }
    }
} // namespace