add_requires("libcurl", {system = false, configs = {shared = false}})
add_requires("openssl", {system = false, configs = {shared = false}})
add_requires("gtest", {system = false, configs = {shared = false}})

target("exmo")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("libcurl", "openssl")
    
