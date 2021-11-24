#include <gtest/gtest.h>
#include <exmo_api/via_curl/exmo_api.hpp>
#include <trader/interactions/interaction.hpp>
#include <trader/interactions/commands/orders_book.hpp>

namespace {
    using namespace TB_NS::Trader_NS::Interaction_NS;

    ExchangeI::SP exchange = std::make_shared<TB_NS::Exmo_NS::ViaCurl_NS::ExmoExchange>("your_key", "your_secret");
    TradePair BTC_USD{ CurrencyID::BTC, CurrencyID::USD };

    TEST(ExmoAPI, GetOrderBook) {
        Commands_NS::OrdersBook command{ exchange };
        ASSERT_EQ(CommandID::GET_ORDER_BOOK, command.code());
        command.request(BTC_USD, 5);
    }
} // namespace