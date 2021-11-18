#include "../orders_book.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    CommandID OrdersBook::getID() const noexcept {
        return CommandID::GET_ORDER_BOOK;
    }

    OrdersBook::Responce OrdersBook::request(CurrencyPair i_pair, Limit i_limit) const {
        auto commandID = getID();
        m_exchange->check(commandID, i_pair);
        m_exchange->check(commandID, i_limit);
        m_exchange->interact(commandID, RequestedParameters{ i_pair, i_limit });
        return Responce();
        // TODO: there is need to insert an exception handling
    }
} // namespace TB_NS::Trader_NS::Interaction_NS
