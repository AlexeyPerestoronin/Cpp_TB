// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

// It is the precompiled header file

#pragma once
#include <type_traits>

#define TB_EXPORT __declspec(dllexport)
#define TB_IMPORT __declspec(dllimport)

#define TB_MAYBE_UNUSED [[maybe_unused]]
#define TB_NODISCARD [[nodiscard]]

// clang-format off
#define TB_CONST_CAST(value) \
    const_cast<std::conditional_t< \
        std::is_pointer_v<decltype(value)>, \
            std::add_pointer_t<std::remove_const_t<std::remove_pointer_t<decltype(value)>>>, \
            std::add_lvalue_reference_t<std::remove_const_t<decltype(value)>>>>(value)
// clang-format on

#include <string>
using namespace std::string_literals;

#include <istream>
#include <ostream>
#include <fstream>
#include <iostream>

#include <ctime>

#include <algorithm>
#include <functional>

#include <map>
#include <vector>

#include <variant>

#include <exception>
#include <stdexcept>

#include <filesystem>
namespace fs = std::filesystem;
#define TB_GET_CURRENT_PATH fs::path(__FILE__).parent_path()

#include <memory>
#include <optional>

#include <mutex>

// clang-format off
// PRS - Pointer Reference SmartPointer
#define TB_PRS(DataType, ...) \
    using This = DataType, __VA_ARGS__; \
    /* reference */ \
        using CR   = const This&; \
    /* pointer */ \
        using CP   = const This*; \
        using PC   = This* const; \
        using CPC  = const This* const; \
    /* smart pointer */ \
        using UP   = std::unique_ptr<This>; \
        using SP   = std::shared_ptr<This>; \
        using WP   = std::weak_ptr<This>; \
        template<class... Args> \
        /* creators */ \
            static This New(Args&&... args) { \
                static_assert(std::is_constructible_v<This, Args&&...>, "the " #DataType #__VA_ARGS__"-type cannot be build from Args..."); \
                return This(std::forward<Args>(args)...); \
            } \
            template<class... Args> \
            static This* NewP(Args&&... args) { \
                static_assert(std::is_constructible_v<This, Args&&...>, "the pointer to the " #DataType #__VA_ARGS__"-type cannot be build from Args..."); \
                return new This(std::forward<Args>(args)...); \
            } \
            template<class... Args> \
            static This::SP NewSP(Args&&... args) { \
                static_assert(std::is_constructible_v<This, Args&&...>, "the shared-ptr to the " #DataType #__VA_ARGS__"-type cannot be build from Args..."); \
                return std::make_shared<This>(std::forward<Args>(args)...); \
            } \
            template<class... Args> \
            static This::UP NewUP(Args&&... args) { \
                static_assert(std::is_constructible_v<This, Args&&...>, "the unique-ptr to the " #DataType #__VA_ARGS__"-type cannot be build from Args..."); \
                return std::make_unique<This>(std::forward<Args>(args)...); \
            } \
    /* optional */ \
        using O    = std::optional<This>; \
        using RO   = std::optional<This>&; \
        using CRO  = const std::optional<This>&; \
    /* list */ \
        using L  = std::list<This>; \
        using RL  = std::list<This>&; \
        using CRL  = const std::list<This>&; \
        /* --- */ \
        using Lp  = std::list<This*>; \
        using RLp  = std::list<This*>&; \
        using CRLp  = const std::list<This*>&; \
        /* --- */ \
        using Lup  = std::list<UP>; \
        using RLup  = std::list<UP>&; \
        using CRLup  = const std::list<UP>&; \
        /* --- */ \
        using Lsp  = std::list<SP>; \
        using RLsp  = std::list<SP>&; \
        using CRLsp  = const std::list<SP>&; \
        /* --- */ \
        using Lwp  = std::list<WP>; \
        using RLwp  = std::list<WP>&; \
        using CRLwp  = const std::list<WP>&; \
    /* vector */ \
        using V  = std::vector<This>; \
        using RV  = std::vector<This>&; \
        using CRV  = const std::vector<This>&; \
        /* --- */ \
        using Vp  = std::vector<This*>; \
        using RVp  = std::vector<This*>&; \
        using CRVp  = const std::vector<This*>&; \
        /* --- */ \
        using Vup  = std::vector<UP>; \
        using RVup  = std::vector<UP>&; \
        using CRVup  = const std::vector<UP>&; \
        /* --- */ \
        using Vsp  = std::vector<SP>; \
        using RVsp  = std::vector<SP>&; \
        using CRVsp  = const std::vector<SP>&; \
        /* --- */ \
        using Vwp  = std::vector<WP>; \
        using RVwp  = std::vector<WP>&; \
        using CRVwp  = const std::vector<WP>& // there is need add ';' at the end of macro

#define TB_PUBLIC_PRS(DataType, ...) \
    public: \
    TB_PRS(DataType, __VA_ARGS__)

#define TB_PRIVATE_PRS(DataType, ...) \
    private: \
    TB_PRS(DataType, __VA_ARGS__)

#define TB_PROTECTED_PRS(DataType, ...) \
    protected: \
    TB_PRS(DataType, __VA_ARGS__)

#define TB_EXTENDED_USING(Aliase, Base, ...) \
    struct Aliase : Base , __VA_ARGS__ { \
        TB_PRS(Aliase); \
        using BaseType = Base, __VA_ARGS__; \
        using BaseType::BaseType; \
        Aliase(BaseType&& i_base) noexcept \
            : BaseType(std::move(i_base)){}; \
        Aliase(const BaseType& i_base) \
            : BaseType(i_base){}; \
    } // there is need add ';' at the end of macro
// clang-format on
