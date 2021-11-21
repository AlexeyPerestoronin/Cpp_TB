#pragma once

#include "command_id_code.hpp"
#include "exchange_id_code.hpp"
#include "trade_pair.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    using Limit = AliasFor<Int::O>;

    using RequestedParameter = AliasFor<std::variant<TradePair, Limit>>;
    using RequestedParameters = AliasFor<RequestedParameter::L>;

    struct ExchangeI : std::enable_shared_from_this<ExchangeI> {
        TB_PRS(ExchangeI);
        virtual ExchangeCode code() const noexcept = 0;
        virtual Json interact(CommandCode i_commandCode, RequestedParameters i_params) = 0;
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
