#include <gtest/gtest.h>
#include <common/base_exception.hpp>
#include <exmo_api/via_curl/initializer.hpp>

int main(int argc, char* argv[]) {
    TB_NS::Error_NS::Exceptions Ins{ TB_NS::Error_NS::CreateExceptionsIns() };
    TB_NS::Exmo_API::LoadModule(Ins);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
