#include "../orders_book.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    CommandCode OrdersBook::code() const noexcept {
        return CommandID::GET_ORDER_BOOK;
    }

    OrdersBook::Responce OrdersBook::request(CurrencyPair i_pair, Limit i_limit) const {
        auto command = code();
        m_exchange->check(command, i_pair);
        m_exchange->check(command, i_limit);
        //m_exchange->interact(command, RequestedParameters{ i_pair, i_limit });
        return Responce();
        // TODO: there is need to insert an exception handling
    }
} // namespace TB_NS::Trader_NS::Interaction_NS
