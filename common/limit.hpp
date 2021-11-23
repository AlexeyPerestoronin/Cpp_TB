// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include <common/base_exception.hpp>

namespace TB_NS {
    namespace {
        struct Default : StrI {
#pragma region StrI
            bool from(Str::CR i_str) noexcept override {
                if (i_str == "DefaultLimit")
                    return true;
                return false;
            }

            Str to() const noexcept override {
                return "DefaultLimit";
            }
#pragma endregion
        };
    } // namespace

    // brief: the default value for constructing an instance of Limit-class
    const Default DefaultLimit{};

    // brief: presents/holds the value or signals that the value is default
    // param-t: Type - the type of the presenting/holding value
    template<class Type>
    class Limit : public StrI {
        static_assert(
            std::is_convertible_v<const Type&, const StrI&> || std::is_same_v<Type, Str> || std::is_trivial_v<Type>,
            "here is possible to preserve only StrI-inherited types");

        TB_PUBLIC_PRS(Limit<Type>);
        using ValueType = Type;

        private:
        std::optional<Type> m_valueOpt{ std::nullopt };

        public:
        Limit(Type i_value) noexcept
            : m_valueOpt(std::move(i_value)) {}

        Limit& operator=(Type i_value) noexcept {
            m_valueOpt = std::move(i_value);
            return *this;
        }

        Limit(Default i_defaultSet) noexcept {};

        Limit& operator=(Default i_defaultSet) noexcept {
            m_valueOpt.reset();
            return *this;
        }

        Limit(Limit&& i_limit)
            : m_valueOpt(std::move(i_limit.m_valueOpt)) {}

#pragma region StrI
        bool from(Str::CR i_str) noexcept override {
            if (FromStr(const_cast<Default&>(DefaultLimit), i_str))
                return true;
            m_valueOpt = FromStr<Type>(i_str);
            return m_valueOpt.has_value();
        }

        Str to() const noexcept override {
            if (!m_valueOpt.has_value())
                return ToStr(DefaultLimit);
            return ToStr(m_valueOpt.value());
        }
#pragma endregion

        TB_NODISCARD bool isDefault() const noexcept {
            return m_valueOpt.has_value();
        }

        TB_NODISCARD const Type& getValue() const {
            if (!m_valueOpt.has_value())
                TB_EXEPT["Limit"]["no_value_presents"] << TB_LOCATION;
            return m_valueOpt.value();
        }

        TB_NODISCARD Type& getValue() {
            if (!m_valueOpt.has_value())
                TB_EXEPT["Limit"]["no_value_presents"] << TB_LOCATION;
            return m_valueOpt.value();
        }

        TB_NODISCARD const Type& getValueOrSet(const Type& i_value) const noexcept {
            if (!m_valueOpt.has_value())
                m_valueOpt = i_value;
            return m_valueOpt.value();
        }

        TB_NODISCARD Type& getValueOrSet(const Type& i_value) noexcept {
            if (!m_valueOpt.has_value())
                m_valueOpt = i_value;
            return m_valueOpt.value();
        }
    };
} // namespace TB_NS
