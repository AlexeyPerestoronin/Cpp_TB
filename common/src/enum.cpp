// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "../enum.hpp"

namespace TB_NS {

    Enum& Enum::add(int i_index, Str::CR i_view) {
        m_intToStr.emplace(i_index, i_view);
        m_strToInt.emplace(i_view, i_index);
        return *this;
    }
} // namespace TB_NS
