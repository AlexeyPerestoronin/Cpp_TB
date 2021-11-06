// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

// Exmo-API Module Entry Point

#pragma once

#include <common/base_exception.hpp>

namespace TB_NS::Exmo_API {
    // brief: flag shows isn't invoked target function
    bool IsModuleLoad();

    // brief: loads Exmo-API module
    // param: io_exceptionIns - entry point to load module's exceptions
    void LoadModule(Error_NS::Exceptions& io_exceptionIns);
} // namespace TB_NS::Exmo_API
