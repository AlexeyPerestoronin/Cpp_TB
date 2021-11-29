#include <tests/common.hpp>
#include <exmo_api/via_curl/exmo_api.hpp>
#include <trader/interactions/interaction.hpp>
#include <trader/interactions/commands/orders_book.hpp>

namespace TB_NS::UnitTests_NS {
    using namespace TB_NS::Trader_NS::Interaction_NS;

    ExchangeI::SP exchange = std::make_shared<TB_NS::Exmo_NS::ViaCurl_NS::ExmoExchange>("your_key", "your_secret");
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
} // namespace TB_NS::UnitTests_NS
