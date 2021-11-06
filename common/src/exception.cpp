// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include <common/base_exception.hpp>

namespace TB_NS::Error_NS {
    Exception::Exception(Exceptions* const i_homeExceptions, IdKeyValues i_idKeyValues) noexcept
        : m_homeExceptions(i_homeExceptions)
        , m_id(std::move(std::get<0>(i_idKeyValues)))
        , m_key(std::move(std::get<1>(i_idKeyValues)))
        , m_values(std::move(std::get<2>(i_idKeyValues))) {}

    Exception::Exception(Exceptions* const i_homeExceptions, IdKey i_idKey) noexcept
        : m_homeExceptions(i_homeExceptions)
        , m_id(std::move(i_idKey.first))
        , m_key(std::move(i_idKey.second)) {}

    Exception::Exception(Exceptions* const i_homeExceptions, Str i_id, Str i_key, Values i_values) noexcept
        : m_homeExceptions(i_homeExceptions)
        , m_id(std::move(i_id))
        , m_key(std::move(i_key))
        , m_values(std::move(i_values)) {}

    bool Exception::isSatisfy(Str::CR i_IdOrKey) const noexcept {
        return m_id == i_IdOrKey || m_key == i_IdOrKey;
    }

    const char* Exception::what() const {
        using namespace boost;

        std::stringstream message;

        auto printer = [&message](size_t offset, auto... i_message) {
            message << Str(offset, ' ');
            ((message << i_message), ...);
            message << std::endl;
        };

        for (auto [suberror, offcet] = std::make_tuple(this, size_t{ 0 }); suberror; suberror = get_error_info<Suberror>(*suberror), offcet += 3) {
            auto print = [&printer, &offcet](auto... i_message) { printer(offcet, i_message...); };
            if (auto* locationP = get_error_info<Location>(*suberror); locationP)
                print("location: ", *locationP);
            print("id: ", suberror->m_id);
            print("key: ", suberror->m_key);
            for (size_t counter{ 0 }; const auto& [key, value] : suberror->m_values)
                print(++counter, ") ", key, ": ", value);
            print("suberror:");
        }
        m_errorMessage = std::make_shared<Str>(message.str());
        return m_errorMessage->data();
    }

    Exception::CR Exception::operator[](Str::CR i_IdOrKey) const noexcept {
        const auto finder = [&i_IdOrKey](const auto& exceptions) -> auto {
            const auto comparator = [&i_IdOrKey](auto exceptionP) -> bool { return exceptionP->isSatisfy(i_IdOrKey); };
            if (const auto exception = std::ranges::find_if(exceptions, comparator); exception != exceptions.end())
                return *exception;
            else
                return decltype(*exception){};
        };

        // try to search through  the sub exceptions list
        if (auto exceptionP = finder(m_subException); exceptionP) {
            exceptionP->m_rootException = nullptr;
            return *exceptionP;
        }

        // try to search through  the sub exceptions list
        if (auto exceptionP = finder(m_homeExceptions->m_allExceptions); exceptionP) {
            exceptionP->m_rootException = this;
            return *exceptionP;
        }

        // if no one known exceptions wasn't be founded then creates new exception, links it with current root-exception and returns
        auto& newException = m_homeExceptions->RegistUnknowException(i_IdOrKey, i_IdOrKey);
        newException.m_rootException = this;
        return newException;
    }
} // namespace TB_NS::Error_NS
