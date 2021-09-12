-- 3th party dependencies
add_requires("libcurl", {system = false, configs = {shared = false}})
add_requires("openssl", {system = false, configs = {shared = false}})
add_requires("gtest", {system = false, configs = {shared = false}})

-- common rules
add_rules("mode.release", "mode.debug")
set_languages("c++20")

target("exmo_api")
    set_kind("static")
    add_files("exmo_api/src/*.cpp")
    add_headerfiles("exmo_api/*.hpp", "exmo_api/*.md")
    add_packages("libcurl", "openssl")

target("main")
    set_kind("binary")
    add_files("src/*.cpp")
    add_headerfiles("*.hpp", "*.md")
    add_packages("libcurl", "openssl")
    add_deps("exmo_api")
