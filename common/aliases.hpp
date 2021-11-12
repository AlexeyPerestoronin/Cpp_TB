// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

// There is the collection of aliases for the TB

#pragma once

#include <common/stdafx.hpp>
#include <common/str.hpp>

#include <nlohmann/json.hpp>

namespace TB_NS {
    TB_EXTENDED_USING(IntToInt, std::map<int, int>);
    TB_EXTENDED_USING(StrView, std::string_view);
    TB_EXTENDED_USING(Path, fs::path);
} // namespace TB_NS
