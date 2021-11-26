// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include <common/str.hpp>

#include <nlohmann/json.hpp>

namespace TB_NS {
    // brief: this class is the small overbuild for nlohmann::json
    class Json final
        : public nlohmann::json
        , public States_NS::MangledState {
        TB_PUBLIC_PRS(Json);
        using BaseType = nlohmann::json;

        using BaseType::BaseType;
        using States_NS::MangledState::MangledState;
        Json() noexcept = default;
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

    struct JsonI {
        // NOTE: since this interface could be used with exception(s) the both of its functions should be marked as noexcept.

        // brief: creates Object of a inherited-Type from the json-object
        // param: i_json - the json-object from which the Object have to be constructed
        // return: true - if successfully constructed; false - in other vise
        TB_MAYBE_UNUSED virtual bool fromJson(Json::CR i_json) noexcept = 0;

        // brief: converts Object of a inherited-Type to a json-object
        // return: json-object presents the Object
        // note1: if it is impossible to convert the Object in a Json-object the returned json-object will be marked as mangled
        virtual Json toJson() const noexcept = 0;
    };

    // brief: converts target json-object to any data
    template<class Type>
    TB_MAYBE_UNUSED bool FromJson(Type& io_value, Json::CR i_json) noexcept {
        if constexpr (std::is_convertible_v<const Type&, const JsonI&>)
            return io_value.fromJson(i_json);
        else if constexpr (std::is_same_v<Type, Json>) {
            io_value = i_json;
            return true;
        } else if constexpr (std::is_trivial_v<Type>) {
            try {
                io_value = static_cast<Type>(i_json);
                return true;
            } catch (...) {
                return false;
            }
        } else
            static_assert(std::false_type::value, "target Type cannot be convert from Json");
    }

    // brief: converts target json-object to any data
    template<class Type>
    TB_NODISCARD std::optional<Type> FromJson(Str::CR i_json) noexcept {
        if constexpr (std::is_convertible_v<const Type&, const JsonI&>) {
            if (Type value; value.fromJson(i_json))
                return value;
            return std::nullopt;
        } else if constexpr (std::is_same_v<Type, Json>) {
            return i_json;
        } else if constexpr (std::is_trivial_v<Type>) {
            try {
                return static_cast<Type>(i_json);
            } catch (...) {
                return std::nullopt;
            }
        } else
            static_assert(std::false_type::value, "target Type cannot be convert from Json");
    }
} // namespace TB_NS
