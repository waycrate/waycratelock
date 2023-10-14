cmake_minimum_required(VERSION 3.16)

project(MimeApp
  VERSION 0.1
  LANGUAGES CXX
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS Quick QuickControls2 Concurrent WaylandClient REQUIRED)

find_package(SessionLockQt REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PAM pam pam_misc pamc REQUIRED IMPORTED_TARGET)

qt_add_executable(appMimeApp
  src/main.cpp
  asserts/source.qrc
)

target_include_directories(appMimeApp PUBLIC
  ${CMAKE_SOURCE_DIR}/src
)

# Not Format Me
qt_add_qml_module(appMimeApp
  URI MimeApp
  VERSION 1.0
  SOURCES
    src/CommandLine.h
    src/CommandLine.cpp
  QML_FILES
    qml/main.qml
    qml/TimePanel.qml
    qml/MainView.qml
)

target_link_libraries(appMimeApp
  PRIVATE
  Qt6::Quick
  Qt6::WaylandClient
  Qt6::QuickControls2
  Qt6::Concurrent
  SessionLockQtInterface
  PkgConfig::PAM)

install(TARGETS appMimeApp
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
