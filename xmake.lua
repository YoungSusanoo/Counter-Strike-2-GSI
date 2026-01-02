add_rules("mode.debug")

set_xmakever("2.5.9")
add_requires("asio", {system = false})

option("test")
  set_default(false)
  set_showmenu(true)
  set_description("Build tests")

target("cs2-gsi")
  set_kind("static")
  set_languages("c++23")
  add_includedirs("include", { public = true, prefix = "cs2gsi" })
  add_files("src/*.cpp")
  add_packages("asio", {public = true})

if has_config("test") then 
  target("test")
    set_kind("binary")
    set_languages("c++23")
    add_files("test/*.cpp")
    add_deps("cs2-gsi")
    set_symbols("debug")
end