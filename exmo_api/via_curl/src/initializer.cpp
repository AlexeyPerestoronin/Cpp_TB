// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../initializer.hpp"

namespace TB_NS::Exmo_API {
    namespace {
        bool isExmoApiModuleLoad{ false };
    }

    bool IsModuleLoad() {
        return isExmoApiModuleLoad;
    }

    void LoadModule(Error_NS::Exceptions& io_exceptionIns) {
        if (!isExmoApiModuleLoad) {
            Str exmoExceptinFile{ "exmo_api.exception.json" };
            io_exceptionIns.LoadSettings(exmoExceptinFile.data(), exmoExceptinFile.size());
            isExmoApiModuleLoad = true;
        }
    }
} // namespace TB_NS::Exmo_API
