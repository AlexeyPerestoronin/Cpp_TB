// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "command_id_code.hpp"
#include "exchange_id_code.hpp"
#include "trade_pair.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    // brief: data type for the cost of an order
    using Cost = Double;
    // brief: data type for the price of an order
    using Price = Double;
    // brief: data type for the quantity of a currency of an order
    using Quantity = Double;
    // brief: one order presentation object
    struct Order {
        TB_PRS(Order);
        Cost cost;
        Price price;
        Quantity quantity;
    };
    // brief: data type that is presenting a limit in an Exchange platform request
    using RequestLimit = Limit<int>;

    // brief: common interface for interacting with an Exchange platform
    class InteractI : Singleton {
        protected:
        CommandCode m_commandCode;

        TB_PUBLIC_PRS(InteractI);
        InteractI(CommandCode i_commandCode) noexcept
            : m_commandCode(i_commandCode){};

        virtual void prepare(TradePair& io_tradePair) = 0;
        virtual void prepare(RequestLimit& io_quantityLimits) = 0;
        virtual Json interact() = 0;
    };

    // brief: common interface for presenting all Exchange platforms
    struct ExchangeI : Singleton {
        TB_PRS(ExchangeI);
        virtual ExchangeCode code() const noexcept = 0;
        virtual InteractI::SP createIteractions(CommandCode i_commandCode) = 0;
    };

    // brief: common interface for presenting all commands that are requested some data from an Exchange platform
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
