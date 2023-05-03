cmake_minimum_required(VERSION 3.16)
project(conf)

if(NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

include(cmake/pkg.cmake)

file(GLOB_RECURSE CONF_SOURCES "src/*.cc")
add_library(conf STATIC ${CONF_SOURCES})
target_compile_features(conf PUBLIC cxx_std_17)
target_include_directories(conf PUBLIC include)
target_link_libraries(conf boost-program_options)

if(MSVC)
  set(_def
    _VARIADIC_MAX=10
    _WIN32_WINNT=0x0501
  )
  set_target_properties(conf PROPERTIES COMPILE_DEFINITIONS "${_def}")
elseif(MINGW)
  target_link_libraries(conf -Wl,-Bstatic)
  set(_def
    BOOST_THREAD_USE_LIB
    WINVER=0x0501
    _WIN32_WINNT=0x0501
    _WIN32_IE=0x0501
  )
  set_target_properties(conf PROPERTIES COMPILE_DEFINITIONS "${_def}")
else()
  set_target_properties(conf PROPERTIES COMPILE_FLAGS "-Wall -Wextra")
endif()

add_executable(conf-example EXCLUDE_FROM_ALL example/main.cc)
target_link_libraries(conf-example conf)

file(GLOB_RECURSE conf-test-files test/*.cc)
add_executable(conf-test EXCLUDE_FROM_ALL ${conf-test-files})
target_link_libraries(conf-test conf)
