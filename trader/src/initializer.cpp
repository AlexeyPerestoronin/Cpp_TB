// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include "../initializer.hpp"
#include <common/base_exception.hpp>

namespace TB_NS::Trader_NS {
    namespace {
        bool isTraderModuleLoad{ false };
    }

    bool IsModuleLoad() {
        return isTraderModuleLoad;
    }

    void LoadModule() {
        if (!isTraderModuleLoad) {
            Error_NS::Exceptions::GetIns().LoadSettings("trader.exception.json");
            isTraderModuleLoad = true;
        }
    }
} // namespace TB_NS::Trader_NS
