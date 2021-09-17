#include <gtest/gtest.h>

#include <common/base_exception.hpp>

#include <boost/throw_exception.hpp>

using namespace TB_NS;

namespace {
    void F1() {
        throw Exception("message: F1(...)", BOOST_CURRENT_LOCATION);
    }

    void F2() {
        try {
            F1();
        } catch (Exception& error) {
            throw Exception("message: F2(...)", BOOST_CURRENT_LOCATION);
        }
    }

    void F3() {
        try {
            F2();
        } catch (Exception& error) {
            throw Exception("message: F3(...)", BOOST_CURRENT_LOCATION, error);
        }
    }

    TEST(BaseException, Simple1) {
        try {
            F3();
            EXPECT_TRUE(false) << "unreachable code";
        } catch (Exception& error) {
            std::cout << boost::diagnostic_information(error, true);
        }
    }

    class A {
        public:
        A() {
            std::cout << "A()\n";
        }

        ~A() {
            std::cout << "~A()\n";
        }
    };

    class B {
        public:
        A a;

        B() {
            std::cout << "B()\n";
        }

        ~B() {
            std::cout << "~B()\n";
        }
    };

    class C {
        public:
        A a;

        C() {
            std::cout << "C()\n";
        }

        ~C() {
            std::cout << "~C()\n";
        }
    };

    TEST(BaseException, Simple2) {
        { B b1; }
        std::cout << "---\n";
        { B b2{}; }
        std::cout << "---\n";
        { C b3{}; }
    }
} // namespace