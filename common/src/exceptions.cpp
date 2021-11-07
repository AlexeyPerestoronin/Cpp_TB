// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include <common/base_exception.hpp>
#include <common/rai.hpp>

#include <nlohmann/json.hpp>

namespace TB_NS::Error_NS {
    Exceptions::Exceptions() {
        Exception::IdKeyValues settingsIsNotLoading{ "-1", "errors isn't load", { { "description", "errors cannot be loaded from {file_name}" } } };
        m_rootException = new Exception(this, settingsIsNotLoading);
        m_allExceptions.emplace_back(m_rootException);
    }

    Exceptions& Exceptions::GetIns() {
        static Exceptions::UP instance{ new Exceptions() };
        return *instance;
    }

    Exceptions::~Exceptions() {
        if (!m_unregistedExceptions.empty()) {
            std::function<void(Str::CR)> logger = [](auto message) { std::cerr << message; };

            RAI<std::fstream> fileLogger("unregistered-error.txt", std::ios::app | std::ios::in);
            if (fileLogger.is_open()) {
                logger = [&fileLogger](const auto& message) {
                    fileLogger << message;
                    std::cerr << message;
                };
            }

            for (const auto& exceptionSP : m_unregistedExceptions)
                logger(exceptionSP->what());
        }

        for (auto& exceptions : std::initializer_list{ m_unregistedExceptions, m_allExceptions })
            for (auto exception : exceptions)
                if (exception)
                    delete exception;
    }

    Exception& Exceptions::RegistNewException(Str::CR i_id, Str::CR i_key, Exception::Values::CR i_values) {
        auto& exceptions = m_allExceptions;
        exceptions.push_back(new Exception(this, i_id, i_key, i_values));
        return *exceptions.back();
    }

    Exception& Exceptions::RegistUnknowException(Str::CR i_id, Str::CR i_key, Exception::Values::CRO i_values) {
        auto& exceptions = m_unregistedExceptions;
        exceptions.push_back(new Exception(
            this, i_id, i_key,
            i_values.value_or(Exception::Values{
                { "unknown exception", "this exceptions cannot be recognized as registered" },
                { "possible action", "try to add exception by presented id/key inside target errof-settings-file" },
            })));
        return *exceptions.back();
    }

    void Exceptions::LoadSettings(Path::CR i_settigsFilePath) {
        using namespace nlohmann;

        json errors;
        if (auto settingsFile = RAI<std::ifstream>(i_settigsFilePath); settingsFile.is_open())
            settingsFile >> errors;
        else
            throw std::ref(*m_rootException);

        struct {
            Exceptions* exceptions;

            Exception::PC createNewException(json& i_error) {
                Exception* result{};
                try {
                    result = &(exceptions->RegistNewException(Str::BaseType(i_error["id"]), Str::BaseType(i_error["key"])));
                    if (auto values = i_error.find("values"); values != i_error.end())
                        for (auto [key, value] : values->items())
                            result->m_values.emplace(key, value);
                } catch (...) {
                }
                return result;
            }

            void operator()(json& i_errors, Exception::PC i_rootException) {
                for (auto error : i_errors["exceptions"]) {
                    if (auto* exceptionPC = createNewException(error); exceptionPC) {
                        i_rootException->m_subException.push_back(exceptionPC);
                        exceptionPC->m_parentException = i_rootException;
                        if (error.find("exceptions") != error.end())
                            this->operator()(error, exceptionPC);
                    } else {
                        std::cerr << "some error in the json:\n***\n" << error.dump(4) << "\n***\n";
                    }
                }
            }
        } parser{ .exceptions = this };
        parser(errors, m_rootException);
    }

    Exception::CR Exceptions::operator[](Str::CR i_IdOrKey) const noexcept {
        const auto& exceptions = m_rootException->m_subException;
        const auto comparator = std::bind(&Exception::isSatisfy, std::placeholders::_1, i_IdOrKey);
        if (const auto exception = std::ranges::find_if(exceptions, std::move(comparator)); exception != exceptions.end())
            return *(*exception);
        return TB_CONST_CAST(this)->RegistUnknowException(i_IdOrKey, i_IdOrKey);
    }
} // namespace TB_NS::Error_NS
