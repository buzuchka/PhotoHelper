project(PhotoHelperApp)

find_package(Qt5 COMPONENTS
  Widgets
  Quick
  Qml REQUIRED)

include_directories(
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Quick_INCLUDE_DIRS}
  ${Qt5Qml_INCLUDE_DIRS})

set(QTLIBRARIES Qt5::Widgets Qt5::Quick Qt5::Qml)

file(GLOB NAME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE RC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.rc")

qt5_add_resources(QT_RESOURCES "resources/PhotoHelperApp.qrc")

add_executable(${PROJECT_NAME} ${NAME_SOURCES} ${RC_FILES} ${QT_RESOURCES})

target_link_libraries(${PROJECT_NAME}
  ${QTLIBRARIES}
  PhotoHelper
)
