-- 3th party dependencies
add_requires("libcurl", {system = false, configs = {shared = true}})
add_requires("openssl", {system = false, configs = {shared = true}})
add_requires("gtest", {system = false, configs = {shared = true}})
add_requires("boost", {system = false, configs = {shared = true}})
add_requires("nlohmann_json", {system = false, configs = {shared = true}})

-- add build rules
add_rules("mode.release", "mode.debug")

-- the debug mode
if is_mode("debug") then
    -- enable the debug symbols
    set_symbols("debug")
    -- disable optimization
    set_optimize("none")
end

-- the release mode
if is_mode("release") then
    -- set the symbols visibility: hidden
    set_symbols("hidden")
    -- enable fastest optimization
    set_optimize("fastest")
    -- strip all symbols
    set_strip("all")
end

-- add C++20 support with corutines
set_languages("c++20")
set_warnings("all", "error")


-- role for targets to supporting usage of corutines
rule("corutine_supporting")
    before_build(function (target) 
        cprint("[INFO] add corutine supporting for the %s", target:name())
        -- add define for corutine compiling
        target:add("defines", "_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING")
        -- add c++-flag for corutime compiling
        target:add("cxxflags", "/await")
    end)
rule_end()


-- role for targets to copping all exception(s) files that are related with them
rule("copy_exception_file")
    -- files with exceptions could be only the files that are satisfying this template
    set_extensions(".exception.json")
    on_build_file(function (target, sourcefile, opt)
        local file_name = path.filename(sourcefile)
        cprint("${green}[INFO] for %s coping exception(s)-file %s", target:name(), file_name)
        os.cp(sourcefile, path.join(target:targetdir(), file_name))
    end)
rule_end()


-- role for coping the all DLL(s) related with the rarget
rule("copy_related_DLLs")
    -- files with exceptions could be only the files that are satisfying this template
    on_install(function (target)
        import("lib.detect.find_file")
        local name_of_target = target:name()
        print("[INFO] copying related dll(s) for %s", name_of_target)
        for pkg_name, pkg_details in pairs(target:pkgs()) do
            print("[INFO] related package is", pkg_name)

            -- TODO: a package with a static config could potentially have shared deps
            local pkg_extrainfo = pkg_details:extrainfo()
            local is_shared = not pkg_extrainfo or (pkg_extrainfo and pkg_extrainfo.configs and pkg_extrainfo.configs.shared)
            -- there are interests only for *.dll(s)
            if not is_shared then
                print("[INFO] there are interests only for *.dll(s)")
                goto for_loop_end_1
            end

            local pkg_links = pkg_details:get("links")
            if not pkg_links then
                cprint("${yellow}[WARN] there are no links for '%s'-package", pkg_name) 
                goto for_loop_end_1
            end

            -- links is a string for one and a table for multiple
            local links = type(pkg_links) == "string" and { pkg_links } or pkg_links
            for _, link in pairs(links) do
                print("[INFO] finding link: %s from package %s", link, pkg_name)
                
                local pkg_linkdirs = pkg_details:get("linkdirs")
                if not pkg_linkdirs then
                    cprint("${yellow}[WARN] there are no dirs for '%s'-link for '%s'-package", link, pkg_name)
                    goto for_loop_end_2
                end

                local target_dir = target:targetdir()

                -- attempt 1: try found target *.dll directly
                local dllname = (link or "").. ".dll"
                local dllpath = find_file(dllname, pkg_linkdirs)
                if dllpath then
                    if os.isfile(path.join(target_dir, dllname)) then
                        cprint("${yellow}[WARN] DLL with the name '%s' already exists in install dir.", dllname_from_lib)
                        cprint("${yellow}       Overwriting with DLL from package: '%s'.", pkg_name or "NO NAME")
                    end -- if
                    os.vcp(dllpath, target_dir)
                    goto for_loop_end_2
                end

                -- attempt 1: try found target *.dll via related *.lib
                local libname = (link or "") .. ".lib"
                local libpath = find_file(libname, pkg_linkdirs)
                if libpath then
                    local file = io.open(libpath, "rb")
                    local dump = file:read("*a") -- TODO: see why buffer read (eg 4096) doesn't work
                    local _, _, dllname_from_lib = dump:find("__IMPORT_DESCRIPTOR_([%a%d_%-%.]+)\0", 1, false)
                    file:close()
                    if dllname_from_lib then
                        dllname_from_lib = dllname_from_lib .. ".dll"
                        local dllpath_from_lib = find_file(dllname_from_lib, "$(env TMP)/../.xmake/packages/**")
                        if dllpath_from_lib then
                            if os.isfile(path.join(target_dir, dllname_from_lib)) then
                                cprint("${yellow}[WARN] DLL with the name '%s' already exists in install dir.", dllname_from_lib)
                                cprint("${yellow}       Overwriting with DLL from package: '%s'.", pkg_name or "NO NAME")
                            end -- if
                            os.vcp(dllpath_from_lib, target_dir)
                        else
                            cprint("${yellow}[WARN] there is impossible detect the dll-name from related lib-name")
                        end -- if
                    else
                        cprint("${yellow}[WARN] there is impossible to detect the dll-name from related lib-name")
                    end -- if
                    goto for_loop_end_2
                end -- if

                cprint("${red}[WARN] target dll cannot be found not directly not via the related *.lib")

                ::for_loop_end_2::    
            end -- for
            ::for_loop_end_1::
        end -- for
    end)
