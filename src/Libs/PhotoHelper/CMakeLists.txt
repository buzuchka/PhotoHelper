project(PhotoHelper)

find_package(Qt5Widgets QUIET)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
set(QTLIBRARIES ${Qt5Widgets_LIBRARIES})

file(GLOB_RECURSE INCLUDE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h**")
file(GLOB_RECURSE HEADER_FILES  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h**")
file(GLOB_RECURSE SOURCE_FILES  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

qt5_add_resources(QT_RESOURCES resources/PhotoHelper.qrc)

add_library(${PROJECT_NAME} STATIC
  ${INCLUDE_FILES}
  ${HEADER_FILES}
  ${SOURCE_FILES}
  ${QT_RESOURCES})

target_include_directories(${PROJECT_NAME}
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  ${CMAKE_CURRENT_BINARY_DIR}
  ${EXPAT_INCLUDE_DIRS}
PRIVATE
  ${CMAKE_BINARY_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/include/PhotoHelper"
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
  ${QTLIBRARIES}
  ${ZLIB_LIBRARIES}
  ${EXPAT_LIBRARIES}
  exiv2lib
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  AUTOMOC       ON
)

generate_export_header(${PROJECT_NAME})
