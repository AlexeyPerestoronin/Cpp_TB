#pragma once

#include "../interaction.hpp"

namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS {
    struct OrdersBook : CommandI {
        TB_PRS(OrdersBook);

        using CommandI::CommandI;

#pragma region CommandI
        CommandCode code() const noexcept override;
#pragma endregion

        struct Responce {
            struct {
                const TradePair pair{};
                const RequestLimit limit{};
            } request{};
            

            struct {
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
            } response{};

            private:
            Responce(TradePair i_pair, RequestLimit i_limit, ExchangeCode i_code, Json::CR i_json);
        };

        Json request(TradePair i_pair, RequestLimit i_limit = DefaultLimit) const;
    };
} // namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS
