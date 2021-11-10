// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// ****Alexey.Perestoronin@yandex.ru ** //
// ************************************ //

#include <common/base_exception.hpp>

namespace TB_NS::Error_NS {
    namespace {
        class Printer {
            size_t m_offcet{};
            std::stringstream m_message{};

            public:
            TB_MAYBE_UNUSED Printer& offcet(size_t i_offcet) {
                m_offcet = i_offcet;
                return *this;
            }

            template<class... Values>
            TB_MAYBE_UNUSED Printer& add(const Values&... i_values) {
                m_message << Str::BaseType(m_offcet, ' ');
                ((m_message << i_values), ...);
                m_message << std::endl;
                return *this;
            }

            Str print() {
                return m_message.str();
            }
        };

        class Processer {
            static Str format(Str templ, Exception::ReplaceUnits::CR units) {
                for (const auto& [key, value] : units)
                    for (size_t start = templ.find(key), len = key.size(); start != std::string::npos; start = templ.find(key))
                        templ.replace(start, len, value);
                return templ;
            }

            public:
            Printer& printer;
            bool is_verbose{ false };

            void process(const Exception* i_exception) const {
                if (auto* locationP = boost::get_error_info<Location>(*i_exception); locationP)
                    printer.add("location: ", *locationP);
                Error::CR error = i_exception->getError();
                printer.add("id: ", error.id);
                printer.add("key: ", error.key.value_or("doesn't defined"));
                if (error.values.has_value())
                    for (const auto& [key, value] : error.values.value())
                        printer.add("* ", key, ": ", format(value, i_exception->getUnits()));
                if (is_verbose)
                    if (Exception::CP parentError = boost::get_error_info<Parenterror>(*i_exception); parentError) {
                        printer.add("parent error:");
                        process(parentError);
                    }
            }
        };
    } // namespace

    Exception::Exception(Error::CR i_error) noexcept
        : m_error{ i_error } {}

    Exception::~Exception() {
        return;
    }

    Error::CR Exception::getError() const {
        return m_error;
    }

    Exception::ReplaceUnits::CR Exception::getUnits() const {
        return m_replaceUnits;
    }

    const char* Exception::what() const {
        Printer message{};
        Processer processer{ .printer = message, .is_verbose = true };

        for (auto [suberror, offcet] = std::make_tuple(this, size_t{ 0 }); suberror; offcet += 3) {
            message.offcet(offcet);
            processer.process(suberror);
            if (suberror = boost::get_error_info<Suberror>(*suberror); suberror)
                message.add("sub-error:");
        }
        m_errorMessage = std::make_shared<Str>(message.print());
        return m_errorMessage->data();
    }

    TB_NODISCARD Exception& Exception::operator()(ReplaceUnit i_replaceUnit) noexcept {
        m_replaceUnits.push_back(std::move(i_replaceUnit));
        return *this;
    }

    TB_NODISCARD Exception Exception::operator[](Str::CR i_IdOrKey) noexcept {
        Error* targetErrorPtr{};
        for (Error* subErrorPtr : m_error.subErrors)
            if (subErrorPtr->isEquivalent(i_IdOrKey))
                targetErrorPtr = subErrorPtr;

        Exception subException = targetErrorPtr ? Exception(*targetErrorPtr) : Exception(PredefinedError_NS::UnregException)({ "{keyOrValue:}", i_IdOrKey });
        subException << Parenterror(*this);
        return std::move(subException);
    }
} // namespace TB_NS::Error_NS
