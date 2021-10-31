#pragma once

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
#define PRS(DataType, ...) \
    using This = DataType, __VA_ARGS__; \
    using O    = std::optional<This>; \
    using RO   = std::optional<This>&; \
    using CRO  = const std::optional<This>&; \
    using P    = This*; \
    using CP   = const This*; \
    using PC   = This* const; \
    using CPC  = const This* const; \
    using C    = const This; \
    using R    = This&; \
    using CR   = const This&; \
    using UP   = std::unique_ptr<This>; \
    using SP   = std::shared_ptr<This>; \
    using WP   = std::weak_ptr<This> // there is need add ';' at the end of macro

#define PublicPRS(DataType, ...) \
    public: \
    PRS(DataType, __VA_ARGS__)

#define PrivatePRS(DataType, ...) \
    private: \
    PRS(DataType, __VA_ARGS__)

#define ProtectedPRS(DataType, ...) \
    protected: \
    PRS(DataType, __VA_ARGS__)

#define ExtendedUsing(Aliase, Base, ...) \
    struct Aliase : Base , __VA_ARGS__ { \
        PRS(Aliase); \
        using BaseType = Base, __VA_ARGS__; \
        using BaseType::BaseType; \
        Aliase(BaseType&& i_base) noexcept \
            : BaseType(std::move(i_base)){}; \
        Aliase(const BaseType& i_base) \
            : BaseType(i_base){}; \
    } // there is need add ';' at the end of macro
// clang-format on

namespace TB_NS {
    ExtendedUsing(Str, std::string);
    ExtendedUsing(IntToInt, std::map<int, int>);
    ExtendedUsing(StrView, std::string_view);
    ExtendedUsing(Path, fs::path);
} // namespace TB_NS
