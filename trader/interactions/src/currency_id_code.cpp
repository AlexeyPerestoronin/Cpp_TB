// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "../currency_id_code.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    namespace {
        std::vector<std::pair<Str, CurrencyID>> NameToCode{
            { "CurrencyID::UNSUPPORTED", CurrencyID::UNSUPPORTED },
            // fiat
            { "CurrencyID::USD", CurrencyID::USD },
            { "CurrencyID::RUB", CurrencyID::RUB },
            // crypto
            { "CurrencyID::BTC", CurrencyID::BTC },
        };
    } // namespace

    bool ToCurrencyIDFromStr(CurrencyID& i_value, Str::CR i_str) noexcept {
        for (const auto& [name, code] : NameToCode)
            if (name == i_str) {
                i_value = code;
                return true;
            }
        i_value = CurrencyID::UNSUPPORTED;
        return false;
    }

    Str ToStrFromCurrencyID(const CurrencyID& i_value) noexcept {
        for (const auto& [name, code] : NameToCode)
            if (code == i_value)
                return name;
        return "CurrencyID::UNSUPPORTED";
    }
} // namespace TB_NS::Trader_NS::Interaction_NS
