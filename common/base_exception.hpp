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
        Exception::CP d_rootException{};   // it is the link to the exception from what this exception was founded
        Exception::CP d_parentException{}; // it is the link to the exception that is the parent in the exceptions' hierarchy
        std::list<Exception::P> d_subException{};
        mutable std::shared_ptr<Str> d_errorMessage{};

        public:
        // TODO: default constructor should be removed
        Exception() = default;
        Exception(Str i_id, Str i_key, Values i_values = Values{}) noexcept;
        bool isSatisfy(Str::CR i_IdOrKey) const noexcept;

#pragma region std::exception
        const char* what() const override;
#pragma endregion

        Exception::CR operator[](Str::CR i_IdOrKey) const noexcept;
    };

    using Location = boost::error_info<struct Tag_Location, boost::source_location>;
    using Suberror = boost::error_info<struct Tag_Suberror, Exception>;

    class Exceptions {
        PublicPRS(Exceptions);

        private:
        friend class Exception;

        Exceptions() = default;
        Exceptions(Exceptions&&) = delete;
        Exceptions(Exceptions::CR) = delete;
        ~Exceptions();

#pragma region static members / methods
        static Exception::PC d_rootException;
        static std::list<Exception::SP> d_allExceptions;
        static std::list<Exception::SP> d_unregistedExceptions;

        public:
        static Exceptions::C Ins; // the lone instance of this class
        static Exception::R RegistUnknowException(Str::CR i_id, Str::CR i_key, Exception::Values::CRO i_values = std::nullopt);
        static void LoadSettings(Path::CR i_settigsFilePath);
#pragma endregion

        const Exception& operator[](Str::CR i_IdOrKey) const noexcept;
    };
} // namespace TB_NS::Error_NS
