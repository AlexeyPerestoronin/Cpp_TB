#pragma once

#include "ptr.hpp"

#include <exception>
#include <nlohmann/json.hpp>
#include <boost/exception/all.hpp>

namespace TB_NS::Error_NS {
    class Exception
        : virtual public boost::exception
        , virtual public std::exception {
        PtrCls(Exception);

        mutable std::string d_errorMessage{};

        public: // std::exception
        const char* what() const override;
    };

    using Location = boost::error_info<struct Tag_Location, boost::source_location>;
    using Description = boost::error_info<struct Tag_Description, std::string>;
    using Suberror = boost::error_info<struct Tag_Suberror, Exception>;
} // namespace TB_NS::Error_NS
