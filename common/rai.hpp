// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
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
        using ThisType = RAI<Type, Category::STREAM>;
        using BaseType = Type;
        using Deleter = std::function<void(BaseType&)>;
        using NoexceptDeleter = void (*)(BaseType&) noexcept;
        PublicPRS(ThisType);

        private:
        Deleter m_delter;

        public:
        RAI(RAI&& i_rai)
        noexcept
            : BaseType(std::move(i_rai)){};

        RAI(BaseType&& i_base)
        noexcept
            : BaseType(std::move(i_base)){};

        RAI(const RAI& i_rai)
        noexcept
            : BaseType(i_rai){};

        RAI(const BaseType& i_base)
        noexcept
            : BaseType(i_base){};

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
        using ThisType = RAI<Type, Category::STREAM>;
        using BaseType = Type;
        PublicPRS(ThisType);

        using BaseType::BaseType;
        RAI(const RAI&) = delete;
        RAI(RAI&& i_rai)
        noexcept
            : BaseType(std::move(i_rai)){};

        RAI(BaseType&& i_stream)
        noexcept
            : BaseType(std::move(i_stream)){};

        ~RAI() noexcept {
            BaseType::close();
        }
    };
} // namespace TB_NS
