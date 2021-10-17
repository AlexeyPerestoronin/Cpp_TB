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
#define Ptr(StructType)                                           \
    public:                                                       \
    using Opt = typename std::optional<typename StructType>;      \
    using Ptr = typename StructType*;                             \
    using CPtr = typename const StructType*;                      \
    using PtrC = typename StructType* const;                      \
    using CPtrC = typename const StructType* const;               \
    using UniPtr = typename std::unique_ptr<typename StructType>; \
    using ShaPtr = typename std::shared_ptr<typename StructType>; \
    using WekPtr = typename std::weak_ptr<typename StructType>;

#define PtrCls(ClassType) Ptr(ClassType) private:

namespace TB_NS {
    using Str = std::string;
    using StrView = std::string_view;
}
