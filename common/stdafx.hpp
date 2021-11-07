#pragma once
#include <type_traits>

#define TB_EXPORT __declspec(dllexport)
#define TB_IMPORT __declspec(dllimport)

#define TB_MAYBE_UNUSED [[maybe_unused]]
#define TB_NODISCARD [[nodiscard]]

#define TB_CONST_CAST(value)                                                                                                 \
    const_cast<std::conditional_t<                                                                                           \
        std::is_pointer_v<decltype(value)>, std::add_pointer_t<std::remove_const_t<std::remove_pointer_t<decltype(value)>>>, \
        std::remove_const_t<decltype(value)>>>(value)

#include <string>
#include <istream>
#include <ostream>
#include <fstream>
#include <iostream>

#include <ctime>

#include <algorithm>
#include <functional>

#include <map>
#include <vector>

#include <exception>
#include <stdexcept>

#include <filesystem>
namespace fs = std::filesystem;
#define TB_GET_CURRENT_PATH fs::path(__FILE__).parent_path()

#include <memory>
#include <optional>

// clang-format off
// PRS - Pointer Reference SmartPointer
#define TB_PRS(DataType, ...) \
    using This = DataType, __VA_ARGS__; \
    using O    = std::optional<This>; \
    using RO   = std::optional<This>&; \
    using CRO  = const std::optional<This>&; \
    using CP   = const This*; \
    using PC   = This* const; \
    using CPC  = const This* const; \
    using CR   = const This&; \
    using UP   = std::unique_ptr<This>; \
    using SP   = std::shared_ptr<This>; \
    using WP   = std::weak_ptr<This> // there is need add ';' at the end of macro

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

namespace TB_NS {
    TB_EXTENDED_USING(Str, std::string);
    TB_EXTENDED_USING(IntToInt, std::map<int, int>);
    TB_EXTENDED_USING(StrView, std::string_view);
    TB_EXTENDED_USING(Path, fs::path);
} // namespace TB_NS
