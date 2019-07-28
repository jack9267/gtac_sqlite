disable_rtti()

set(SOURCES src/sqlite3.c)

new_library_static(sqlite "${SOURCES}" "")

target_include_directories(sqlite_static PUBLIC "include")

set_target_properties(sqlite_static PROPERTIES FOLDER Libraries)
