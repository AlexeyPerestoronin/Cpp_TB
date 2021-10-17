#pragma once

#include <common/stdafx.hpp>
#include <boost/exception/all.hpp>

namespace TB_NS::Error_NS {
    class Exceptions;
	
    class Exception
        : virtual public boost::exception
        , virtual public std::exception {
        PtrCls(Exception);

        friend class Exceptions;

        using Values = std::map<Str, Str>;

        Str d_id;
        Str d_key;
        Values d_values;
        Exception::CPtr d_rootException{}; // it is the link to the exception from what this exception was founded
        Exception::CPtr d_parentException{}; // it is the link to the exception that is the parent in the exceptions' hierarchy
        std::list<Exception::Ptr> d_subException{};
        mutable std::shared_ptr<Str> d_errorMessage{};

        public:
        // TODO: default constructor should be removed
        Exception() = default;
        Exception(Str i_id, Str i_key, Values i_values = Values{}) noexcept;
        bool isSatisfy(StrView i_IdOrKey) const noexcept;

#pragma region std::exception
        const char* what() const override;
#pragma endregion

    	const Exception& operator[](StrView i_IdOrKey) const noexcept;
    };

    using Location = boost::error_info<struct Tag_Location, boost::source_location>;
    using Description = boost::error_info<struct Tag_Description, Str>;
    using Suberror = boost::error_info<struct Tag_Suberror, Exception>;

    class Exceptions {
        PtrCls(Exceptions);
    	
        friend class Exception;
    	
#pragma region static members / methods
        static Exception::PtrC d_rootException;
        static std::list<Exception::ShaPtr> d_allExceptions;
    
        public:
        static Exceptions Ins;
        static void LoadSettings(const fs::path& i_settigsFilePath);
#pragma endregion

    	const Exception& operator[](StrView i_IdOrKey) const noexcept;
    };
} // namespace TB_NS::Error_NS
