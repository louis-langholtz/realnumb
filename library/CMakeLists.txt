file(GLOB REALNUMB_SRCS "source/realnumb/*.cpp")
file(GLOB REALNUMB_HDRS "include/realnumb/*.hpp")
file(GLOB EXT_SRCS "source/ext/*.cpp")
file(GLOB EXT_HDRS "include/ext/*.hpp")

if (${REALNUMB_ENABLE_COVERAGE} AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	message(STATUS "lib: Adding definitions for coverage analysis.")
	add_definitions(--coverage)
endif()

set(libsrc
	${REALNUMB_SRCS}
	${REALNUMB_HDRS}
	${EXT_SRCS}
	${EXT_HDRS}
)

add_library(realnumb ${libsrc})
add_library(realnumb::realnumb ALIAS realnumb)

target_compile_options(realnumb PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Werror>
  $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Werror>
  $<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wextra -Wpedantic -Werror>
)

target_compile_features(realnumb PUBLIC cxx_std_20)
set_target_properties(realnumb PROPERTIES
	OUTPUT_NAME "realnumb"
	VERSION ${REALNUMB_VERSION}
	SOVERSION ${REALNUMB_VERSION})
target_include_directories(realnumb PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include/>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
