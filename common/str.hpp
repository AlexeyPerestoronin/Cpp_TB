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

    class Str final : public std::string {
        TB_PUBLIC_PRS(Str);
        using BaseType = std::string;

        using BaseType::BaseType;
        Str(BaseType&& i_baseType) noexcept;
        Str& operator=(BaseType&& i_baseType) noexcept;
        Str(const BaseType& i_baseType);
        Str& operator=(const BaseType& i_baseType);

        TB_MAYBE_UNUSED Str& format(const BaseType& i_key, const BaseType& i_value) noexcept;
        TB_MAYBE_UNUSED Str& format(const std::map<Str, Str>& i_replaceUnits) noexcept;

        std::pair<Str, Str> splite(Str::CR i_anchor) const noexcept;

        Str& operator+=(Str::CR i_str);
    };

    std::ostream& operator<<(std::ostream& i_stream, Str::CR i_str);
    
    std::ifstream& operator>>(std::ifstream& i_stream, Str& i_str);

    struct StrI {
        TB_PRS(StrI);
        // NOTE: since this interface could be used with exception(s) the both of its functions should be marked as noexcept.

        // brief: creates Object of a inherited-Type from the string
        // param: i_str - the string from which the Object have to be constructed
        // return: true - if successfully constructed; false - in other vise
        TB_MAYBE_UNUSED virtual bool fromStr(Str::CR i_str) noexcept = 0;

        // brief: converts Object of a inherited-Type to a string
        // return: string presents the Object
        virtual Str toStr() const noexcept = 0;
    };

    // brief: converts the data of any type to string (Str)
    template<class Type>
    TB_NODISCARD Str ToStr(const Type& i_value) noexcept {
        if constexpr (std::is_convertible_v<const Type&, const StrI&>)
            return i_value.toStr();
        else if constexpr (std::is_same_v<Type, Str>)
            return i_value;
        else if constexpr (std::is_trivial_v<Type>)
            return std::to_string(i_value);
        else
            static_assert(std::false_type::value, "target Type cannot be convert toStr Str");
    }

    // brief: converts target string to any data
    template<class Type>
    TB_MAYBE_UNUSED bool FromStr(Type& io_value, Str::CR i_str) noexcept {
        if constexpr (std::is_convertible_v<const Type&, const StrI&>)
            return io_value.fromStr(i_str);
        else if constexpr (std::is_same_v<Type, Str>) {
            io_value = i_str;
            return true;
        } else if constexpr (std::is_trivial_v<Type>) {
            try {
                std::stringstream stream{};
                stream << i_str;
                stream >> io_value;
                return true;
            } catch (...) {
                return false;
            }
        } else
            static_assert(std::false_type::value, "target Type cannot be convert toStr Str");
    }

    // brief: converts target string to any data
    template<class Type>
    TB_NODISCARD std::optional<Type> FromStr(Str::CR i_str) noexcept {
        if constexpr (std::is_convertible_v<const Type&, const StrI&>) {
            if (Type value; value.fromStr(i_str))
                return value;
            return std::nullopt;
        } else if constexpr (std::is_same_v<Type, Str>) {
            return i_str;
        } else if constexpr (std::is_trivial_v<Type>) {
            try {
                Type value;
                std::stringstream stream{};
                stream << i_str;
                stream >> value;
                return value;
            } catch (...) {
                return std::nullopt;
            }
        } else
            static_assert(std::false_type::value, "target Type cannot be convert toStr Str");
    }
} // namespace TB_NS
