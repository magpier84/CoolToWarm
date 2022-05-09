CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
CMAKE_POLICY(VERSION 3.0)

project(CoolToWarm VERSION 0.1.0)

set(PROJECT_VENDOR "Ruslan Sorokin")
set(PROJECT_COPYRIGHT_YEAR "2018")

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
set(CMAKE_INSTALL_UCRT_LIBRARIES 1)

if (MSVC)
    add_definitions(-DNOMINMAX)
endif(MSVC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(APPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR})
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/Bin)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)

string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
get_filename_component(3RD_PARTY_DIR "${CMAKE_BINARY_DIR}/Thirdparty" ABSOLUTE)
set(3RD_PARTY_BUILD_DIR    "${3RD_PARTY_DIR}/build/${BUILD_TYPE}")
set(3RD_PARTY_INSTALL_DIR  "${3RD_PARTY_DIR}/build/${BUILD_TYPE}")
set(3RD_PARTY_DOWNLOAD_DIR "${3RD_PARTY_DIR}/download")
set(3RD_PARTY_SOURCES_DIR  "${3RD_PARTY_DIR}/src")

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories("${3RD_PARTY_INSTALL_DIR}/include")
include_directories("${CMAKE_BINARY_DIR}")

add_definitions(-Dcimg_use_png)
add_definitions(-Dcimg_display=0)

include(Thirdparty.cmake)

add_subdirectory(${CMAKE_SOURCE_DIR}/Thirdparty/zlib)
add_subdirectory(${CMAKE_SOURCE_DIR}/Thirdparty/png)

file(GLOB_RECURSE SRC *.h *.cpp *.ui)
file(GLOB FORMS . *.ui)
QT5_WRAP_UI(FORMS_HEADERS ${FORMS})

set(APPLICATION_NAME ${PROJECT_NAME})

add_executable(${APPLICATION_NAME} WIN32 ${SRC})
target_link_libraries(${APPLICATION_NAME} png libz Qt5::Widgets)

# Install

macro(install_qt_plugins PLUGINS DESTINATION_DIR)
    foreach(plugin ${PLUGINS})
        unset(PLUGIN_LOCATION)
        if(TARGET "${plugin}")
            get_target_property(PLUGIN_LOCATION ${plugin} LOCATION)
            install(FILES ${PLUGIN_LOCATION} DESTINATION ${DESTINATION_DIR})
        elseif(NOT "${CMAKE_BUILD_TYPE}" STREQUAL DEBUG)
            message(FATAL_ERROR "Missing plugin ${plugin}")
        endif()
    endforeach()
endmacro()

set(QTCONF_DIR Bin)
set(PLUGINS_DIR Bin)
set(BUNDLE "\${CMAKE_INSTALL_PREFIX}/Bin/${APPLICATION_NAME}.exe")

get_target_property(QT_LIBRARY_DIR Qt5::Core LOCATION)
get_filename_component(QT_LIBRARY_DIR ${QT_LIBRARY_DIR} PATH)
get_filename_component(QT_LIBRARY_DIR "${QT_LIBRARY_DIR}/../bin" ABSOLUTE)

set(PLATFORM_INTEGRATION_PLUGIN Qt5::QWindowsIntegrationPlugin)

install(TARGETS ${APPLICATION} DESTINATION .)
install_qt_plugins("${PLATFORM_INTEGRATION_PLUGIN}" ${PLUGINS_DIR}/platforms)

INCLUDE(InstallRequiredSystemLibraries)

INSTALL(CODE "
    FILE(WRITE \"\${CMAKE_INSTALL_PREFIX}/${QTCONF_DIR}/qt.conf\" \"\")")

install(CODE "
        file(GLOB_RECURSE PLUGINS \"${CMAKE_INSTALL_PREFIX}/${PLUGINS_DIR}/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
        message(PLUGINS: ${PLUGINS})
        include(BundleUtilities)
        fixup_bundle(\"${BUNDLE}\" \"\${PLUGINS}\" \"${QT_LIBRARY_DIR}\")")

