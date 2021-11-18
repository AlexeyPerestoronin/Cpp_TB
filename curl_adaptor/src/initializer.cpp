// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../initializer.hpp"

namespace TB_NS::CurlAdapter_NS {
    namespace {
        bool isCurlModuleLoad{ false };
    }

    bool IsModuleLoad() {
        return isCurlModuleLoad;
    }

    void LoadModule() {
        if (!isCurlModuleLoad) {
            Error_NS::Exceptions::GetIns().LoadSettings("curl_adaptor.exception.json");
            isCurlModuleLoad = true;
        }
    }
} // namespace TB_NS::CurlAdapter_NS
