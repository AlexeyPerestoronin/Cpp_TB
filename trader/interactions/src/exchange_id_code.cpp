// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../exchange_id_code.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    void ToExchangeIDFromStr(ExchangeID& i_value, Str::CR i_str) noexcept {
        static StrToInt nameToCode{
            { "ExchangeID::UNSUPPORTED", Int{ ExchangeID::UNSUPPORTED } },
            { "ExchangeID::EXMO", Int{ ExchangeID::EXMO } },
        };

        for (const auto& [name, code] : nameToCode)
            if (name == i_str) {
                i_value = code;
                return;
            }
        i_value = ExchangeID::UNSUPPORTED;
    }

    Str ToStrExchangeIDType(const ExchangeID& i_value) noexcept {
        static IntToStr codeToName{
            { Int{ ExchangeID::UNSUPPORTED }, "ExchangeID::UNSUPPORTED" },
            { Int{ ExchangeID::EXMO }, "ExchangeID::EXMO" },
        };

        for (const auto& [code, name] : codeToName)
            if (code == i_value)
                return name;
        return "ExchangeID::UNSUPPORTED";
    }
} // namespace TB_NS::Trader_NS::Interaction_NS
