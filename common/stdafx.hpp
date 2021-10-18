#include <string>
#include <istream>
#include <ostream>
#include <fstream>
#include <iostream>

#include <algorithm>

#include <exception>

#include <filesystem>
namespace fs = std::filesystem;
#define TB_GET_CURRENT_PATH fs::path(__FILE__).parent_path()

#include <memory>
#include <optional>
#define Ptr(DataType)                                           \
    using Opt = typename std::optional<typename DataType>;      \
    using Ptr = typename DataType*;                             \
    using CPtr = typename const DataType*;                      \
    using PtrC = typename DataType* const;                      \
    using CPtrC = typename const DataType* const;               \
    using Ref = typename DataType&;                             \
    using CRef = typename const DataType&;                      \
    using UniPtr = typename std::unique_ptr<typename DataType>; \
    using ShaPtr = typename std::shared_ptr<typename DataType>; \
    using WekPtr = typename std::weak_ptr<typename DataType> // there is need add ';' at the end of macro

#define PublicPtr(DataType) \
    public:                 \
    Ptr(DataType)

#define PrivatePtr(DataType) \
    public:                  \
    Ptr(DataType)

#define ProtectedPtr(DataType) \
    public:                    \
    Ptr(DataType)

#define ExtentedUsing(Aliase, Base)          \
    struct Aliase : Base {                   \
        PublicPtr(Aliase);                   \
        using BaseType = Base;               \
        using BaseType::BaseType;            \
        Aliase(BaseType&& i_base) noexcept   \
            : BaseType(std::move(i_base)){}; \
        Aliase(const BaseType& i_base)       \
            : BaseType(i_base){};            \
    } // there is need add ';' at the end of macro

namespace TB_NS {
    ExtentedUsing(Str, std::string);
    ExtentedUsing(StrView, std::string_view);
} // namespace TB_NS
