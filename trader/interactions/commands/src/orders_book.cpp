#include "../orders_book.hpp"

namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS {
    CommandCode OrdersBook::code() const noexcept {
        return CommandID::GET_ORDER_BOOK;
    }

    TB_NODISCARD OrdersBook::Response OrdersBook::request(TradePair i_pair, RequestLimit i_limit) const {
        try {
            auto interaction = m_exchange->createIteractions(code());
            interaction->prepare(i_pair);
            interaction->prepare(i_limit);
            return Response(std::move(i_pair), std::move(i_limit), m_exchange->code(), interaction->interact());
        } catch (TB_NS::Error_NS::Exception& error) {
            throw TB_EXEPT["CommandI"]["OrdersBook"] << TB_SUBERROR(error) << TB_LOCATION;
        }
    }
} // namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS
