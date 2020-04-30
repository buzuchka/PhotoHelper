cmake_minimum_required(VERSION 3.1.0)

project(PhotoHelperSolution CXX)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
    message("GNUCXX or GNUCC compiler!")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lgomp -g -fPIC")
elseif (MSVC)
    message("MSVC compiler!")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3 /MP /openmp /volatile:ms")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /MP /openmp /volatile:ms")
else()
    message("Unknown compiler")
endif()

# отключение Q_ASSERT & qDebug() в Release
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_RELEASE "-DQT_NO_DEBUG -DQT_NO_DEBUG_OUTPUT ${CMAKE_CXX_FLAGS_RELEASE}")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/DQT_NO_DEBUG /DQT_NO_DEBUG_OUTPUT ${CMAKE_CXX_FLAGS_RELEASE}")
endif()

set(CMAKE_BUILD_WITH_INSTALL_RPATH true)
set(CMAKE_INSTALL_RPATH ".")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include(GenerateExportHeader)

add_subdirectory(3rdparty)
add_subdirectory(src)
