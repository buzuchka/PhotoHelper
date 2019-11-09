project(PhotoHelperApp)

find_package(Qt5Widgets QUIET)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
set(QTLIBRARIES ${Qt5Widgets_LIBRARIES})

file(GLOB NAME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${NAME_SOURCES})

target_link_libraries(${PROJECT_NAME}
  ${QTLIBRARIES}
  PhotoHelper
)
