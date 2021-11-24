// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../exchange_id_code.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    namespace {
        const std::vector<std::pair<Str, ExchangeID>> NameToCode{
            { "ExchangeID::UNSUPPORTED", ExchangeID::UNSUPPORTED },
            { "ExchangeID::EXMO", ExchangeID::EXMO },
        };
    }

    bool ToExchangeIDFromStr(ExchangeID& i_value, Str::CR i_str) noexcept {
        for (const auto& [name, code] : NameToCode)
            if (name == i_str) {
                i_value = code;
                return true;
            }
        i_value = ExchangeID::UNSUPPORTED;
        return false;
    }

    Str ToStrFromExchangeID(const ExchangeID& i_value) noexcept {
        for (const auto& [name, code] : NameToCode)
            if (code == i_value)
                return name;
        return "ExchangeID::UNSUPPORTED";
    }
} // namespace TB_NS::Trader_NS::Interaction_NS
