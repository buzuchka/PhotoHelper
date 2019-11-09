project(PhotoHelper)

find_package(Qt5Widgets QUIET)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
set(QTLIBRARIES ${Qt5Widgets_LIBRARIES})

file(GLOB_RECURSE INCLUDE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h**")
file(GLOB_RECURSE HEADER_FILES  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h**")
file(GLOB_RECURSE SOURCE_FILES  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${INCLUDE_FILES} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  ${CMAKE_CURRENT_BINARY_DIR}
PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
  ${QTLIBRARIES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  AUTOMOC       ON
)

generate_export_header(${PROJECT_NAME})
