#include <gtest/gtest.h>

#include <cppcoro/async_generator.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/generator.hpp>

#include <algorithm>
#include <fstream>
#include <ranges>
#include <chrono>
#include <array>
#include <future>

namespace {
    constexpr const uint8_t MAX = 0xCui8;

    template<size_t dimension>
    class number : public std::array<uint8_t, dimension> {
        static_assert(dimension >= 2, "the dimension must be more or equal 2");

        public:
        using base_type = std::array<uint8_t, dimension>;
        using value_type = typename base_type::value_type;

        public:
        constexpr static size_t dimension = dimension;
        constexpr static bool is_even = dimension % 2 == 0;

        private:
        bool increment(size_t i_index) {
            if (auto& sym{ base_type::operator[](i_index) }; sym < MAX)
                return ++sym;
            else
                return sym = 0x0;
        };

        public:
        // clang-format off
		[[nodiscard("there is no reason to create object without using")]]
		number(value_type i_initial_value = value_type{}) {
			for (size_t i{ 0 }; i < dimension; ++i)
				base_type::operator[](i) = i_initial_value;
		}
        // clang-format on

        uint32_t getSum() const {
            uint32_t result{};
            for (size_t i{ 0 }; i < dimension; ++i)
                result += base_type::operator[](i);
            return result;
        }

        uint32_t getLSum() const {
            uint32_t result{};
            for (size_t i{ 0 }; i < dimension / 2; ++i)
                result += base_type::operator[](i);
            return result;
        }

        uint32_t getRSum() const {
            uint32_t result{};
            for (size_t i{ dimension / 2 + (is_even ? 0 : 1) }; i < dimension; ++i)
                result += base_type::operator[](i);
            return result;
        }

        bool isBeautiful() const {
            return (base_type::operator[](0) == base_type::value_type(0x0)) && (getLSum() == getRSum());
        }

        number& operator++() {
            for (size_t i{ dimension - 1 }; i >= 0; --i)
                if (increment(i))
                    break;
            return *this;
        }

        bool operator==(const number& i_number) const {
            return static_cast<const base_type&>(*this) == static_cast<const base_type&>(i_number);
        }

        static constexpr const number& max() {
            static const number max(MAX);
            return max;
        }

        friend std::ostream& operator<<(std::ostream& i_stream, const number& i_number) {
            size_t i{ 0 };

            i_stream << '[' << std::hex;

            i_stream << static_cast<uint32_t>(i_number[i++]);
            for (; i < dimension / 2; ++i)
                i_stream << static_cast<uint32_t>(i_number[i]);

            if constexpr (!number::is_even)
                i_stream << '|' << static_cast<uint32_t>(i_number[i++]) << '|';
            else
                i_stream << '|';

            for (; i < dimension; ++i)
                i_stream << static_cast<uint32_t>(i_number[i]);

            i_stream << ']' << std::dec;

            return i_stream;
        }
    };

    template<size_t dimension>
    cppcoro::generator<const number<dimension>&> get_next_number() {
        number<dimension> num{};
        for (; num != number<dimension>::max(); ++num)
            co_yield num;
        co_yield num;
    }

    template<size_t dimension, bool is_verbose = false>
    class TestTask : public ::testing::Test {
        inline static std::shared_ptr<std::fstream> file_ptr{};

        protected:
        static std::string getSaveFileName() {
            return std::format("../../../../result_{}.txt", std::to_string(dimension));
        }

        static void SetUpTestSuite() {
            auto open_mode = std::ios::in | std::ios::out | std::ios::trunc;
            file_ptr = std::shared_ptr<std::fstream>(new std::fstream(getSaveFileName(), open_mode), [](std::fstream* ptr) {
                if (!ptr)
                    return;
                ptr->close();
                delete ptr;
            });

            if (!file_ptr->is_open())
                throw std::exception("file cannot be open");
        }

        public:
        // brief: compute all beautiful 13th-means-numbers
        // note1: designed only for proof of hypotheses about computation correct
        // note2: makes computation only for 13th-means-numbers that are less than 8 dimension
        void runSolution_1() {
            *file_ptr << "\nsolution 1:" << std::endl;

            if constexpr (dimension < 8) {
                uint64_t counter{ 0 };
                auto start = std::chrono::high_resolution_clock::now();
                for (auto num : get_next_number<dimension>())
                    if (num.isBeautiful()) {
                        ++counter;
                        if constexpr (is_verbose)
                            *file_ptr << '\t' << num << ' ' << counter << std::endl;
                    }
                auto stop = std::chrono::high_resolution_clock::now();

                *file_ptr << "\tdimension: " << dimension << std::endl
                          << "\tcomputation type: " << (stop - start) << std::endl
                          << "\tquantity of beautiful: " << counter << std::endl;
            } else
                *file_ptr << "\tisn't consider!" << std::endl;
        }

        // brief: compute all beautiful 13th-means-numbers
        // note1: designed for fast computation performing
        // note2: makes computation only for 13th-means-numbers that are greater than 3 dimension
        void runSolution_2() {
            *file_ptr << "\nsolution 2:" << std::endl;

            using sum_to_quantity = std::map<uint32_t, size_t>;
            if constexpr (dimension > 3) {
                auto l_side_computer = std::async(std::launch::async, []() -> sum_to_quantity {
                    sum_to_quantity result{};
                    for (auto num : get_next_number<dimension / 2>())
                        if (num[0] == 0x0)
                            ++result[num.getSum()];
                    return result;
                });

                auto r_side_computer = std::async(std::launch::async, []() -> sum_to_quantity {
                    sum_to_quantity result{};
                    for (auto num : get_next_number<dimension / 2>())
                        ++result[num.getSum()];
                    return result;
                });

                auto start = std::chrono::high_resolution_clock::now();
                uint64_t counter{ 0 };
                for (auto l_side = l_side_computer.get(), r_side = r_side_computer.get(); auto [sum, quantity] : l_side)
                    counter += quantity * r_side[sum];
                if constexpr (dimension % 2 != 0)
                    counter *= (MAX + 1);
                auto stop = std::chrono::high_resolution_clock::now();

                *file_ptr << "\tdimension: " << dimension << std::endl
                          << "\tcomputation type: " << (stop - start) << std::endl
                          << "\tquantity of beautiful: " << counter << std::endl;
            } else
                *file_ptr << "\tisn't consider!" << std::endl;
        }
    };

#define REG_TEST(dimension, ...)                                        \
    namespace {                                                         \
        using Dimension_##dimension = TestTask<dimension, __VA_ARGS__>; \
                                                                        \
        TEST_F(Dimension_##dimension, Solution_1) {                     \
            ASSERT_NO_THROW(runSolution_1());                           \
        }                                                               \
                                                                        \
        TEST_F(Dimension_##dimension, Solution_2) {                     \
            ASSERT_NO_THROW(runSolution_2());                           \
        }                                                               \
    }

    REG_TEST(2, true)
    REG_TEST(3, true)
    REG_TEST(4, true)
    REG_TEST(5, true)
    REG_TEST(6, true)
    REG_TEST(7)
    REG_TEST(8)
    REG_TEST(9)
    REG_TEST(10)
    REG_TEST(11)
    REG_TEST(12)
    REG_TEST(13)
} // namespace