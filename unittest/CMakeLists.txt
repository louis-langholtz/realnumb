# Using an installed GTest package that's not been built with same compiler may result
# in undefined symbols linker errors.
# find_package(GTest CONFIG REQUIRED)

# Hides options.
mark_as_advanced(FORCE BUILD_GMOCK BUILD_GTEST)
mark_as_advanced(FORCE INSTALL_GMOCK INSTALL_GTEST)
mark_as_advanced(FORCE gmock_build_tests)
mark_as_advanced(FORCE gtest_build_samples gtest_build_tests gtest_disable_pthreads gtest_hide_internal_symbols)
mark_as_advanced(FORCE gtest_force_shared_crt)
mark_as_advanced(FORCE CMAKE_DEBUG_POSTFIX)

set(BUILD_GMOCK ON)
set(INSTALL_GTEST OFF CACHE BOOL "Enable installation of googletest.")
set(INSTALL_GMOCK OFF CACHE BOOL "Enable installation of googlemock.")

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(googletest)

file(GLOB Test_SRCS *.cpp)

# Add an executable to the project using specified source files.
# See details at: https://cmake.org/cmake/help/v3.1/command/add_executable.html
add_executable(unittest ${Test_SRCS})

# Link a target to given libraries.
# See details at: https://cmake.org/cmake/help/v3.1/command/target_link_libraries.html
#target_link_libraries(unittest realnumb::realnumb GTest::gtest GTest::gtest_main)
#target_link_libraries(unittest realnumb::realnumb GTest::gtest_main)
target_link_libraries(unittest PUBLIC realnumb::realnumb)
target_link_libraries(unittest PUBLIC GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unittest)

if(REALNUMB_INSTALL)
	include(GNUInstallDirs)
	install(TARGETS unittest
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		COMPONENT Applications)
endif()
