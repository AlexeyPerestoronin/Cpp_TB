// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../orders_book.hpp"

namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS {
    namespace {
        void ParseExmoResponse(OrdersBook::Response::ReceivedData& io_response, Json::CR i_json) {
            bool isSuccess{ true };
            for (int counter{ 0 }; const auto& [currencyPair, values] : i_json.items()) {
                // TODO: only one currency pair is currently processed
                if (counter)
                    continue;

                isSuccess &= FromJson(io_response.sell.quantity, values["ask_quantity"]);
                isSuccess &= FromJson(io_response.sell.cost, values["ask_amount"]);
                for (const auto& sellOrders : values["ask"]) {
                    Order order;
                    isSuccess &= FromJson(order.price, sellOrders[0]);
                    isSuccess &= FromJson(order.quantity, sellOrders[1]);
                    isSuccess &= FromJson(order.cost, sellOrders[2]);
                    io_response.sell.orders.push_back(std::move(order));
                }

                isSuccess &= FromJson(io_response.buy.quantity, values["bid_quantity"]);
                isSuccess &= FromJson(io_response.buy.cost, values["bid_amount"]);
                for (const auto& buyOrders : values["bid"]) {
                    Order order;
                    isSuccess &= FromJson(order.price, buyOrders[0]);
                    isSuccess &= FromJson(order.quantity, buyOrders[1]);
                    isSuccess &= FromJson(order.cost, buyOrders[2]);
                    io_response.buy.orders.push_back(std::move(order));
                }

                ++counter;
            }

            if (!isSuccess)
                throw TB_EXEPT["CommandI"]["OrdersBook"]["Response"]["Parsing Error"]("{json:}", i_json.dump()) << TB_LOCATION;
        }
    } // namespace

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
