// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../pair_settings.hpp"

namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS {
#define PS_RESPONSE TB_EXEPT["CommandI"]["PairSettigs"]["Response"]
    namespace {
     void ParseExmoResponse(PairSettigs::Response::ReceivedData& io_response, TradePair::CR i_pair, Json::CR i_json) {
            if (i_pair.view.status())
             throw PS_RESPONSE["Mangled TradePair"]("{view:}", i_pair.view) << TB_LOCATION;

            bool isSuccess{ true };
            Json::CR forPair = i_json[i_pair.view];

            isSuccess &= io_response.quantity.max.fromJson(forPair["max_quantity"]);
            isSuccess &= io_response.quantity.min.fromJson(forPair["min_quantity"]);
            isSuccess &= io_response.price.max.fromJson(forPair["max_price"]);
            isSuccess &= io_response.price.min.fromJson(forPair["min_price"]);
            isSuccess &= io_response.cost.max.fromJson(forPair["max_amount"]);
            isSuccess &= io_response.cost.min.fromJson(forPair["min_amount"]);

            if (!isSuccess)
                throw PS_RESPONSE["Parsing Error"]("{json:}", i_json.dump()) << TB_LOCATION;
        }
    } // namespace

    PairSettigs::Response::Response(TradePair i_pair, ExchangeCode i_code, Json::CR i_json) {
        try {
            TB_CONST_CAST(request.pair) = std::move(i_pair);

            if (i_code.value() == ExchangeID::EXMO) {
                ParseExmoResponse(TB_CONST_CAST(response), request.pair, i_json);
            } else {
                throw PS_RESPONSE["Unsupported Exchange"]("{platform:}", ToStr(i_code)) << TB_LOCATION;
            }
        } catch (TB_NS::Error_NS::Exception& error) {
            throw PS_RESPONSE << TB_SUBERROR(error) << TB_LOCATION;
        }
    }
#undef PS_RESPONSE
} // namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS
