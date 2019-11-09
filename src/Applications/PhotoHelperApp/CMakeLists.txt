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

file(GLOB NAME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

qt5_add_resources(QT_RESOURCES PhotoHelperApp.qrc)

add_executable(${PROJECT_NAME} ${NAME_SOURCES} ${QT_RESOURCES})

target_link_libraries(${PROJECT_NAME}
  ${QTLIBRARIES}
  PhotoHelper
)
