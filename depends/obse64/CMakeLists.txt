cmake_minimum_required(VERSION 3.18)

# ---- Project ----

project(
	umbrella
	LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
)
endif()

# ---- Add sub projects ----

if (NOT TARGET obse64)
	add_subdirectory(obse64)
endif()

if (NOT TARGET obse64_common)
	add_subdirectory(obse64_common)
endif()

if (NOT TARGET obse64_loader)
	add_subdirectory(obse64_loader)
endif()
