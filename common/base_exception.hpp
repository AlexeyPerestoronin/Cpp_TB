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

    // brief: class presents one runtime-exception
    // note1: not accessible outside the common.dll
    class Exception
        : virtual public boost::exception
        , virtual public std::exception {
        friend class Exceptions;

        TB_PUBLIC_PRS(Exception);
        TB_EXTENDED_USING(IdKey, std::pair<Str, Str>);
        TB_EXTENDED_USING(Values, std::map<Str, Str>);
        TB_EXTENDED_USING(IdKeyValues, std::tuple<Str, Str, Values>);

        private:
        Exceptions* const m_homeExceptions; // it is the link to the Exceptions-class instance that is the owner for the it exception
        Str m_id{};
        Str m_key{};
        Values m_values{};
        Exception::CP m_rootException{};   // it is the link to the exception from what this exception was founded
        Exception::CP m_parentException{}; // it is the link to the exception that is the parent in the exceptions' hierarchy
        std::list<Exception*> m_subException{};
        mutable std::shared_ptr<Str> m_errorMessage{};

        public:
        Exception(Exceptions* const i_homeExceptions, IdKeyValues i_idKeyValues) noexcept;
        Exception(Exceptions* const i_homeExceptions, IdKey i_idKey) noexcept;
        Exception(Exceptions* const i_homeExceptions, Str i_id, Str i_key, Values i_values = Values{}) noexcept;

        TB_EXPORT bool isSatisfy(Str::CR i_IdOrKey) const noexcept;

#pragma region std::exception
        TB_EXPORT const char* what() const override;
#pragma endregion

        TB_EXPORT Exception::CR operator[](Str::CR i_IdOrKey) const noexcept;
    };

    using Location = boost::error_info<struct Tag_Location, boost::source_location>;
    using Suberror = boost::error_info<struct Tag_Suberror, Exception>;

    // brief: class manages the all runtime-exceptions
    // note1: not accessible outside the common.dll
    class Exceptions {
        TB_PUBLIC_PRS(Exceptions);

        private:
        friend class Exception;

        Exception::PC m_rootException{};
        std::list<Exception::SP> m_allExceptions{};
        std::list<Exception::SP> m_unregistedExceptions{};

        Exceptions(Exceptions&&) = delete;
        Exceptions& operator=(Exceptions&&) = delete;
        Exceptions(Exceptions::CR) = delete;
        Exceptions& operator=(Exceptions::CR) = delete;

        public:
        Exceptions();
        TB_EXPORT ~Exceptions();
        TB_EXPORT Exception& RegistNewException(Str::CR i_id, Str::CR i_key, Exception::Values::CR i_values = Exception::Values{});
        TB_EXPORT Exception& RegistUnknowException(Str::CR i_id, Str::CR i_key, Exception::Values::CRO i_values = std::nullopt);
        TB_EXPORT void LoadSettings(const char* i_settigsFilePath, size_t i_size);

        TB_EXPORT Exception::CR operator[](Str::CR i_IdOrKey) const noexcept;
    };

    // brief: creates instance of Exceptions-class
    // note1: accessible outside the common.dll
    TB_EXPORT Exceptions CreateExceptionsIns();
} // namespace TB_NS::Error_NS
