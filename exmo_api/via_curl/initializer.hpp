// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

// Exmo-API Module Entry Point

#pragma once

#include <common/base_exception.hpp>

namespace TB_NS::Exmo_API {
    // brief: flag shows isn't invoked 'LoadModule'-function
    bool IsModuleLoad();

    // brief: loads Exmo-API module
    void LoadModule();
} // namespace TB_NS::Exmo_API
