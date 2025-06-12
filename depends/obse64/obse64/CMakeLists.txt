cmake_minimum_required(VERSION 3.18)

# ---- Project ----

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/versioning.cmake)

project(
	obse64
	VERSION ${OBSE_VERSION_MAJOR}.${OBSE_VERSION_MINOR}.${OBSE_VERSION_PATCH}
	LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
)
endif()

# ---- Build options ----

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

# ---- Dependencies ----

if (NOT TARGET obse64_common)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../obse64_common obse64_common)	# bundled
endif()

if (NOT TARGET xbyak)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../xbyak xbyak)	# bundled
endif()

# ---- Add source files ----

file(GLOB headers CONFIGURE_DEPENDS *.h)
file(GLOB sources CONFIGURE_DEPENDS *.cpp)

source_group(
	${PROJECT_NAME}
	FILES
		obse64.cpp
		obse64.def
		obse64_version.rc
)

source_group(
	${PROJECT_NAME}/game
	FILES
		GameConsole.cpp
		GameConsole.h
		GameScript.cpp
		GameScript.h
		GameData.cpp
		GameData.h
		GameTypes.cpp
		GameTypes.h
		GameRTTI.cpp
		GameRTTI.h
		GameRTTI.inl
		GameBSExtraData.cpp
		GameBSExtraData.h
		GameExtraData.cpp
		GameExtraData.h
		GameForms.cpp
		GameForms.h
		GameObjects.cpp
		GameObjects.h
)

source_group(
	${PROJECT_NAME}/altar
	FILES
		AltarSync.cpp
		AltarSync.h
		AltarExtraData.cpp
		AltarExtraData.h
		STLTypes.h
)

source_group(
	${PROJECT_NAME}/ue
	FILES
		UnrealAPI.cpp
		UnrealAPI.h
		UnrealObject.cpp
		UnrealObject.h
		UnrealTypes.cpp
		UnrealTypes.h
)

source_group(
	${PROJECT_NAME}/ni
	FILES
		NiTypes.h
)

source_group(
	${PROJECT_NAME}/hooks
	FILES
		Hooks_Script.cpp
		Hooks_Script.h
		Hooks_Version.cpp
		Hooks_Version.h
		Hooks_Gameplay.cpp
		Hooks_Gameplay.h
		Hooks_Data.cpp
		Hooks_Data.h
)

source_group(
	${PROJECT_NAME}/internal
	FILES
		PluginAPI.h
		PluginManager.cpp
		PluginManager.h
		SteamInit.cpp
		SteamInit.h
		CommandTable.cpp
		CommandTable.h
		ParamInfos.cpp
		ParamInfos.h
		MersenneTwister.cpp
		MersenneTwister.h
)

source_group(
	${PROJECT_NAME}/commands
	FILES
		Commands_Inventory.cpp
		Commands_Input.cpp
		Commands_Console.cpp
		Commands_Player.cpp
		Commands_Math.cpp
		Commands_MiscReference.cpp
)

# ---- Create library ----

add_library(
	${PROJECT_NAME}
	SHARED
	${headers}
	${sources}
	obse64.def
	obse64_version.rc
	GameRTTI.inl
)

add_library(obse64::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/configuration.cmake)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
		OUTPUT_NAME "${PROJECT_NAME}_${RUNTIME_VERSION_MAJOR}_${RUNTIME_VERSION_MINOR}_${RUNTIME_VERSION_PATCH}"
)

target_compile_definitions(
	${PROJECT_NAME}
	PRIVATE
		RUNTIME
		RUNTIME_VERSION=${RUNTIME_VERSION_PACKED}
)

target_compile_features(
	${PROJECT_NAME}
	PUBLIC
		cxx_std_17
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
		$<INSTALL_INTERFACE:include>
)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
		obse64::obse64_common
		xbyak::xbyak
)

target_compile_options(
	${PROJECT_NAME}
	PUBLIC
		/wd4200
)

# ---- Configure all targets ----

set_target_properties(
	${PROJECT_NAME}
	obse64_common
	PROPERTIES
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)

# ---- Create an installable target ----

include(GNUInstallDirs)

install(
	TARGETS
		${PROJECT_NAME}
	EXPORT
		${PROJECT_NAME}-targets
)

install(
	FILES
		${headers}
	DESTINATION
		${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

configure_file(
	cmake/config.cmake.in
	${PROJECT_NAME}-config.cmake
	@ONLY
)

install(
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
	DESTINATION
		${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
	EXPORT
		${PROJECT_NAME}-targets
	NAMESPACE
		obse64::
	DESTINATION
		${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/installation.cmake)
