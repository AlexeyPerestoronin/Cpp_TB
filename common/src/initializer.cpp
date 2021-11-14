// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../initializer.hpp"

namespace TB_NS {
    namespace {
        bool isCommonModuleLoad{ false };
    }

    bool IsModuleLoad() {
        return isCommonModuleLoad;
    }

    void LoadModule() {
        if (!isCommonModuleLoad) {
            Error_NS::Exceptions::GetIns().LoadSettings("common.exception.json");
            isCommonModuleLoad = true;
        }
    }
} // namespace TB_NS::Common_API
