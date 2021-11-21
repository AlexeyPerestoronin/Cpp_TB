// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "../currency_id_code.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    void ToCurrencyIDFromStr(CurrencyID& i_value, Str::CR i_str) noexcept {
        static StrToInt nameToCode{
            { "CurrencyID::UNSUPPORTED", CurrencyID::UNSUPPORTED },
            // fiat
            { "CurrencyID::USD", CurrencyID::USD },
            { "CurrencyID::RUB", CurrencyID::RUB },
            // crypto
            { "CurrencyID::BTC", CurrencyID::BTC },
        };

        for (const auto& [name, code] : nameToCode)
            if (name == i_str) {
                i_value = code;
                return;
            }
        i_value = CurrencyID::UNSUPPORTED;
    }

    Str ToStrFromCurrencyID(const CurrencyID& i_value) noexcept {
        static IntToStr codeToName{
            { CurrencyID::UNSUPPORTED , "CurrencyID::UNSUPPORTED" },
            // fiat
            { CurrencyID::USD , "CurrencyID::USD" },
            { CurrencyID::RUB , "CurrencyID::RUB" },
            // crypto
            { CurrencyID::BTC , "CurrencyID::BTC" },
        };

        for (const auto& [code, name] : codeToName)
            if (code == i_value)
                return name;
        return "CurrencyID::UNSUPPORTED";
    }
} // namespace TB_NS::Trader_NS::Interaction_NS
