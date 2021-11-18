// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include <common/stdafx.hpp>
#include <common/str.hpp>

#include <nlohmann/json.hpp>

namespace TB_NS {
    // brief: this class is the small overbuild for nlohmann::json
    class Json : public nlohmann::json {
        TB_PUBLIC_PRS(Json);
        using BaseType = nlohmann::json;

        using BaseType::BaseType;
        Json(BaseType&& i_baseType) noexcept;
        Json& operator=(BaseType&& i_baseType) noexcept;
        Json(const BaseType& i_baseType);
        Json& operator=(const BaseType& i_baseType);

        Json(Str&& i_str) noexcept;
        Json& operator=(Str&& i_str) noexcept;
        Json(Str::CR i_str);
        Json& operator=(Str::CR i_str);
        operator Str() const noexcept;
    };
} // namespace TB_NS
