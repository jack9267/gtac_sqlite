
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(UNIX)
	# link time generation
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

	if(CMAKE_COMPILER_IS_GNUCC)
		# default hidden
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -fdata-sections -ffunction-sections")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -fdata-sections -ffunction-sections")

		# strip on release builds
		set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL} -s")
		set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -s")

		set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "${CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL} -s")
		set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -s")
	endif()
endif()

if(MSVC)
	# Enable PDB on all configurations
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")

	set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /Zi")
	set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /Zi")

	# change it so subsystem is set to windows for DLL files as this is default in Visual Studio
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /debug")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /debug")

	set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "${CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL} /debug")
	set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} /debug")
endif()

# C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

set(RUNTIME_DEBUG_POSTFIX "_d")
set(RUNTIME_RELEASE_POSTFIX "")

# determine bits
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ENGINE_PLATFORM "x64")
else()
	set(ENGINE_PLATFORM "x86")
endif()

# on Windows append the bits (if 64-bit only)
if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(RUNTIME_DEBUG_POSTFIX "_d_${ENGINE_PLATFORM}")
	set(RUNTIME_RELEASE_POSTFIX "_${ENGINE_PLATFORM}")
endif()

# lib directory
if(MSVC)
	set(LIB_DIRECTORY "Lib/${ENGINE_PLATFORM}/${CMAKE_VS_PLATFORM_TOOLSET}")
else()
	set(LIB_DIRECTORY "lib")
endif()

# hide symbols
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# hide symbols on unix
if(UNIX AND CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fvisibility=hidden")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -fvisibility=hidden")
endif()

macro(add_compiler_flags FLAGS)
	foreach(FLAG ${FLAGS})
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}")
	endforeach()
endmacro()

macro(compiler_flags CONFIGURATIONS FLAGS)
	foreach(CONFIGURATION ${CONFIGURATIONS})
		foreach(FLAG ${FLAGS})
			set(CMAKE_C_FLAGS_${CONFIGURATION} "${CMAKE_C_FLAGS_${CONFIGURATION}} ${FLAG}")
			set(CMAKE_CXX_FLAGS_${CONFIGURATION} "${CMAKE_CXX_FLAGS_${CONFIGURATION}} ${FLAG}")
		endforeach()
	endforeach()
endmacro()

macro(linker_flags TYPES CONFIGURATIONS FLAGS)
	foreach(CONFIGURATION ${CONFIGURATIONS})
		foreach(TYPE ${TYPES})
			set(CMAKE_${TYPE}_LINKER_FLAGS_${CONFIGURATION} "${CMAKE_${TYPE}_LINKER_FLAGS_${CONFIGURATION}} ${FLAGS}")
		endforeach()
	endforeach()
endmacro()

macro(define CONFIGURATION DEFINES)
	foreach(DEFINE ${DEFINES})
		set(CMAKE_C_FLAGS_${CONFIGURATION} "${CMAKE_C_FLAGS_${CONFIGURATION}} -D${DEFINE}")
		set(CMAKE_CXX_FLAGS_${CONFIGURATION} "${CMAKE_CXX_FLAGS_${CONFIGURATION}} -D${DEFINE}")
	endforeach()
endmacro()

macro(define_debug DEFINES)
	define(DEBUG ${DEFINES})
endmacro()

# Enable DEBUG=1 for debug builds as some projects check that
define_debug(DEBUG=1)

