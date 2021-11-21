// ************************************ //
// **************** TB **************** //
// *** Alexey Perestoronin's project ** //
// *** Alexey.Perestoronin@yandex.ru ** //
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

            TB_NODISCARD Str print() {
                return m_message.str();
            }
        };

        struct Processer {
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
                        printer.add("* ", key, ": ", static_cast<Str>(value).format(i_exception->getUnits()));
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
        Printer printer{};
        Processer processer{ .printer = printer, .is_verbose = true };

        for (auto [suberror, offcet] = std::make_tuple(this, size_t{ 0 }); suberror; offcet += 3) {
            printer.offcet(offcet);
            processer.process(suberror);
            if (suberror = boost::get_error_info<Suberror>(*suberror); suberror)
                printer.add("sub-error:");
        }
        m_errorMessage = std::make_shared<Str>(printer.print());
        return m_errorMessage->data();
    }

    TB_NODISCARD Exception& Exception::operator()(ReplaceUnits i_replaceUnits) noexcept {
        m_replaceUnits.insert(std::make_move_iterator(i_replaceUnits.begin()), std::make_move_iterator(i_replaceUnits.end()));
        return *this;
    }

    TB_NODISCARD Exception& Exception::operator()(ReplaceUnits::value_type i_replaceUnit) noexcept {
        m_replaceUnits.emplace(std::move(i_replaceUnit));
        return *this;
    }

    TB_NODISCARD Exception& Exception::operator()(Str i_key, Str i_value) noexcept {
        m_replaceUnits.emplace(std::move(i_key), std::move(i_value));
        return *this;
    }

    TB_NODISCARD Exception Exception::operator[](Str::CR i_IdOrKey) noexcept {
        const Error* targetErrorPtr = &PredefinedError_NS::UnregException;
        for (Error* subErrorPtr : m_error.subErrors)
            if (subErrorPtr->isEquivalent(i_IdOrKey))
                targetErrorPtr = subErrorPtr;
        
        Exception subException = Exception(*targetErrorPtr);
        subException << Parenterror(*this);
        return std::move(subException);
    }
} // namespace TB_NS::Error_NS
