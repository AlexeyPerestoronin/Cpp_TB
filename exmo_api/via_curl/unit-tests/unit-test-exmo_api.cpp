#include <gtest/gtest.h>
#include <exmo_api/via_curl/exmo_api.hpp>
#include <trader/interactions/interaction.hpp>
#include <trader/interactions/commands/orders_book.hpp>

namespace {
    using namespace TB_NS::Trader_NS::Interaction_NS;

    ExchangeI::SP exchange = std::make_shared<TB_NS::Exmo_NS::ViaCurl_NS::ExmoExchange>("your_key", "your_secret");
    TradePair BTC_USD{ CurrencyID::BTC, CurrencyID::USD };

    TEST(ExmoAPI, GetOrderBook) {
        auto CheckOrders = [](Order::CRL i_orders, size_t i_expectedSize) {
            EXPECT_EQ(i_orders.size(), i_expectedSize);
            for (const auto& order : i_orders)
                EXPECT_NEAR(static_cast<double>(order.price * order.quantity), static_cast<double>(order.cost), 0.0000001);
        };

        Commands_NS::OrdersBook command{ exchange };
        ASSERT_EQ(CommandID::GET_ORDER_BOOK, command.code());
        Commands_NS::OrdersBook::Response response = command.request(BTC_USD, 5);
        EXPECT_EQ(response.request.pair._1, CurrencyID::BTC);
        EXPECT_EQ(response.request.pair._2, CurrencyID::USD);
        CheckOrders(response.response.sell.orders, 5);
        CheckOrders(response.response.buy.orders, 5);
    }
} // namespace