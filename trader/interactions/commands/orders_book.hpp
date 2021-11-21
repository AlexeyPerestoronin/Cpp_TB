#pragma once

#include "../interaction.hpp"

namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS {
    class OrdersBook : public CommandI {
        TB_PUBLIC_PRS(CommandI);

        protected:
        ExchangeI::SP m_exchange{};

        public:
#pragma region CommandI
        using CommandI::CommandI;
        CommandCode code() const noexcept override;
#pragma endregion

        struct Responce {
            // TODO: ...
        };

        Responce getR(TradePair i_pair);

        Responce request(TradePair i_pair, QuantityLimit i_limit = DefaultLimit) const;
    };
} // namespace TB_NS::Trader_NS::Interaction_NS::Commands_NS