# Visual Studio specifics
if(MSVC)
	# Force /MT for static VC runtimes if Release...
	option(FORCE_STATIC_VCRT "Force /MT for static VC runtimes" ON)
	if(FORCE_STATIC_VCRT)
		foreach(flag_var
			CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
			CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
			CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
			CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
			if(${flag_var} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
			endif()
		endforeach()

		set(LIB_DIRECTORY "${LIB_DIRECTORY}_static")
	endif()

	if(MSVC_VERSION GREATER_EQUAL 1900 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
		option(NO_ENHANCED_INSTRUCTIONS "No Enhanced Instructions" OFF)
		if(NO_ENHANCED_INSTRUCTIONS)
			# No Enhanced Instructions
			add_compiler_flags(/arch:IA32)
		endif()
	endif()

	# common defines
	add_definitions(-DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS -D_USE_MATH_DEFINES=1 -DNOMINMAX)

	if(MSVC_VERSION GREATER_EQUAL 1900)
		# Prevent static variables from being thread local storage
		add_compiler_flags("/Zc:threadSafeInit-")
	endif()

	# multi-processor core
	add_compiler_flags("/MP")

	# smaller binary in release
	compiler_flags("RELEASE" "/O1 /Ob2")

	# whole program optimisation
	compiler_flags("RELEASE;RELWITHDEBINFO;MINSIZEREL" "/GL /Oi")

	foreach(flag_var
		CMAKE_SHARED_LINKER_FLAGS_RELEASE
		CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO
		CMAKE_EXE_LINKER_FLAGS_RELEASE
		CMAKE_EXE_LINKER_FLAGS_MINSIZEREL CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO)
		if(${flag_var} MATCHES "/INCREMENTAL" AND NOT ${flag_var} MATCHES "/INCREMENTAL:NO")
			string(REGEX REPLACE "/INCREMENTAL" "/INCREMENTAL:NO" ${flag_var} "${${flag_var}}")
		endif()
	endforeach()

	linker_flags("SHARED;STATIC;EXE" "RELEASE;RELWITHDEBINFO;MINSIZEREL" "/LTCG")
	linker_flags("SHARED;EXE" "RELEASE;RELWITHDEBINFO;MINSIZEREL" "/OPT:REF")

	# disable manifest on dlls
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")

	# macro to disable VS warning
	macro(disable_warning ID)
		add_compiler_flags(/wd${ID})
	endmacro()

	# macro to disable VS linker warnings
	macro(disable_linker_warning ID)
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /ignore:${ID}")
		set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:${ID}")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:${ID}")
	endmacro()

	# This object file does not define any previously undefined public symbols, so it will not be used by any link operation that consumes this library
	disable_linker_warning(4221)
endif()

if(UNIX)
	# Disable deprecated warnings because of unnamed unions
	add_compiler_flags("-Wno-deprecated-declarations")
endif()

# precompiled header macro
macro(precompiled_header HEADER)
	if(MSVC)
		add_compiler_flags(/Yu${HEADER}.h)
		set_source_files_properties(${HEADER}.cpp PROPERTIES COMPILE_FLAGS "/Yc${HEADER}.h")
	endif()
endmacro()

macro(set_target_precompiled_header NAME HEADER)
	if(MSVC)
		set_target_properties("${NAME}" PROPERTIES COMPILE_FLAGS "/Yu${HEADER}.h")
		set_source_files_properties(${HEADER}.cpp PROPERTIES COMPILE_FLAGS "/Yc${HEADER}.h")
	endif()
endmacro()

function(set_target_module_definition_file NAME ENGINE_DEF CONFIGURATION)
	if(MSVC)
		set(LINK_CMD "/DEF:\"${CMAKE_SOURCE_DIR}/${ENGINE_DEF}\"")

		# apply link_cmd to all release configurations
		set_target_properties("${NAME}" PROPERTIES LINK_FLAGS_${CONFIGURATION} ${LINK_CMD})
	endif()
endfunction()

macro(install_target NAME)
	if(WIN32)
		install(TARGETS "${NAME}"
			RUNTIME DESTINATION "${LIB_DIRECTORY}/Debug" CONFIGURATIONS Debug
			LIBRARY DESTINATION "${LIB_DIRECTORY}/Debug" CONFIGURATIONS Debug
			ARCHIVE DESTINATION "${LIB_DIRECTORY}/Debug" CONFIGURATIONS Debug)
		install(TARGETS "${NAME}"
			RUNTIME DESTINATION "${LIB_DIRECTORY}/Release" CONFIGURATIONS Release
			LIBRARY DESTINATION "${LIB_DIRECTORY}/Release" CONFIGURATIONS Release
			ARCHIVE DESTINATION "${LIB_DIRECTORY}/Release" CONFIGURATIONS Release)
		install(TARGETS "${NAME}"
			RUNTIME DESTINATION "${LIB_DIRECTORY}/RelWithDebInfo" CONFIGURATIONS RelWithDebInfo
			LIBRARY DESTINATION "${LIB_DIRECTORY}/RelWithDebInfo" CONFIGURATIONS RelWithDebInfo
			ARCHIVE DESTINATION "${LIB_DIRECTORY}/RelWithDebInfo" CONFIGURATIONS RelWithDebInfo)
	else()
		install(TARGETS "${NAME}"
			RUNTIME DESTINATION "${LIB_DIRECTORY}"
			LIBRARY DESTINATION "${LIB_DIRECTORY}"
			ARCHIVE DESTINATION "${LIB_DIRECTORY}")
	endif()
endmacro()

macro(install_target_to NAME TO)
	if(WIN32)
		install(TARGETS "${NAME}"
			RUNTIME DESTINATION "${LIB_DIRECTORY}/Debug/${TO}" CONFIGURATIONS Debug
			LIBRARY DESTINATION "${LIB_DIRECTORY}/Debug/${TO}" CONFIGURATIONS Debug
			ARCHIVE DESTINATION "${LIB_DIRECTORY}/Debug/${TO}" CONFIGURATIONS Debug)
		install(TARGETS "${NAME}"
			RUNTIME DESTINATION "${LIB_DIRECTORY}/Release/${TO}" CONFIGURATIONS Release
			LIBRARY DESTINATION "${LIB_DIRECTORY}/Release/${TO}" CONFIGURATIONS Release
			ARCHIVE DESTINATION "${LIB_DIRECTORY}/Release/${TO}" CONFIGURATIONS Release)
		install(TARGETS "${NAME}"
			RUNTIME DESTINATION "${LIB_DIRECTORY}/RelWithDebInfo/${TO}" CONFIGURATIONS RelWithDebInfo
			LIBRARY DESTINATION "${LIB_DIRECTORY}/RelWithDebInfo/${TO}" CONFIGURATIONS RelWithDebInfo
			ARCHIVE DESTINATION "${LIB_DIRECTORY}/RelWithDebInfo/${TO}" CONFIGURATIONS RelWithDebInfo)
	else()
		install(TARGETS "${NAME}"
			RUNTIME DESTINATION "${LIB_DIRECTORY}/${TO}"
			LIBRARY DESTINATION "${LIB_DIRECTORY}/${TO}"
			ARCHIVE DESTINATION "${LIB_DIRECTORY}/${TO}")
	endif()
endmacro()

macro(install_target_debug_info NAME)
	if(MSVC)
		install(FILES $<TARGET_PDB_FILE:${NAME}> DESTINATION "${LIB_DIRECTORY}/Debug" CONFIGURATIONS Debug OPTIONAL)
		install(FILES $<TARGET_PDB_FILE:${NAME}> DESTINATION "${LIB_DIRECTORY}/Release" CONFIGURATIONS Release OPTIONAL)
		install(FILES $<TARGET_PDB_FILE:${NAME}> DESTINATION "${LIB_DIRECTORY}/RelWithDebInfo" CONFIGURATIONS RelWithDebInfo OPTIONAL)
	endif()
endmacro()

macro(install_target_debug_info_to NAME TO)
	if(MSVC)
		install(FILES $<TARGET_PDB_FILE:${NAME}> DESTINATION "${LIB_DIRECTORY}/Debug/${TO}" CONFIGURATIONS Debug OPTIONAL)
		install(FILES $<TARGET_PDB_FILE:${NAME}> DESTINATION "${LIB_DIRECTORY}/Release/${TO}" CONFIGURATIONS Release OPTIONAL)
		install(FILES $<TARGET_PDB_FILE:${NAME}> DESTINATION "${LIB_DIRECTORY}/RelWithDebInfo/${TO}" CONFIGURATIONS RelWithDebInfo OPTIONAL)
	endif()
endmacro()

# to avoid repeating stuff
function(new_library_static NAME SOURCES DEFINES)
	set(LIBRARY_SOURCES "")
	foreach(SOURCE ${SOURCES})
		list(APPEND LIBRARY_SOURCES ${SOURCE})
	endforeach()

	add_library("${NAME}_static" STATIC ${LIBRARY_SOURCES})

	if(MSVC AND EMBEDDED_PDB)
		# embedd pdb
		set_target_properties("${NAME}_static" PROPERTIES COMPILE_OPTIONS "/Z7")
	endif()

	foreach(DEFINE ${DEFINES})
		target_compile_definitions("${NAME}_static" PRIVATE ${DEFINE})
	endforeach()

	install_target(${NAME}_static)
endfunction()

function(new_library_shared NAME SOURCES DEFINES)
	set(LIBRARY_SOURCES "")
	foreach(SOURCE ${SOURCES})
		list(APPEND LIBRARY_SOURCES ${SOURCE})
	endforeach()

	add_library("${NAME}" SHARED ${LIBRARY_SOURCES})

	set_target_properties("${NAME}" PROPERTIES RUNTIME_OUTPUT_NAME "${NAME}${RUNTIME_RELEASE_POSTFIX}")
	set_target_properties("${NAME}" PROPERTIES RUNTIME_OUTPUT_NAME_DEBUG "${NAME}${RUNTIME_DEBUG_POSTFIX}")

	foreach(DEFINE ${DEFINES})
		target_compile_definitions("${NAME}" PRIVATE ${DEFINE})
	endforeach()

	install_target(${NAME})
	install_target_debug_info("${NAME}")
endfunction()

function(new_library_executable NAME SOURCES DEFINES)
	set(LIBRARY_SOURCES "")
	foreach(SOURCE ${SOURCES})
		list(APPEND LIBRARY_SOURCES ${SOURCE})
	endforeach()

	add_executable("${NAME}" ${LIBRARY_SOURCES})

	set_target_properties("${NAME}" PROPERTIES RUNTIME_OUTPUT_NAME "${NAME}${RUNTIME_RELEASE_POSTFIX}")
	set_target_properties("${NAME}" PROPERTIES RUNTIME_OUTPUT_NAME_DEBUG "${NAME}${RUNTIME_DEBUG_POSTFIX}")

	foreach(DEFINE ${DEFINES})
		target_compile_definitions("${NAME}" PRIVATE ${DEFINE})
	endforeach()

	install_target(${NAME})
	install_target_debug_info(${NAME})
endfunction()

macro(disable_rtti)
	if(MSVC)
		# disable RTTI
		add_compiler_flags(/GR-)
	endif()
endmacro()

macro(group_sources SOURCES DIRS)
	if(MSVC)
		foreach(FILE ${SOURCES})
		    # Get the directory of the source file
		    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

		    set(GROUP ${PARENT_DIR})

		    # Remove common directory prefix to make the group
		    foreach(DIR ${DIRS})
		    	string(REPLACE "${DIR}" "" GROUP "${GROUP}")
		    endforeach()

		    # Make sure we are using windows slashes
		    string(REPLACE "/" "\\" GROUP "${GROUP}")

		    # Group into "Source Files" and "Header Files"
		    if ("${FILE}" MATCHES ".*\\.(cpp|CPP)")
		       set(GROUP "Source Files\\${GROUP}")
		    elseif("${FILE}" MATCHES ".*\\.(h|H)")
		       set(GROUP "Header Files\\${GROUP}")
		    elseif("${FILE}" MATCHES ".*\\.(rc|RC)")
		       set(GROUP "Resource Files\\${GROUP}")
		    endif()

		    source_group("${GROUP}" FILES "${FILE}")
		endforeach()
	endif()
endmacro()
