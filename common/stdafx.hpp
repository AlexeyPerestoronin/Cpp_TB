#include <string>
#include <istream>
#include <ostream>
#include <fstream>
#include <iostream>

#include <algorithm>

#include <map>

#include <exception>

#include <filesystem>
namespace fs = std::filesystem;
#define TB_GET_CURRENT_PATH fs::path(__FILE__).parent_path()

#include <memory>
#include <optional>

// clang-format off
// PRS - Pointer Reference SmartPointer
#define PRS(DataType) \
    using Opt   = typename std::optional<typename DataType>; \
    using Ptr   = typename DataType*; \
    using CPtr  = typename const DataType*; \
    using PtrC  = typename DataType* const; \
    using CPtrC = typename const DataType* const; \
    using Ref   = typename DataType&; \
    using CRef  = typename const DataType&; \
    using UPtr  = typename std::unique_ptr<typename DataType>; \
    using SPtr  = typename std::shared_ptr<typename DataType>; \
    using WPtr  = typename std::weak_ptr<typename DataType> // there is need add ';' at the end of macro

#define PublicPRS(DataType) \
    public: \
    PRS(DataType)

#define PrivatePRS(DataType) \
    private: \
    PRS(DataType)

#define ProtectedPRS(DataType) \
    protected: \
    PRS(DataType)

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
} // namespace TB_NS
