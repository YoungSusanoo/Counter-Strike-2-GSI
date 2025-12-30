add_rules("mode.debug")

set_xmakever("2.5.9")

add_requires("asio", {system = false})
target("gsi")
set_languages("c++23")
set_kind("binary")

add_files("src/*.cpp")
add_packages("asio")
