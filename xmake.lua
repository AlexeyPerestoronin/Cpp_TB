-- 3th party dependencies
add_requires("gtest", {system = false, configs = {shared = false}})

-- common rules
add_rules("mode.release", "mode.debug")
-- add C++20 support with corutines
set_languages("c++20")
add_defines("_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING")
add_cxxflags("/await")

add_includedirs(".external/cppcoro/include/")
target("cppcoro")
    set_kind("static")
    add_files(".external/cppcoro/lib/**.cpp")
    add_headerfiles(".external/cppcoro/include/cppcoro/**.hpp")
    --
    set_group("external")
target_end()

target("UnitTests")
    set_kind("binary")
    add_files("*.cpp")
    add_packages("gtest")
    add_deps("cppcoro")
target_end()
