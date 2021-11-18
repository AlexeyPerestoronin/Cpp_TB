// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

// OpenSSL-Adapter Module Entry Point

#pragma once

#include <common/base_exception.hpp>

namespace TB_NS::OpenSslAdapter_NS {
    // brief: flag shows isn't invoked 'LoadModule'-function
    bool IsModuleLoad();

    // brief: loads OpenSSL-Adapter Module
    void LoadModule();
} // namespace TB_NS::OpenSslAdapter_NS
