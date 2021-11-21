#include "../orders_book.hpp"

namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS {
    CommandCode OrdersBook::code() const noexcept {
        return CommandID::GET_ORDER_BOOK;
    }

    Json OrdersBook::request(TradePair i_pair, TB_NS::Limit<Int>) const {
        QuantityLimit i_limit{ DefaultLimit };
        auto interaction = m_exchange->createIteractions(code());
        interaction->prepare(i_pair);
        interaction->prepare(i_limit);
        Json responce = interaction->interact();
        Str str = responce.dump(4);
        return responce;
        // TODO: there is need to insert an exception handling
    }
} // namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS
