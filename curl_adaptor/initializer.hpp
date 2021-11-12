// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

// CURL-Adapter Module Entry Point

#pragma once

#include <common/base_exception.hpp>

namespace TB_NS::CurlAdapter_NS {
    // brief: flag shows isn't invoked 'LoadModule'-function
    bool IsModuleLoad();

    // brief: loads CURL-Adapter Module
    void LoadModule();
} // namespace TB_NS::CurlAdapter_NS
