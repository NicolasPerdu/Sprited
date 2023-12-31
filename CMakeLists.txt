cmake_minimum_required(VERSION 3.5)

project(Sprited VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Sprited_ui MainWindow.ui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTORCC ON)

set(MACOSX_BUNDLE_ICON_FILE Sprited.icns)

set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/Sprited.icns")
set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB SOURCES
        src/*.cpp
	src/pack/*.cpp
	src/tga/*.cpp
        include/*.hpp
	include/*.h
)

file(GLOB UI
        ui/*.ui)

qt_wrap_ui(FORMS_HEADERS ${UI})

qt_add_executable(Sprited MANUAL_FINALIZATION ${SOURCES} ${FORMS_HEADERS} ${app_icon_macos})

target_link_libraries(Sprited PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Sprited)
endif()
set_target_properties(Sprited PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Sprited
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Sprited)
endif()
