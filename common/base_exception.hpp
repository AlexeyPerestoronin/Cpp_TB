#pragma once

#include <common/stdafx.hpp>
#include <boost/exception/all.hpp>

namespace TB_NS::Error_NS {
    class Exception
        : virtual public boost::exception
        , virtual public std::exception {
        PtrCls(Exception);

#pragma region static members / methods
        static std::list<Exception::Ptr> d_rootExceptions;
    public:
        static void LoadSettings(const fs::path& i_settigsFilePath);
#pragma endregion

    private:
        Str d_id;
        Str d_key;
        std::map<Str, Str> d_values;
        std::list<Exception::Ptr> d_subException{};
        mutable std::shared_ptr<Str> d_errorMessage{};

        public:
        // TODO: default constructor should be removed
        Exception() = default;
        Exception(Str i_id, Str i_key) noexcept;

#pragma region std::exception
        const char* what() const override;
#pragma endregion
    };

    using Location = boost::error_info<struct Tag_Location, boost::source_location>;
    using Description = boost::error_info<struct Tag_Description, Str>;
    using Suberror = boost::error_info<struct Tag_Suberror, Exception>;
} // namespace TB_NS::Error_NS
