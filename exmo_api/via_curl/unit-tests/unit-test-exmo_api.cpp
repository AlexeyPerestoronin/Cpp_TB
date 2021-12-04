#include <tests/common.hpp>
#include <exmo_api/via_curl/exmo_api.hpp>
#include <trader/interactions/interaction.hpp>
#include <trader/interactions/commands/all.hpp>

namespace TB_NS::UnitTests_NS {
    using namespace TB_NS::Exmo_NS::ViaCurl_NS;
    using namespace TB_NS::Trader_NS::Interaction_NS;

    ExchangeI::SP exchange = ExmoExchange::NewSP("your_key", "your_secret");
    TradePair BTC_USD{ CurrencyID::BTC, CurrencyID::USD };

    TEST(ExmoAPI, GetOrderBook) {
        auto CheckOrders = [](Order::CRL i_orders, size_t i_expectedSize) -> bool {
            Error log{};
            EXPECT_EQ(i_orders.size(), i_expectedSize) << log("unexpected orders quantity");
            for (const auto& order : i_orders)
                EXPECT_NEAR(static_cast<double>(order.price * order.quantity), static_cast<double>(order.cost), 0.0000001) << log("failed order's accuracy");
            return log.result();
        };

        Commands_NS::OrdersBook command{ exchange };
        ASSERT_EQ(CommandID::GET_ORDER_BOOK, command.code());
        const int ordersQuantity{ 5 };
        Commands_NS::OrdersBook::Response response = command.request(BTC_USD, ordersQuantity);
        EXPECT_EQ(response.request.pair._1, CurrencyID::BTC);
        EXPECT_EQ(response.request.pair._2, CurrencyID::USD);
        EXPECT_TRUE(CheckOrders(response.response.sell.orders, ordersQuantity));
        EXPECT_TRUE(CheckOrders(response.response.buy.orders, ordersQuantity));
    }

    TEST(ExmoAPI, GetPairSettings) {
        Commands_NS::PairSettigs command{ exchange };
        ASSERT_EQ(CommandID::GET_PAIR_SETTINGS, command.code());
        Commands_NS::PairSettigs::Response response = command.request(BTC_USD);
        EXPECT_EQ(response.request.pair._1, CurrencyID::BTC);
        EXPECT_EQ(response.request.pair._2, CurrencyID::USD);
        EXPECT_LT(response.response.quantity.min, response.response.quantity.max);
        EXPECT_LT(response.response.price.min, response.response.price.max);
        EXPECT_LT(response.response.cost.min, response.response.cost.max);
    }
} // namespace TB_NS::UnitTests_NS
