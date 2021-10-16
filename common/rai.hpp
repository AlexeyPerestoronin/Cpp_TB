// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ************************************ //

// Here is the template implementation of RAI techniques.

#pragma once

namespace TB_NS {
    namespace {
        template<class TargetType, class Type, class... Types>
        struct IsSame {
            static constexpr bool value = std::is_same_v<TargetType, Type> || IsSame<TargetType, Types...>::value;
        };

        template<class TargetType, class Type>
        struct IsSame<TargetType, Type> {
            static constexpr bool value = std::is_same_v<TargetType, Type>;
        };

        enum class Category {
            COMMON,
            STREAM,
        };

        template<class Type>
        consteval Category define() {
            if (IsSame<Type, std::ifstream, std::ostream, std::fstream>::value)
                return Category::STREAM;
            return Category::COMMON;
        };
    } // namespace

    template<class Type, Category category = define<Type>()>
    class RAI : public Type {
        using BaseType = Type;
        using Deleter = std::function<void(BaseType&)>;
        using NoexceptDeleter = void (*)(BaseType&) noexcept;

        Deleter m_delter;

        public:
        RAI(RAI&&) noexcept = default;
        RAI(const RAI&) noexcept = default;

        template<std::convertible_to<NoexceptDeleter> DeleterT, class... Args>
        RAI(DeleterT&& i_deleter, Args&&... i_args)
        noexcept
            : BaseType(std::forward<Args>(i_args)...)
            , m_delter(std::move(i_deleter)) {}

        ~RAI() noexcept {
            m_delter(*this);
        }
    };

    template<class Type>
    requires requires(Type stream) {
        stream.close();
    }
    class RAI<Type, Category::STREAM> : public Type {
        using BaseType = Type;

        public:
        RAI(RAI&&) noexcept = default;
        RAI(const RAI&) noexcept = default;

        template<class... Args>
        RAI(Args&&... i_args)
        noexcept
            : BaseType(std::forward<Args>(i_args)...) {}

        ~RAI() noexcept {
            BaseType::close();
        }
    };
} // namespace TB_NS
