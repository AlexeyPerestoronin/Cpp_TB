// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "../command_id_code.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    bool ToCommandIDFromStr(CommandID& i_value, Str::CR i_str) noexcept {
        static std::map<Str, CommandID> nameToCode{
            { "CommandID::UNSUPPORTED", CommandID::UNSUPPORTED },
            { "CommandID::GET_ORDER_BOOK", CommandID::GET_ORDER_BOOK },
        };

        for (const auto& [name, code] : nameToCode)
            if (name == i_str) {
                i_value = code;
                return true;
            }
        i_value = CommandID::UNSUPPORTED;
        return false;
    }

    Str ToStrfromCommandID(const CommandID& i_value) noexcept {
        static std::map<CommandID, Str> codeToName{
            { CommandID::UNSUPPORTED, "CommandID::UNSUPPORTED" },
            { CommandID::GET_ORDER_BOOK, "CommandID::GET_ORDER_BOOK" },
        };

        for (const auto& [code, name] : codeToName)
            if (code == i_value)
                return name;
        return "CommandID::UNSUPPORTED";
    }
} // namespace TB_NS::Trader_NS::Interaction_NS
