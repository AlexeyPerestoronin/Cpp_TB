// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../initializer.hpp"

namespace TB_NS::Exmo_API {
    namespace {
        bool isExmoApiModuleLoad{ false };
    }

    bool IsModuleLoad() {
        return isExmoApiModuleLoad;
    }

    void LoadModule() {
        if (!isExmoApiModuleLoad) {
            Error_NS::Exceptions::GetIns().LoadSettings("exmo_api.exception.json");
            isExmoApiModuleLoad = true;
        }
    }
} // namespace TB_NS::Exmo_API
