cmake_minimum_required(VERSION 3.16)

project(MimeApp
  VERSION 0.1
  LANGUAGES CXX
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS Quick WaylandClient REQUIRED)

find_package(SessionLockQt REQUIRED)

qt_add_executable(appMimeApp
  src/main.cpp
  src/Command.h
  src/Command.cpp
  src/WindowElement.h
  src/WindowElement.cpp
)

# Not Format Me
qt_add_qml_module(appMimeApp
  URI MimeApp
  VERSION 1.0
  QML_FILES
    ./qml/main.qml
)

set_target_properties(appMimeApp PROPERTIES
  MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE)

target_link_libraries(appMimeApp
  PRIVATE
  Qt6::Quick
  Qt6::WaylandClient
  SessionLockQtInterface)

install(TARGETS appMimeApp
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
