#include "../orders_book.hpp"

namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS {
    CommandCode OrdersBook::code() const noexcept {
        return CommandID::GET_ORDER_BOOK;
    }

    OrdersBook::Responce OrdersBook::getR(TradePair i_pair) {
        //Json responce = m_exchange->interact(code(), RequestedParameters{ std::move(i_pair), std::move(QuantityLimit{ Int{ 100 } }) });
        //Str str = responce.dump(4);
        return Responce{};
    }

    OrdersBook::Responce OrdersBook::request(TradePair i_pair, QuantityLimit i_limit) const {
        /*Json responce = */m_exchange->interact(code(), std::move(RequestedParameters{ std::move(i_pair), std::move(i_limit) }));
        return Responce();
        // TODO: there is need to insert an exception handling
    }
} // namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS
