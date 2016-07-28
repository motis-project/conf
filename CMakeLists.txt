cmake_minimum_required(VERSION 2.6)
project(conf)

# if (MSVC)
#   set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${BOOST_ROOT}\\stage\\lib")
#   link_directories(${CMAKE_LIBRARY_PATH})
# endif()

if (MSVC)
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_MULTITHREADED ON)
  set(Boost_USE_STATIC_RUNTIME ON)
endif()

# stolen from gtest
if (MSVC)
  # For MSVC, CMake sets certain flags to defaults we want to override.
  # This replacement code is taken from sample in the CMake Wiki at
  # http://www.cmake.org/Wiki/CMake_FAQ#Dynamic_Replace.
  foreach (flag_var
           CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
           CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if (NOT BUILD_SHARED_LIBS AND NOT gtest_force_shared_crt)
      # When Google Test is built as a shared library, it should also use
      # shared runtime libraries.  Otherwise, it may end up with multiple
      # copies of runtime library data in different modules, resulting in
      # hard-to-find crashes. When it is built as a static library, it is
      # preferable to use CRT as static libraries, as we don't have to rely
      # on CRT DLLs being available. CMake always defaults to using shared
      # CRT libraries, so we override that default here.
      string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
    endif()

    # We prefer more strict warning checking for building Google Test.
    # Replaces /W3 with /W4 in defaults.
    string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")
  endforeach()
endif()

find_package(Boost COMPONENTS system program_options REQUIRED)
link_directories(${Boost_LIBRARY_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

include_directories("./include")

file(GLOB_RECURSE CONF_SOURCES "src/*.cc")
add_library(conf STATIC ${CONF_SOURCES})

if(MSVC)
  target_link_libraries(conf ${Boost_LIBRARIES})
  set(_def
    _VARIADIC_MAX=10
    _WIN32_WINNT=0x0501
  )
  set_target_properties(conf PROPERTIES COMPILE_DEFINITIONS "${_def}")
elseif(MINGW)
  target_link_libraries(conf -Wl,-Bstatic ${Boost_LIBRARIES})
  set_target_properties(conf PROPERTIES COMPILE_FLAGS "-std=c++11")
  set(_def
    BOOST_THREAD_USE_LIB
    WINVER=0x0501
    _WIN32_WINNT=0x0501
    _WIN32_IE=0x0501
  )
  set_target_properties(conf PROPERTIES COMPILE_DEFINITIONS "${_def}")
else()
  set_target_properties(conf PROPERTIES COMPILE_FLAGS "-Wall -Wextra -std=c++11")
  target_link_libraries(conf ${Boost_LIBRARIES})
endif()

add_executable(conf-example EXCLUDE_FROM_ALL example/main.cc)
target_link_libraries(conf-example ${Boost_LIBRARIES} conf)
set_target_properties(conf-example PROPERTIES COMPILE_FLAGS "-std=c++11")

file(GLOB_RECURSE conf-test-files test/*.cc)
add_executable(conf-test EXCLUDE_FROM_ALL ${conf-test-files})
target_link_libraries(conf-test ${Boost_LIBRARIES} conf)
set_target_properties(conf-test PROPERTIES COMPILE_FLAGS "-std=c++11")
