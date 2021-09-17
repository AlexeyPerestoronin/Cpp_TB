#include <gtest/gtest.h>
#include <exmo_api/exmo_api.hpp>

TEST(ExmoAPI, Simple1) {
	TB_NS::ExmoApi api("your_key", "your_secret");

    TB_NS::http::JsonData response = api.call("user_info", "");
    std::clog << ">> user_info: " << response << "\n\n";

    response = api.call("order_book", api.build({ "pair=BTC_USD" }));
    std::clog << ">> order_book: " << response << "\n\n";
	
    EXPECT_TRUE(true);
}