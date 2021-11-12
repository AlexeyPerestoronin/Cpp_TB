#include <gtest/gtest.h>

#include <common/base_exception.hpp>

#include <nlohmann/json.hpp>

namespace {
    using namespace TB_NS::Error_NS;

    Exceptions& Errors{ Exceptions::GetIns() };

    void F1() {
        Exception::CR excption = Errors["F1()"];
        throw excption << TB_LOCATION;
    }

    void F2() {
        try {
            F1();
        } catch (Exception& error) {
            Exception::CR excption = Errors["F2()"];
            throw excption << TB_LOCATION << Suberror(error);
        }
    }

    void F3() {
        try {
            F2();
        } catch (Exception& error) {
            Exception::CR excption = Errors["F3()"];
            throw excption << TB_LOCATION << Suberror(error);
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
        Exceptions::GetIns().LoadSettings(TB_GET_CURRENT_PATH / "test_exceptions.json");
        try {
            throw Errors["DB"]["connection"]["incorrect name"]({"{name:}", "local.db"}) << TB_LOCATION;
            EXPECT_TRUE(false) << "unreachable code";
        } catch (Exception& error) {
            std::cout << error.what();
        }
    }
} // namespace