#include "../orders_book.hpp"

namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS {
    OrdersBook::Responce::Responce(TradePair i_pair, RequestLimit i_limit, ExchangeCode i_code, Json::CR i_json) {
        TB_CONST_CAST(request.pair) = std::move(i_pair);
        TB_CONST_CAST(request.limit) = std::move(i_limit);
    }
} // namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS
