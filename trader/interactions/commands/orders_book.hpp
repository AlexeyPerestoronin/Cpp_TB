#pragma once

#include "../interaction.hpp"

namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS {
    struct OrdersBook final : CommandI {
        TB_PRS(OrdersBook);

        using CommandI::CommandI;

#pragma region CommandI
        CommandCode code() const noexcept override;
#pragma endregion

        struct Response {
            friend struct OrdersBook;

            struct RequestedData {
                TradePair pair{};
                RequestLimit limit{};
            } const request{};
            
            struct ReceivedData {
                struct {
                    Quantity quantity{};
                    Cost cost{};
                    Order::L orders{};
                } sell{};

                struct {
                    Quantity quantity{};
                    Cost cost{};
                    Order::L orders{};
                } buy{};
            } const response{};

            private:
            Response(TradePair i_pair, RequestLimit i_limit, ExchangeCode i_code, Json::CR i_json);
        };

        TB_NODISCARD Response request(TradePair i_pair, RequestLimit i_limit = DefaultLimit) const;
    };
} // namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS
