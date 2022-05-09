INCLUDE(ExternalProject)

macro(Thirdparty_Add_CMake url patch cmake_args)
    set(DOWNLOAD_DIR ${3RD_PARTY_DOWNLOAD_DIR})
    set(SOURCES_DIR  ${3RD_PARTY_SOURCES_DIR}/${PROJECT_NAME})
    set(BUILD_DIR    ${3RD_PARTY_BUILD_DIR}/${PROJECT_NAME})
    set(PREFIX_DIR   ${3RD_PARTY_INSTALL_DIR})

    #status file
    set(STATUS_FILE ${BUILD_DIR}/status)
    if (NOT EXISTS ${BUILD_DIR}/status)
        file(WRITE ${BUILD_DIR}/status clean)
    endif()

    #copy template file
    configure_file(${CMAKE_SOURCE_DIR}/BuildDone.cmake.in ${BUILD_DIR}/BuildDone.cmake @ONLY)

    #make config file
    configure_file(${BUILD_DIR}/status ${PREFIX_DIR}/tmp/${PROJECT_NAME}-status)

    file(READ ${STATUS_FILE} STATUS)

    if ("${STATUS}" STREQUAL "clean")
        message(STATUS "External project <${PROJECT_NAME}> needs rebuild")
        ExternalProject_Add(${PROJECT_NAME}
            BUILD_IN_SOURCE 0
            URL ${url}
            PREFIX ${PREFIX_DIR}
            DOWNLOAD_DIR ${DOWNLOAD_DIR}
            SOURCE_DIR ${SOURCES_DIR}
            BINARY_DIR ${BUILD_DIR}
            STAMP_DIR ${BUILD_DIR}/stamp
            TMP_DIR ${BUILD_DIR}/tmp
            UPDATE_COMMAND ""
            PATCH_COMMAND ${patch}
            CMAKE_ARGS ${cmake_args})

        add_custom_command(TARGET ${PROJECT_NAME}
                           POST_BUILD COMMAND ${CMAKE_COMMAND} -P ${BUILD_DIR}/BuildDone.cmake)
    else()
        message(STATUS "Skip external project <${PROJECT_NAME}>")
    endif()
endmacro()
