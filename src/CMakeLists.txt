cmake_minimum_required(VERSION 3.5)

# Set Version, Release need cmake
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)

# use git version as library version
find_package(Git QUIET)
if (Git_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../.git")
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-list --count HEAD
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE _git_version
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%h
            OUTPUT_VARIABLE PROJECT_GIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
            WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
    )
else ()
    set(_git_version 0)
    set(PROJECT_GIT_HASH "")
endif ()

project(YFEL VERSION 0.0.${_git_version} LANGUAGES CXX)

option(CMAKE_QT_CLION "Using CLion develop" OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_BINARY_DIR}/yfel_config.h"
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    configure_file(
            "${PROJECT_SOURCE_DIR}/assets/YFEL.rc.in"
            "${PROJECT_BINARY_DIR}/YFEL.rc"
    )
    set(WIN32_RESOURCES ${PROJECT_BINARY_DIR}/YFEL.rc)
endif ()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Concurrent)

set(I18N_TS_FILES "${PROJECT_SOURCE_DIR}/i18n/YFEL_zh_CN.ts")
set_source_files_properties(${I18N_TS_FILES} PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/i18n")

file(GLOB PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB CHIP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/chips/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/chips/*.h)

set(
        PROJECT_SOURCES
        mainwindow.ui
        tpr13dialog.ui
        icon.qrc

        ${PROJECT_SOURCES}
        ${CHIP_SOURCES}
        ${I18N_TS_FILES}
        ${PROJECT_BINARY_DIR}/yfel_config.h
        ${WIN32_RESOURCES}

        lib/QHexView/QHexView.cpp
        lib/QHexView/QHexView.h
        utils.h)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(YFEL MANUAL_FINALIZATION ${PROJECT_SOURCES})

    qt_add_lupdate(YFEL TS_FILES ${I18N_TS_FILES})
    qt_add_lrelease(YFEL
            TS_FILES ${I18N_TS_FILES}
            QM_FILES_OUTPUT_VARIABLE qm_files)
    qt_add_resources(YFEL "translations"
            PREFIX "/"
            BASE "${CMAKE_CURRENT_BINARY_DIR}"
            FILES "${qm_files}")
else ()
    if (ANDROID)
        add_library(YFEL SHARED ${PROJECT_SOURCES})
    else ()
        add_executable(YFEL ${PROJECT_SOURCES})
    endif ()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif ()

if (WIN32)
    target_link_libraries(YFEL PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent
            ${CMAKE_SOURCE_DIR}/lib/libusb/libusb-MinGW-x64/lib/libusb-1.0.a)
    target_include_directories(YFEL PRIVATE ${CMAKE_SOURCE_DIR}/lib/libusb/libusb-MinGW-x64/include)
elseif (UNIX)
    target_link_libraries(YFEL PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent usb-1.0)
else()
    message(FAIL "Not support platform")
endif ()

set_target_properties(
        YFEL PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER yfel.gloomyghost.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(YFEL)
endif ()

if (WIN32)
    if (CMAKE_QT_CLION)
        set(DEBUG_SUFFIX)
        if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
            set(DEBUG_SUFFIX "d")
        endif ()
        set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
            if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
                set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
            endif ()
        endif ()
        if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        endif ()
        foreach (QT_LIB Core Gui Widgets)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
        endforeach (QT_LIB)
    endif ()
endif ()

install(TARGETS YFEL
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(FILES ${PROJECT_BINARY_DIR}/yfel_config.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

install(FILES ${qm_files}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/locale)

install(FILES ${PROJECT_SOURCE_DIR}/assets/applications/io.github.yuzukitsuru.yfel.desktop
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)

install(FILES ${PROJECT_SOURCE_DIR}/assets/img/yfel.png
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/pixmaps)

