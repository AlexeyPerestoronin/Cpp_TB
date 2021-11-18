// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../initializer.hpp"

namespace TB_NS::OpenSslAdapter_NS {
    namespace {
        bool isOpenSslModuleLoad{ false };
    }

    bool IsModuleLoad() {
        return isOpenSslModuleLoad;
    }

    void LoadModule() {
        if (!isOpenSslModuleLoad) {
            Error_NS::Exceptions::GetIns().LoadSettings("openssl_adaptor.exception.json");
            isOpenSslModuleLoad = true;
        }
    }
} // namespace TB_NS::OpenSslAdapter_NS
