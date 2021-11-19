// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include <common/stdafx.hpp>

namespace TB_NS {
    // brief: the class extending base functional of standard strings
    class Str;


    class Str : public std::string {
        using KeyToValue = std::map<Str, Str>;
        TB_PUBLIC_PRS(Str);
        using BaseType = std::string;

        using BaseType::BaseType;
        Str(BaseType&& i_baseType) noexcept;
        Str& operator=(BaseType&& i_baseType) noexcept;
        Str(const BaseType& i_baseType);
        Str& operator=(const BaseType& i_baseType);

        TB_MAYBE_UNUSED Str& format(const BaseType& i_key, const BaseType& i_value);
        TB_MAYBE_UNUSED Str& format(const KeyToValue& i_replaceUnits);

        Str& operator+=(Str::CR i_str);
    };

    struct StrI {
        TB_PRS(StrI);
        virtual void from(Str::CR i_str) = 0;
        virtual Str to() const = 0;
    };

    // brief: converts the data of any type to string (Str)
    template<class Type>
    Str ToStr(const Type& i_value) {
        if constexpr (std::is_convertible_v<const Type&, const StrI&>)
            return i_value.to();
        else if constexpr (std::is_trivial_v<Type>)
            return std::to_string(i_value);
        else
            static_assert(std::false_type::value, "target Type cannot be convert to Str");
    }
} // namespace TB_NS
