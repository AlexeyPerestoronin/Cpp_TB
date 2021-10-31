#pragma once

#include "../_curl_object.hpp"

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
        }
        return m_curlPtr;
    }
} // namespace TB_NS::CURL_NS
