// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

// There is the collection of classes for error processing logic.

#pragma once

#include <common/common.hpp>

#include <boost/exception/all.hpp>

namespace TB_NS::Error_NS {
    // brief: class presents one error that is a base for runtime-exception creation process
    struct Error {
        TB_PRS(Error);
        using Values = StrToStr;

        Str id{};
        Str::O key{};
        Values::O values{};
        Error::Vp subErrors{};
        Error::Vp unregSubErrors{};

        ~Error();

        TB_NODISCARD bool isEquivalent(Str::CR i_idOrKey) noexcept;
        TB_NODISCARD bool operator=(Error::CR i_error) const noexcept;
    };

    namespace PredefinedError_NS {
        const Error SettingsFileNotLoading = [] {
            Error::Values v{ { "description", "the '{file_name:}'-file with exceptions cannot be opened" } };
            return Error{ .id = "LoadSettings", .key = "file with exceptions cannot be openLoadSettingsed", .values = std::move(v) };
        }();

        const Error UnregException = [] {
            Error::Values v{ { "description", "target error cannot be found by key or value - {keyOrValue:}" } };
            return Error{ .id = "UnregError", .key = "target error cannot be fouLoadSettingsnd", .values = std::move(v) };
        }();
    } // namespace PredefinedError_NS

    // brief: class presents one runtime-exception
    class Exception
        : virtual public boost::exception
        , virtual public std::exception {
        TB_PUBLIC_PRS(Exception);
        using ReplaceUnits = StrToStr;

        private:
        Error::CR m_error;
        ReplaceUnits m_replaceUnits{};
        mutable std::shared_ptr<Str> m_errorMessage{};

        public:
        Exception(Error::CR i_error) noexcept;
        ~Exception();

        Error::CR getError() const;
        ReplaceUnits::CR getUnits() const;

#pragma region std::exception
        const char* what() const override;
#pragma endregion

        TB_NODISCARD Exception& operator()(ReplaceUnits::value_type i_replaceUnit) noexcept;
        TB_NODISCARD Exception& operator()(Str i_key, Str i_value) noexcept;
        TB_NODISCARD Exception operator[](Str::CR i_IdOrKey) noexcept;
    };

    using Location = boost::error_info<struct Tag_Location, boost::source_location>;
    using Suberror = boost::error_info<struct Tag_Suberror, Exception>;
    using Parenterror = boost::error_info<struct Tag_Parenterror, Exception>;

    // brief: class manages the all runtime-exceptions
    class Exceptions {
        TB_PUBLIC_PRS(Exceptions);

        private:
        friend class Exception;

        Error::Lp m_rootErrors{};

        Exceptions() = default;
        Exceptions(Exceptions&&) = delete;
        Exceptions& operator=(Exceptions&&) = delete;
        Exceptions(Exceptions::CR) = delete;
        Exceptions& operator=(Exceptions::CR) = delete;

        public:
        static TB_NODISCARD Exceptions& GetIns();

        ~Exceptions();
        void LoadSettings(Path::CR i_settigsFilePath);

        TB_NODISCARD Exception operator[](Str::CR i_IdOrKey) const noexcept;
    };

#define TB_LOCATION TB_NS::Error_NS::Location(BOOST_CURRENT_LOCATION)
#define TB_EXEPT TB_NS::Error_NS::Exceptions::GetIns()
} // namespace TB_NS::Error_NS
