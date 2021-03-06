// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "../trade_pair.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    bool TradePair::operator==(TradePair::CR i_tradePair) const noexcept {
        return _1.value() == i_tradePair._1.value() && _2.value() == i_tradePair._2.value();
    }

    bool TradePair::fromStr(Str::CR i_str) noexcept {
        auto [before, after] = i_str.splite(",");
        bool r_result{ true };
        r_result &= FromStr(_1, before);
        r_result &= FromStr(_2, after);
        return r_result;
    }

    Str TradePair::toStr() const noexcept {
        return Str{ "_1,-2" }.format("_1", ToStr(_1)).format("_2", ToStr(_2));
    }
} // namespace TB_NS::Trader_NS::Interaction_NS
