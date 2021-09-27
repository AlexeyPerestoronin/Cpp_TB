-- 3th party dependencies
add_requires("libcurl", {system = false, configs = {shared = false}})
add_requires("openssl", {system = false, configs = {shared = false}})
add_requires("gtest", {system = false, configs = {shared = false}})
add_requires("boost", {system = false, configs = {shared = false}})

-- common rules
add_rules("mode.release", "mode.debug")
-- add C++20 support with corutines
set_languages("c++20")
add_defines("_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING")
add_cxxflags("/await")

add_includedirs("./")


add_includedirs(".external/cppcoro/include/")
target("cppcoro")
    set_kind("static")
    add_files(".external/cppcoro/lib/*.cpp")
    add_headerfiles(".external/cppcoro/include/cppcoro/**.hpp")
    --
    set_group("external")
target_end()

target("common")
    set_kind("static")
    add_files("common/src/*.cpp")
    add_headerfiles("common/unit-tests/*.cpp")
    add_headerfiles("common/*.hpp", "common/*.md")
    add_packages("libcurl", "boost")
    add_deps("cppcoro")
    --
    set_group("internal")
target_end()

target("exmo_api")
    set_kind("static")
    add_files("exmo_api/src/*.cpp")
    add_headerfiles("exmo_api/unit-tests/*.cpp")
    add_headerfiles("exmo_api/*.hpp", "exmo_api/*.md")
    add_packages("libcurl", "openssl")
    --
    set_group("internal")
target_end()

target("TB")
    set_kind("binary")
    add_files("src/TB.cpp")
    add_headerfiles("*.md")
    add_packages("libcurl", "openssl")
    add_deps("exmo_api")
    --
    set_group("internal")
target_end()

target("UnitTests")
    set_kind("binary")
    add_files("src/UnitTests.cpp")
    add_files("**/unit-tests/*.cpp")
    add_packages("libcurl", "openssl", "boost", "gtest")
    add_deps("exmo_api", "common")
    --
    set_group("internal")
target_end()
