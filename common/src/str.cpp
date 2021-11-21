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

    TB_MAYBE_UNUSED Str& Str::format(const BaseType& i_key, const BaseType& i_value) noexcept {
        for (size_t start = BaseType::find(i_key), len = i_key.size(); start != BaseType::npos; start = BaseType::find(i_key))
            BaseType::replace(start, len, i_value);
        return *this;
    }

    TB_MAYBE_UNUSED Str& Str::format(const std::map<Str, Str>& i_replaceUnits) noexcept {
        for (const auto& [key, value] : i_replaceUnits)
            format(key, value);
        return *this;
    }

    std::pair<Str, Str> Str::splite(Str::CR i_anchor) const noexcept {
        std::pair<Str, Str> r_result{};
        auto& [before, after] = r_result;
        size_t start = std::min(BaseType::find(i_anchor), BaseType::size());
        before = Str(BaseType::data(), start);
        after = Str(BaseType::data() + start + i_anchor.size(), BaseType::size());
        return r_result;
    }

    Str& Str::operator+=(Str::CR i_str) {
        BaseType::append(i_str.data(), i_str.size());
        return *this;
    }
} // namespace TB_NS
