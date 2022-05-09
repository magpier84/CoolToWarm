project(libpng)

set(ARGS -DCMAKE_INSTALL_PREFIX:PATH=${3RD_PARTY_INSTALL_DIR}
         -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
         -DBUILD_SHARED_LIBS:BOOL=FALSE
         -DPNG_SHARED:BOOL=FALSE
         _DPNG_TESTS:BOOL=FALSE)

Thirdparty_Add_CMake("${CMAKE_SOURCE_DIR}/Thirdparty/libpng-1.6.25.tar.xz" "" "${ARGS}")

add_library(png STATIC IMPORTED GLOBAL)

if(UNIX OR MINGW)
    set(libPng "${3RD_PARTY_INSTALL_DIR}/lib/libpng.a")
elseif (MSVC)
    set(libPng "${3RD_PARTY_INSTALL_DIR}/lib/libpng16_static.lib")
    set(libPngd "${3RD_PARTY_INSTALL_DIR}/lib/libpng16_staticd.lib")
endif()

set_target_properties(png PROPERTIES IMPORTED_LOCATION ${libPng})

if(MSVC)
    set_target_properties(png PROPERTIES IMPORTED_LOCATION_DEBUG ${libPngd})
endif()

add_dependencies(png ${PROJECT_NAME})

