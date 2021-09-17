#pragma once

#include <curl/curl.h>

#include "base_exception.hpp"

#include <boost/exception/all.hpp>
#include <boost/exception/exception.hpp>

namespace TB_NS {
    class CurlEasy {
        static CURL* ch;

        public:
        CurlEasy() = default;

        ~CurlEasy();

        static CURL* get_instance();
    };

    inline CURLM* d_curlm = []() {
        CURLM* r_curlm = curl_multi_init();
        /*if (!r_curlm)
            throw CurlException{} << CurlExceptionInfo{ "global instance of CURLM cannot be create" };*/
        return r_curlm;
    }();
    ;

    CurlEasy MakeCurlEasy();
} // namespace TB_NS
