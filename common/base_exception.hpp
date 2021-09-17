#pragma once

#include "ptr.hpp"

#include <exception>
#include <boost/exception/all.hpp>

namespace TB_NS {
    class Exception
        : virtual public boost::exception
        , virtual public std::exception {
        PtrCls(Exception);
    	
        std::string d_message{};
        boost::source_location d_location{};
        Exception::UniPtr d_suberror{};

    public:
        Exception() = default;
        Exception(Exception&&) = default;
        Exception(const Exception&) = default;
        Exception(std::string i_message, boost::source_location&& i_location);
        Exception(std::string i_message, boost::source_location&& i_location, Exception& io_suberror);
        ~Exception() = default;

    	const char* what() const override;
    };    
} // namespace TB_NS
