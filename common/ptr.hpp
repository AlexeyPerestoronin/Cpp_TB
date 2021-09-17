#pragma once

#include <memory>
#include <optional>

#define Ptr(StructType)                                           \
    public:                                                       \
    using Opt = typename std::optional<typename StructType>;      \
    using Ptr = typename StructType*;                             \
    using UniPtr = typename std::unique_ptr<typename StructType>; \
    using ShaPtr = typename std::shared_ptr<typename StructType>; \
    using WekPtr = typename std::weak_ptr<typename StructType>;

#define PtrCls(ClassType) Ptr(ClassType) private: