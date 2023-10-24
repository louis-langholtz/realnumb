file(GLOB REALNUMB_SRCS "source/realnumb/*.cpp")
file(GLOB REALNUMB_HDRS "include/realnumb/*.hpp")

if (${REALNUMB_ENABLE_COVERAGE} AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	message(STATUS "lib: Adding definitions for coverage analysis.")
	add_definitions(--coverage)
endif()

set(libinc
	include/realnumb/fixed.hpp
	include/realnumb/fixed_limits.hpp
	include/realnumb/fixed_math.hpp
	)
set(libsrc
)

#add_library(realnumb ${libsrc})
add_library(realnumb INTERFACE ${libinc})
add_library(realnumb::realnumb ALIAS realnumb)
#target_compile_options(realnumb PRIVATE
#	$<$<CXX_COMPILER_ID:MSVC>:/W4>
#	$<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Werror>
#	$<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Werror>
#	$<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wextra -Wpedantic -Werror>
#)
#target_compile_features(realnumb PUBLIC cxx_std_20)
set_target_properties(realnumb PROPERTIES
	OUTPUT_NAME "realnumb"
	VERSION ${REALNUMB_VERSION}
	SOVERSION ${REALNUMB_VERSION})
target_include_directories(realnumb INTERFACE
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include/>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

if(REALNUMB_INSTALL)
	include(GNUInstallDirs)
	include(CMakePackageConfigHelpers)

	# install headers
	install(FILES ${libinc}
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/realnumb
		COMPONENT Library)

	# install libraries
	install(TARGETS realnumb
		EXPORT realnumb-targets
		COMPONENT Library
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

	# install build system hooks for third-party apps
	install(EXPORT realnumb-targets
		FILE realnumb-targets.cmake
		NAMESPACE realnumb::
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/realnumb)

	set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}
		CACHE PATH "Location of header files")
	configure_package_config_file(realnumbConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/realnumbConfig.cmake
		INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/realnumb
		PATH_VARS INCLUDE_INSTALL_DIR
	)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/realnumbConfig.cmake
		DESTINATION ${LIB_INSTALL_DIR}/cmake/realnumb)
endif()