rule_end()

-- adds project's root path as the root of include files search process
add_includedirs("./")

-- build from the source files the cppcoro library and include the all headers of this library
add_includedirs(".external/cppcoro/include/")
target("cppcoro")
    set_kind("static")

    -- because, the cppcoro is the external project we aren't responsible for its warnings
    set_warnings("none")

    add_rules("corutine_supporting")

    add_files(".external/cppcoro/lib/*.cpp")

    add_headerfiles(".external/cppcoro/include/cppcoro/**.hpp")

    add_links("synchronization")
    --
    set_group("external")
target_end()


target("common")
    set_kind("static")

    add_rules("copy_exception_file")
    add_rules("corutine_supporting")

    add_files("common/src/*.cpp")
    add_files("common/**exception.json")

    add_headerfiles("common/unit-tests/*.cpp")
    add_headerfiles("common/*.hpp")
    add_headerfiles("common/*.md")
    add_headerfiles("common/**.json")

    add_packages("libcurl", "boost", "nlohmann_json")

    add_deps("cppcoro")
    --
    set_group("internal/lib")
target_end()


target("curl_adaptor")
    set_kind("static")

    add_rules("copy_exception_file")

    add_files("curl_adaptor/src/*.cpp")
    add_files("curl_adaptor/**exception.json")

    add_headerfiles("curl_adaptor/unit-tests/*.cpp")
    add_headerfiles("curl_adaptor/*.hpp")
    add_headerfiles("curl_adaptor/*.md")
    add_headerfiles("curl_adaptor/**.json")

    add_packages("libcurl", "openssl", "boost", "nlohmann_json")
    --
    set_group("internal/lib")
target_end()


target("openssl_adaptor")
    set_kind("static")

    add_rules("copy_exception_file")

    add_files("openssl_adaptor/src/*.cpp")
    add_files("openssl_adaptor/**exception.json")

    add_headerfiles("openssl_adaptor/unit-tests/*.cpp")
    add_headerfiles("openssl_adaptor/*.hpp")
    add_headerfiles("openssl_adaptor/*.md")
    add_headerfiles("openssl_adaptor/**.json")

    add_packages("libcurl", "openssl", "boost", "nlohmann_json")
    --
    set_group("internal/lib")
target_end()


target("exmo_api")
    set_kind("static")

    add_rules("copy_exception_file")
    add_rules("corutine_supporting")

    add_files("exmo_api/**/src/*.cpp")
    add_files("exmo_api/**exception.json")

    add_headerfiles("exmo_api/**/unit-tests/*.cpp")
    add_headerfiles("exmo_api/**/*.hpp")
    add_headerfiles("exmo_api/**.json")
    add_headerfiles("exmo_api/**.md")

    add_packages("libcurl", "openssl", "boost", "nlohmann_json")

    add_deps("common", "curl_adaptor", "openssl_adaptor", "trader")
    --
    set_group("internal/lib")
target_end()


target("trader")
    set_kind("static")

    add_rules("copy_exception_file")
    add_rules("corutine_supporting")

    add_files("trader/src/*.cpp")
    add_files("trader/**/src/*.cpp")
    add_files("trader/**exception.json")

    add_headerfiles("trader/**/unit-tests/*.cpp")
    add_headerfiles("trader/**.hpp")
    add_headerfiles("trader/**.json")
    add_headerfiles("trader/**.md")

    add_packages("libcurl", "openssl", "boost", "nlohmann_json")

    add_deps("common", "curl_adaptor", "openssl_adaptor")
    --
    set_group("internal/lib")
target_end()


target("TB")
    set_kind("binary")

    add_rules("copy_exception_file")
    add_rules("corutine_supporting")
    add_rules("copy_related_DLLs")

    add_files("src/TB.cpp")
    add_files("src/TB.exception.json")

    add_packages("libcurl", "openssl", "boost", "nlohmann_json")
    
    add_deps("exmo_api", "common", "curl_adaptor", "openssl_adaptor")
    --
    set_group("internal/exe")
target_end()


target("UnitTests")
    set_kind("binary")

    add_rules("copy_exception_file")
    add_rules("corutine_supporting")
    add_rules("copy_related_DLLs")

    add_files("src/UnitTests.cpp")
    add_files("src/UnitTests.exception.json")
    add_files("**/unit-tests/*.cpp")

    add_packages("libcurl", "openssl", "boost", "gtest", "nlohmann_json")

    add_deps("common", "curl_adaptor", "openssl_adaptor", "exmo_api", "trader")
    --
    set_group("internal/exe")
target_end()
