cmake_minimum_required(VERSION 3.16)
project(conf)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if (MSVC)
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_MULTITHREADED ON)
  set(Boost_USE_STATIC_RUNTIME ON)
endif()

find_package(Boost COMPONENTS system program_options REQUIRED)
link_directories(${Boost_LIBRARY_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIR})


file(GLOB_RECURSE CONF_SOURCES "src/*.cc")
add_library(conf STATIC ${CONF_SOURCES})
target_compile_features(conf PUBLIC cxx_std_17)
target_include_directories(conf PUBLIC include)

if(MSVC)
  target_link_libraries(conf ${Boost_LIBRARIES})
  set(_def
    _VARIADIC_MAX=10
    _WIN32_WINNT=0x0501
  )
  set_target_properties(conf PROPERTIES COMPILE_DEFINITIONS "${_def}")
elseif(MINGW)
  target_link_libraries(conf -Wl,-Bstatic ${Boost_LIBRARIES})
  set_target_properties(conf PROPERTIES COMPILE_FLAGS "-std=c++17")
  set(_def
    BOOST_THREAD_USE_LIB
    WINVER=0x0501
    _WIN32_WINNT=0x0501
    _WIN32_IE=0x0501
  )
  set_target_properties(conf PROPERTIES COMPILE_DEFINITIONS "${_def}")
else()
  set_target_properties(conf PROPERTIES COMPILE_FLAGS "-Wall -Wextra")
  target_link_libraries(conf ${Boost_LIBRARIES})
endif()

add_executable(conf-example EXCLUDE_FROM_ALL example/main.cc)
target_link_libraries(conf-example ${Boost_LIBRARIES} conf)

file(GLOB_RECURSE conf-test-files test/*.cc)
add_executable(conf-test EXCLUDE_FROM_ALL ${conf-test-files})
target_link_libraries(conf-test ${Boost_LIBRARIES} conf)
