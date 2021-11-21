#include <gtest/gtest.h>
#include <exmo_api/via_curl/exmo_api.hpp>
#include <trader/interactions/interaction.hpp>
#include <trader/interactions/commands/orders_book.hpp>

namespace {
    using namespace TB_NS::Trader_NS::Interaction_NS;

    ExchangeI::SP exchange = TB_NS::Exmo_NS::API("your_key", "your_secret").shared_from_this();

    TEST(ExmoAPI, GetOrderBook) {
        Commands_NS::OrdersBook command{ exchange };
        command.code();
        //Limit lim{};
        command.getR(TradePair{ CurrencyID::BTC, CurrencyID::USD });
        //command.request(TradePair{ CurrencyID::BTC, CurrencyID::USD }, lim);
    }

    TEST(ExmoAPI, Simple2) {
        TB_NS::Exmo_NS::API api("your_key", "your_secret");
        TB_NS::Json response = api.call("user_info", "");
        std::clog << ">> user_info: " << response.dump(4) << "\n\n";

        response = api.call("order_book", api.build_dd({ "pair=BTC_USD" }));
        std::clog << ">> order_book: " << response.dump(4) << "\n\n";

        EXPECT_TRUE(true);
    }
} // namespace