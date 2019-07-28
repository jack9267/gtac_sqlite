disable_rtti()

file(GLOB_RECURSE SOURCES *.cpp *.h)

if(MSVC)
	precompiled_header(pch)
endif()

new_library_shared(gtac_sqlite "${SOURCES}" "")

# Link with SQLite
target_link_libraries(gtac_sqlite sqlite_static)

set_target_properties(gtac_sqlite PROPERTIES FOLDER Modules)
