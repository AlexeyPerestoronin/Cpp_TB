// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "../trade_pair.hpp"

namespace TB_NS::Trader_NS::Interaction_NS {
    void TradePair::from(Str::CR i_str) noexcept {
        auto [before, after] = i_str.splite(",");
        FromStr(_1, before);
        FromStr(_2, after);
    }

    Str TradePair::to() const noexcept {
        return Str{ "_1,-2" }.format("_1", ToStr(_1)).format("_2", ToStr(_2));
    }
} // namespace TB_NS::Trader_NS::Interaction_NS
