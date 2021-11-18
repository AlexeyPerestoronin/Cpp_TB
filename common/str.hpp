// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once
#include <common/stdafx.hpp>
#include <common/aliases.hpp>

namespace TB_NS {
    // brief: the class extending base functional of standard strings
    class Str;
    using VStrs = AliasFor<std::vector<Str>>;
    using LStrs = AliasFor<std::list<Str>>;
    using StrToStr = AliasFor<std::map<Str, Str>>;
    using StrToInt = AliasFor<std::map<Str, int>>;
    using IntToStr = AliasFor<std::map<int, Str>>;

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

        Str& operator+=(Str::CR i_str);
    };

    struct StrI {
        TB_PRS(StrI);
        virtual void from(Str::CR i_str) noexcept = 0;
        virtual Str to() noexcept = 0;
    };

    // brief: convers the data of any type to string (Str)
    // param: i_value - target data to conversion
    template<class FromType>
    Str ToStr(FromType&& i_value) {
        if constexpr (std::is_convertible_v<std::decay_t<FromType>, int>)
            return std::to_string(i_value);
        else
            static_assert(std::false_type::value, "target Type cannot be convert to Str");
    }
} // namespace TB_NS
