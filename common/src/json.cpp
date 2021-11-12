// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../json.hpp"

namespace TB_NS {
    Json::Json(BaseType&& i_baseType) noexcept
        : BaseType(std::move(i_baseType)) {}

    Json& Json::operator=(BaseType&& i_baseType) noexcept {
        BaseType::operator=(std::move(i_baseType));
        return *this;
    }

    Json::Json(const BaseType& i_baseType)
        : BaseType(i_baseType) {}

    Json& Json::operator=(const BaseType& i_baseType) {
        BaseType::operator=(i_baseType);
        return *this;
    }

    Json::Json(Str&& i_str) noexcept
        : BaseType(static_cast<Str::BaseType&&>(std::move(i_str))) {}

    Json& Json::operator=(Str&& i_str) noexcept {
        BaseType::operator=(static_cast<Str::BaseType&&>(std::move(i_str)));
        return *this;
    }

    Json::Json(Str::CR i_str)
        : BaseType(std::move(static_cast<Str::BaseType>(i_str))) {}

    Json& Json::operator=(Str::CR i_str) {
        BaseType::operator=(std::move(static_cast<Str::BaseType>(i_str)));
        return *this;
    }

    Json::operator Str() const noexcept {
        return Str(std::move(static_cast<Str::BaseType>(*this)));
    }
} // namespace TB_NS
