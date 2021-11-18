#include <gtest/gtest.h>
#include <exmo_api/via_curl/exmo_api.hpp>

namespace {
    TEST(ExmoAPI, Simple1) {
        TB_NS::Exmo_NS::API api("your_key", "your_secret");

        TB_NS::Json response = api.call("user_info", "");
        std::clog << ">> user_info: " << response.dump(4) << "\n\n";

        response = api.call("order_book", api.build({ "pair=BTC_USD" }));
        std::clog << ">> order_book: " << response.dump(4) << "\n\n";

        EXPECT_TRUE(true);
    }
} // namespace