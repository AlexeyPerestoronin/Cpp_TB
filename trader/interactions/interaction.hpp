#pragma once

#include "command_id_code.hpp"
#include "exchange_id_code.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    using CurrencyPair = AliasFor<Str::O>;
    using Limit = AliasFor<Int::O>;

    using RequestedParameter = AliasFor<std::variant<CurrencyPair, Limit>>;
    using RequestedParameters = AliasFor<RequestedParameter::L>;

    struct ExchangeI {
        TB_PRS(ExchangeI);
        virtual ExchangeCode code() const noexcept = 0;
        virtual void check(CommandCode i_commandCode, CurrencyPair& io_pair) const = 0;
        virtual void check(CommandCode i_commandCode, Limit& io_limit) const = 0;
        virtual Json interact(CommandCode i_commandCode, RequestedParameters::CR i_params) = 0;
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
