#pragma once

#include <common/common.hpp>
#include "exchange_id_code.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    enum class CommandID : uint8_t {
        UNSUPPORTED,
        GET_ORDER_BOOK,
    };

    using CurrencyPair = AliasFor<Str::O>;
    using Limit = AliasFor<std::optional<int>>;

    using RequestedParameter = AliasFor<std::variant<CurrencyPair, Limit>>;
    using RequestedParameters = AliasFor<RequestedParameter::L>;

    struct ExchangeI {
        TB_PRS(ExchangeI);
        virtual ExchangeCode getID() const noexcept = 0;
        virtual void check(CommandID command_id, CurrencyPair& io_pair) const = 0;
        virtual void check(CommandID command_id, Limit& io_limit) const = 0;
        virtual Json interact(CommandID command_id, RequestedParameters::CR i_params) = 0;
    };

    class CommandI {
        TB_PUBLIC_PRS(CommandI);

        protected:
        ExchangeI::SP m_exchange{};

        public:
        CommandI(ExchangeI::SP i_exchange) noexcept
            : m_exchange(std::move(i_exchange)) {}

        virtual ~CommandI() = default;

        virtual CommandID getID() const noexcept = 0;
    };
} // namespace TB_NS::Trader_NS::Interaction_NS
