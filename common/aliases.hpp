// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

// There is the collection of aliases for the TB

#pragma once

#include <common/str.hpp>

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

    // brief: creates the usage-friendly alias for the target type
    // param-t: Type - target type for which the alias should be built
    // param-t: FromStr - pointer to a function converts a value of Str-type to the Type
    // param-t: ToStr - pointer to a function converts a value of the Type to Str-type
    template<typename Type, void (*FromStr)(Type&, Str::CR) = nullptr, Str (*ToStr)(const Type&) = nullptr>
    struct AliasFor
        : StrI
        , std::conditional_t<std::is_trivial_v<Type>, BuiltInType<Type>, Type> {
        static constexpr bool IsTrivialType = std::is_trivial_v<Type>;
        using BaseType = std::conditional_t<IsTrivialType, BuiltInType<Type>, Type>;
        TB_PRS(AliasFor<BaseType>);

        using BaseType::BaseType;
        using BaseType::operator=;

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

#pragma region StrI
        void from(Str::CR i_str) override final {
            if (!FromStr)
                // Think-Over-Point: maybe there is better to raise an exception
                return;
            if constexpr (IsTrivialType) {
                Type value = static_cast<const BaseType&>(*this);
                (*FromStr)(value, i_str);
                *this = value;
            } else
                (*FromStr)(*this, i_str);
        }

        Str to() const override final {
            if (!ToStr)
                // Think-Over-Point: maybe there is better to raise an exception
                return Str{};
            if constexpr (IsTrivialType) {
                Type value = static_cast<const BaseType&>(*this);
                return (*ToStr)(value);
            } else
                return (*ToStr)(*this);
        }
#pragma endregion
    };


#pragma region Aliases for buit-in types
    namespace {
        template<typename InnerType>
        void ToInnerTypeFromStr(InnerType& i_value, Str::CR i_str) {
            (std::stringstream{} << i_str) >> i_value;
        }

        template<typename InnerType>
        Str ToStrFromInnerType(const InnerType& i_value) {
            return (std::stringstream{} << i_value).str();
        }

        template<typename InnerType>
        using AliasForBuitInType = AliasFor<InnerType, &ToInnerTypeFromStr<InnerType>, &ToStrFromInnerType<InnerType>>;
    } // namespace

    // NOTE: please add here aliases for built-in types when they will be needed
    using Int = AliasForBuitInType<int>;
    using Double = AliasForBuitInType<double>;
#pragma endregion

    // TODO: there is need to implement a common engine of to/from-Str-conversion for the each type below
    using Path = AliasFor<fs::path>;
    using VStrs = AliasFor<std::vector<Str>>;
    using LStrs = AliasFor<std::list<Str>>;
    using StrToStr = AliasFor<std::map<Str, Str>>;
    using StrToInt = AliasFor<std::map<Str, int>>;
    using IntToStr = AliasFor<std::map<int, Str>>;
} // namespace TB_NS
