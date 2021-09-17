#include "../base_exception.hpp"

namespace TB_NS {
    const char* Exception::what() const {
        std::stringstream message;

        for (Exception const* suberror = this; suberror; suberror = boost::get_error_info<Suberror>(*suberror)) {
            message << "***" << std::endl;

            message << "[location] ";
            if (boost::source_location const* location = boost::get_error_info<Location>(*suberror); location)
                message << *location << std::endl;
            else
                message << "no info" << std::endl;
            
            message << "[description] ";
            if (std::string const* description = boost::get_error_info<Description>(*suberror); description)
                message << *description << std::endl;
            else
                message << "no info" << std::endl;

            message << "***" << std::endl;
        }

        return d_errorMessage.assign(message.str()).data();
    }

} // namespace TB_NS
