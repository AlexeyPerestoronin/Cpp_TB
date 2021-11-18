// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

// There is the collection of aliases for the TB

#pragma once

#include <common/stdafx.hpp>

namespace TB_NS {
    namespace {
        template<typename Type>
        class BuiltInType {
            Type m_value;

            public:
            BuiltInType()
                : m_value{ Type{} } {}

            BuiltInType(Type i_value)
                : m_value{ i_value } {}

            BuiltInType& operator=(Type i_value) {
                m_value = i_value;
                return *this;
            }

#pragma region operator(s)
            operator Type() const noexcept {
                return m_value;
            }

            BuiltInType& operator++() noexcept {
                ++m_value;
                return *this;
            }

            BuiltInType operator++(int) noexcept {
                BuiltInType r_result{ *this };
                this->operator++();
                return r_result;
            }

            BuiltInType& operator--() noexcept {
                --m_value;
                return *this;
            }

            BuiltInType operator--(int) noexcept {
                BuiltInType r_result{ *this };
                this->operator--();
                return r_result;
            }

#define DEFINE_OPERATOR_1(opr)                         \
    BuiltInType& operator opr(Type i_value) noexcept { \
        m_value opr i_value;                           \
        return *this;                                  \
    }

            DEFINE_OPERATOR_1(+=)
            DEFINE_OPERATOR_1(-=)
            DEFINE_OPERATOR_1(*=)
            DEFINE_OPERATOR_1(/=)

#define DEFINE_OPERATOR_2(opr)                              \
    BuiltInType operator opr(Type i_value) const noexcept { \
        return m_value opr i_value;                         \
    }

            DEFINE_OPERATOR_2(+)
            DEFINE_OPERATOR_2(-)
            DEFINE_OPERATOR_2(*)
            DEFINE_OPERATOR_2(/)

#undef DEFINE_OPERATOR_2
#undef DEFINE_OPERATOR_1
#pragma endregion
        };
    } // namespace

    template<typename Type>
    struct AliasFor : public std::conditional_t<std::is_trivial_v<Type>, BuiltInType<Type>, Type> {
        using BaseType = std::conditional_t<std::is_trivial_v<Type>, BuiltInType<Type>, Type>;
        TB_PRS(AliasFor<BaseType>);

        using BaseType::BaseType;

        AliasFor(BaseType&& i_base) noexcept
            : BaseType(std::move(i_base)) {}

        AliasFor& operator=(BaseType&& i_base) noexcept {
            std::swap<BaseType>(*this, i_base);
            return *this;
        }

        AliasFor(const BaseType& i_base)
            : BaseType(i_base){};

        AliasFor& operator=(const BaseType& i_base) noexcept {
            static_cast<BaseType&>(*this) = i_base;
            return *this;
        }
    };

    using Path = AliasFor<fs::path>;
} // namespace TB_NS
