// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../str.hpp"

namespace TB_NS {
    Str::Str(BaseType&& i_baseType) noexcept
        : BaseType(std::move(i_baseType)) {}

    Str& Str::operator=(BaseType&& i_baseType) noexcept {
        BaseType::operator=(std::move(i_baseType));
        return *this;
    }

    Str::Str(const BaseType& i_baseType)
        : BaseType(i_baseType) {}

    Str& Str::operator=(const BaseType& i_baseType) {
        BaseType::operator=(i_baseType);
        return *this;
    }

    TB_MAYBE_UNUSED Str& Str::format(const BaseType& i_key, const BaseType& i_value) {
        for (size_t start = BaseType::find(i_key), len = i_key.size(); start != BaseType::npos; start = BaseType::find(i_key))
            BaseType::replace(start, len, i_value);
        return *this;
    }

    TB_MAYBE_UNUSED Str& Str::format(const KeyToValue& i_replaceUnits) {
        for (const auto& [key, value] : i_replaceUnits)
            format(key, value);
        return *this;
    }

    Str& Str::operator+=(Str::CR i_str) {
        BaseType::append(i_str.data(), i_str.size());
        return *this;
    }
} // namespace TB_NS
