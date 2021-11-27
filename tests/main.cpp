// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include <gtest/gtest.h>
#include <common/base_exception.hpp>
#include <common/initializer.hpp>
#include <curl_adaptor/initializer.hpp>
#include <openssl_adaptor/initializer.hpp>
#include <exmo_api/via_curl/initializer.hpp>
#include <trader/initializer.hpp>

int main(int argc, char* argv[]) {
    try {
        TB_NS::LoadModule();
        TB_NS::CurlAdapter_NS::LoadModule();
        TB_NS::OpenSslAdapter_NS::LoadModule();
        TB_NS::Exmo_API::LoadModule();
        TB_NS::Trader_NS::LoadModule();

        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    } catch (std::exception& error) {
        std::cerr << error.what();
        return -1;
    }
}
