#pragma once

#include "command_id_code.hpp"
#include "exchange_id_code.hpp"
#include "trade_pair.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    using QuantityLimit = Limit<int>;

    class InteractI : Singleton {
        protected:
        CommandCode m_commandCode;

        TB_PUBLIC_PRS(InteractI);
        InteractI(CommandCode i_commandCode) noexcept
            : m_commandCode(i_commandCode){};

        virtual void prepare(TradePair& io_tradePair) = 0;
        virtual void prepare(QuantityLimit& io_quantityLimits) = 0;
        virtual Json interact() = 0;
    };

    struct ExchangeI : Singleton {
        TB_PRS(ExchangeI);
        virtual ExchangeCode code() const noexcept = 0;
        virtual InteractI::SP createIteractions(CommandCode i_commandCode) = 0;
    };

    class CommandI {
        TB_PUBLIC_PRS(CommandI);

        protected:
        ExchangeI::SP m_exchange{};

        public:
        CommandI(ExchangeI::SP i_exchange) noexcept
            : m_exchange(std::move(i_exchange)) {}

        virtual ~CommandI() = default;

        virtual CommandCode code() const noexcept = 0;
    };
} // namespace TB_NS::Trader_NS::Interaction_NS
