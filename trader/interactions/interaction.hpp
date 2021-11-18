#pragma once

#include <common/common.hpp>

namespace TB_NS::Trader_NS {
    namespace Interaction_NS {
        enum class ExchangeID : uint8_t {
            UNSUPPORTED,
            EXMO,
        };

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
            virtual ExchangeID getID() const noexcept = 0;
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
    } // namespace Interaction_NS
} // namespace TB_NS::Trader_NS
