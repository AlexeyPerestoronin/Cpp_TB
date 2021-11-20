// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "../command_id_code.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    void ToCommandIDFromStr(CommandID& i_value, Str::CR i_str) noexcept {
        static StrToInt nameToCode{
            { "CommandID::UNSUPPORTED", Int{ CommandID::UNSUPPORTED } },
            { "CommandID::GET_ORDER_BOOK", Int{ CommandID::GET_ORDER_BOOK } },
        };

        for (const auto& [name, code] : nameToCode)
            if (name == i_str) {
                i_value = code;
                return;
            }
        i_value = CommandID::UNSUPPORTED;
    }

    Str ToStrfromCommandID(const CommandID& i_value) noexcept {
        static IntToStr codeToName{
            { Int{ CommandID::UNSUPPORTED }, "CommandID::UNSUPPORTED" },
            { Int{ CommandID::GET_ORDER_BOOK }, "CommandID::GET_ORDER_BOOK" },
        };

        for (const auto& [code, name] : codeToName)
            if (code == i_value)
                return name;
        return "CommandID::UNSUPPORTED";
    }
} // namespace TB_NS::Trader_NS::Interaction_NS
