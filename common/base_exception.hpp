// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

// There is the collection of classes for error processing logic.

#pragma once

#include <common/stdafx.hpp>
#include <boost/exception/all.hpp>

namespace TB_NS::Error_NS {
    class Exceptions;

    class Exception
        : virtual public boost::exception
        , virtual public std::exception {
        PublicPRS(Exception);

        private:
        friend class Exceptions;

        ExtendedUsing(Values, std::map<Str, Str>);

        Str d_id;
        Str d_key;
        Values d_values;
        Exception::CPtr d_rootException{};   // it is the link to the exception from what this exception was founded
        Exception::CPtr d_parentException{}; // it is the link to the exception that is the parent in the exceptions' hierarchy
        std::list<Exception::Ptr> d_subException{};
        mutable std::shared_ptr<Str> d_errorMessage{};

        public:
        // TODO: default constructor should be removed
        Exception() = default;
        Exception(Str i_id, Str i_key, Values i_values = Values{}) noexcept;
        bool isSatisfy(Str::CRef i_IdOrKey) const noexcept;

#pragma region std::exception
        const char* what() const override;
#pragma endregion

        Exception::CRef operator[](Str::CRef i_IdOrKey) const noexcept;
    };

    using Location = boost::error_info<struct Tag_Location, boost::source_location>;
    using Suberror = boost::error_info<struct Tag_Suberror, Exception>;

    class Exceptions {
        PublicPRS(Exceptions);

        private:
        friend class Exception;

        Exceptions() = default;
        Exceptions(Exceptions&&) = delete;
        Exceptions(Exceptions::CRef) = delete;
        ~Exceptions();

#pragma region static members / methods
        static Exception::PtrC d_rootException;
        static std::list<Exception::SPtr> d_allExceptions;
        static std::list<Exception::SPtr> d_unregistedExceptions;

        public:
        static Exceptions::C Ins; // the lone instance of this class
        static Exception::Ref RegistUnknowException(Str::CRef i_id, Str::CRef i_key, Exception::Values::CROpt i_values = std::nullopt);
        static void LoadSettings(Path::CRef i_settigsFilePath);
#pragma endregion

        const Exception& operator[](Str::CRef i_IdOrKey) const noexcept;
    };
} // namespace TB_NS::Error_NS
