#include "../base_exception.hpp"

#include <nlohmann/json.hpp>

#include <fstream>

namespace TB_NS::Error_NS {
    namespace {
        // brief: container to serach exception by its ID
        using ErrorId = std::vector<uint8_t>;
        using IdToExceptions = std::map<ErrorId, Exception&>;
        IdToExceptions ExceptionsById{};

        // brief: container to serach exception by its key
        using ErrorKey = std::string;
        using KeyToExceptions = std::map<ErrorKey, Exception&>;
        KeyToExceptions ExceptionsByKey{};

        // brief: list of all exceptions of the instance of the programm
        std::list<Exception> Exceptions{};
    }; // namespace

    const char* Exception::what() const {
        using namespace boost;

        std::stringstream message;
        for (auto [suberror, extractor] = std::make_tuple(
                 this,
                 [&message](std::string i_info, auto*i_data, std::string i_defaultMessage = "no info") {
                     message << std::move(i_info);
                     i_data ? message << *i_data : message << i_defaultMessage;
                     message << std::endl;
                 });
             suberror; suberror = get_error_info<Suberror>(*suberror)) {
            message << "***" << std::endl;
            extractor("[location] ", get_error_info<Location>(*suberror));
            extractor("[description] ", get_error_info<Description>(*suberror));
            message << "***" << std::endl;
        }
        return d_errorMessage.assign(message.str()).data();
    }

    // TODO: this class is need to move in separate file
    template<class Type>
    class RAI : public Type {
        using BaseType = Type;
        using Deleter = void (*)(BaseType&) noexcept;

        Deleter m_delter;

        public:
        RAI(RAI&&) noexcept = default;
        RAI(const RAI&) noexcept = default;

        template<class... Args>
        RAI(Deleter&& i_deleter, Args&&... i_args)
        noexcept
            : BaseType(std::forward<Args>(i_args)...)
            , m_delter(i_deleter) {}

        ~RAI() noexcept {
            m_delter(*this);
        }
    };

    void LoadSettings(const fs::path& i_settigsFilePath) {
        using namespace nlohmann;

        json json;
        RAI<std::ifstream>([](std::ifstream& i_file) noexcept { i_file.close(); }, i_settigsFilePath) >> json;

        // TODO: there is good to use a recursive algorithm to parsing the json-presented-errors

        return;
    }
} // namespace TB_NS::Error_NS
