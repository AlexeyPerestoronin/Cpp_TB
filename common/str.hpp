// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once
#include <common/stdafx.hpp>

namespace TB_NS {
    // brief: the class extending base functional of standard strings
    class Str;
    TB_EXTENDED_USING(StrToStr, std::map<Str, Str>);

    class Str : public std::string {
        TB_PUBLIC_PRS(Str);
        using BaseType = std::string;

        using BaseType::BaseType;
        Str(BaseType&& i_baseType) noexcept;
        Str& operator=(BaseType&& i_baseType) noexcept;
        Str(const BaseType& i_baseType);
        Str& operator=(const BaseType& i_baseType);

        TB_MAYBE_UNUSED Str& format(Str::CR i_key, Str::CR i_value);
        TB_MAYBE_UNUSED Str& format(StrToStr::CR i_replaceUnits);
    };
} // namespace TB_NS
