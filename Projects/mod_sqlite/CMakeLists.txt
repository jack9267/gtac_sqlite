disable_rtti()

file(GLOB_RECURSE SOURCES *.cpp *.h)

if(MSVC)
	precompiled_header(pch)
endif()

new_library_shared(mod_sqlite "${SOURCES}" "")

# Link with SQLite
target_link_libraries(mod_sqlite sqlite_static)

set_target_properties(mod_sqlite PROPERTIES FOLDER Modules)
