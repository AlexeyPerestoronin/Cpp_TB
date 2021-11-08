// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#pragma once

#include "../_curl_object.hpp"

#include <common/base_exception.hpp>

namespace TB_NS::CURL_NS {
    CURL* EntryPoint::m_curlPtr = nullptr;

    EntryPoint::~EntryPoint() {
        if (m_curlPtr) {
            curl_easy_cleanup(m_curlPtr);
            m_curlPtr = nullptr;
        }
    }

    CURL* EntryPoint::GetIns() {
        if (!m_curlPtr) {
            m_curlPtr = curl_easy_init();
            if (!m_curlPtr)
                throw TB_EXEPT["CURL"]["easy_init"] << TB_LOCATION;
        }
        return m_curlPtr;
    }
} // namespace TB_NS::CURL_NS
