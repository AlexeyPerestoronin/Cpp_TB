// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

// There is the collection of aliases for the TB

#pragma once

#include <common/json.hpp>

namespace TB_NS {
    namespace {
        template<typename Type>
        class BuiltInType {
            Type m_value{};

            public:
            BuiltInType() noexcept = default;

            template<typename FromType>
            BuiltInType(FromType i_value) noexcept {
                if constexpr (std::is_same_v<FromType, BuiltInType>)
                    m_value = i_value.m_value;
                else if constexpr (std::is_same_v<FromType, Type>)
                    m_value = i_value;
                else if constexpr ((std::is_enum_v<FromType> && std::is_integral_v<Type>) || std::is_convertible_v<FromType, Type>)
                    m_value = static_cast<Type>(i_value);
                else
                    static_assert(std::false_type::value, "unsupported conversion");
            }

            Type& value() noexcept {
                return m_value;
            }

            const Type& value() const noexcept {
                return m_value;
            }

#pragma region operator(s)
            BuiltInType& operator=(const BuiltInType&) noexcept = default;

            template<typename FromType>
            BuiltInType& operator=(FromType i_value) noexcept {
                if constexpr (std::is_same_v<FromType, Type>)
                    m_value = i_value;
                else if constexpr (std::is_enum_v<FromType> && std::is_integral_v<Type>)
                    m_value = static_cast<Type>(i_value);
                else
                    static_assert(std::false_type::value, "unsupported conversion");
                return *this;
            }

            template<typename ToType>
            operator ToType() const noexcept {
                if constexpr (std::is_same_v<ToType, Type>)
                    return m_value;
                else if constexpr (std::is_enum_v<Type> && std::is_integral_v<ToType>)
                    return static_cast<ToType>(m_value);
                else
                    static_assert(std::false_type::value, "unsupported conversion");
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

#define DEFINE_OPERATOR_1(opr)                                    \
    BuiltInType& operator opr(Type i_value) noexcept {            \
        m_value opr i_value;                                      \
        return *this;                                             \
    }                                                             \
                                                                  \
    BuiltInType& operator opr(const BuiltInType& i_bt) noexcept { \
        m_value opr i_bt.m_value;                                 \
        return *this;                                             \
    }

            DEFINE_OPERATOR_1(+=)
            DEFINE_OPERATOR_1(-=)
            DEFINE_OPERATOR_1(*=)
            DEFINE_OPERATOR_1(/=)

#define DEFINE_OPERATOR_2(opr)                                         \
    BuiltInType operator opr(Type i_value) const noexcept {            \
        return m_value opr i_value;                                    \
    }                                                                  \
                                                                       \
    BuiltInType operator opr(const BuiltInType& i_bt) const noexcept { \
        return m_value opr i_bt.m_value;                               \
    }

            DEFINE_OPERATOR_2(+)
            DEFINE_OPERATOR_2(-)
            DEFINE_OPERATOR_2(*)
            DEFINE_OPERATOR_2(/)

#define DEFINE_OPERATOR_3(opr)                                  \
    bool operator opr(Type i_value) const noexcept {            \
        return m_value opr i_value;                             \
    }                                                           \
                                                                \
    bool operator opr(const BuiltInType& i_bt) const noexcept { \
        return m_value opr i_bt.m_value;                        \
    }

            DEFINE_OPERATOR_3(>)
            DEFINE_OPERATOR_3(<)
            DEFINE_OPERATOR_3(>=)
            DEFINE_OPERATOR_3(<=)

            template<typename WithType>
            bool operator==(WithType i_value) const noexcept {
                if constexpr (std::is_same_v<WithType, BuiltInType>)
                    return m_value == i_value.m_value;
                else if constexpr (std::is_same_v<WithType, Type>)
                    return m_value == i_value;
                else if constexpr (std::is_enum_v<WithType> && std::is_integral_v<Type>)
                    return m_value == static_cast<Type>(i_value);
                else
                    static_assert(std::false_type::value, "unsupported conversion");
            }

#undef DEFINE_OPERATOR_3
#undef DEFINE_OPERATOR_2
#undef DEFINE_OPERATOR_1
#pragma endregion
        };
    } // namespace

    // brief: creates the usage-friendly alias for the target type
    // param-t: Type - target type for which the alias should be built
    // param-t: FromStr - pointer to a function converts a value of Str-type to the Type
    // param-t: ToStr - pointer to a function converts a value of the Type to Str-type
    // param-t: FromJson - pointer to a function converts a value of json-object to the Type
    // param-t: ToJson - pointer to a function converts a value of the Type to json-object
    // clang-format off
    template<
        typename Type,
        bool (*FromStr)(Type&, Str::CR) noexcept = nullptr,
        Str (*ToStr)(const Type&) noexcept = nullptr,
        bool (*FromJson)(Type&, Json::CR) noexcept = nullptr,
        Json (*ToJson)(const Type&) noexcept = nullptr
    >
    // clang-format on
    struct AliasFor
        : StrI
        , JsonI
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
        bool fromStr(Str::CR i_str) noexcept override final {
            if (!FromStr)
                return false;

            if constexpr (IsTrivialType)
                return (*FromStr)(BaseType::value(), i_str);
            else
                return (*FromStr)(*this, i_str);
        }

        Str toStr() const noexcept override final {
            if (!ToStr)
                return Str{ States_NS::States::MANGLED };

            if constexpr (IsTrivialType)
                return (*ToStr)(BaseType::value());
            else
                return (*ToStr)(*this);
        }
#pragma endregion

#pragma region JsonI
        bool fromJson(Json::CR i_json) noexcept override final {
            if (!FromJson)
                return false;

            if constexpr (IsTrivialType)
                return (*FromJson)(BaseType::value(), i_json);
            else
                return (*FromJson)(*this, i_json);
        }

        Json toJson() const noexcept override final {
            if (!ToJson)
                return Json{ States_NS::States::MANGLED };

            if constexpr (IsTrivialType)
                return (*ToJson)(BaseType::value());
            else
                return (*ToJson)(*this);
        }
#pragma endregion
    };

#pragma region Aliases for buit - in types
    namespace {
        template<typename InnerType>
        bool ToInnerTypeFromStr(InnerType& i_value, Str::CR i_str) noexcept {
            try {
                (std::stringstream{} << i_str) >> i_value;
                return true;
            } catch (...) {
                return false;
            }
        }

        template<typename InnerType>
        Str ToStrFromInnerType(const InnerType& i_value) noexcept {
            return (std::stringstream{} << i_value).str();
        }

        template<typename InnerType>
        bool ToInnerTypeFromJson(InnerType& i_value, Json::CR i_json) noexcept {
            try {
                i_value = static_cast<InnerType>(i_json);
                return true;
            } catch (...) {
                return false;
            }
        }

        template<typename InnerType>
        Json ToJsonFromInnerType(const InnerType& i_value) noexcept {
            return Json{ i_value }[0];
        }

        template<typename InnerType>
        using AliasForBuitInType = AliasFor<
            InnerType,
            &ToInnerTypeFromStr<InnerType>,
            &ToStrFromInnerType<InnerType>,
            &ToInnerTypeFromJson<InnerType>,
            &ToJsonFromInnerType<InnerType>>;
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
    using StrToInt = AliasFor<std::map<Str, Int>>;
    using IntToStr = AliasFor<std::map<Int, Str>>;
} // namespace TB_NS
