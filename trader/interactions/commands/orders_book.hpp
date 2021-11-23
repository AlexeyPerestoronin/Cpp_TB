#pragma once

#include "../interaction.hpp"

namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS {
    struct OrdersBook : CommandI {
        TB_PRS(OrdersBook);

        using CommandI::CommandI;

#pragma region CommandI
        CommandCode code() const noexcept override;
#pragma endregion

        //struct Responce {
        //    // TODO: ...
        //};

        Json request(TradePair i_pair, QuantityLimit i_limit = DefaultLimit) const;
    };
} // namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS
