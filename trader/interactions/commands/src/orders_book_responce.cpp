#include "../orders_book.hpp"

namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS {
    namespace {
        void ParseExmoResponse(OrdersBook::Response::ReceivedData& io_response, Json::CR i_json) {
            //for (const auto& [currency_pair, values] : i_json.items())
                //io_response.sell.quantity = values[""];
            return;
        }
    }
    OrdersBook::Response::Response(TradePair i_pair, RequestLimit i_limit, ExchangeCode i_code, Json::CR i_json) {
        try {
            TB_CONST_CAST(request.pair) = std::move(i_pair);
            TB_CONST_CAST(request.limit) = std::move(i_limit);

            if (i_code == ExchangeID::EXMO) {
                ParseExmoResponse(TB_CONST_CAST(response), i_json);
            } else {
                throw TB_EXEPT["CommandI"]["OrdersBook"]["Response"]["Unsupported Exchange"]("{platform:}", ToStr(i_code)) << TB_LOCATION;
            }
        } catch (TB_NS::Error_NS::Exception& error) {
            throw TB_EXEPT["CommandI"]["OrdersBook"]["Response"] << TB_SUBERROR(error) << TB_LOCATION;
        }
    }
} // namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS
