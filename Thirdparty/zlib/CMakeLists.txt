project(zlib)

set(ARGS -DCMAKE_INSTALL_PREFIX:PATH=${3RD_PARTY_INSTALL_DIR}
         -DBUILD_SHARED_LIBS:BOOL=FALSE
         -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE})

Thirdparty_Add_CMake("${CMAKE_SOURCE_DIR}/Thirdparty/zlib-1.2.8.tar.gz" "" "${ARGS}")

add_library(libz STATIC IMPORTED GLOBAL)

if(UNIX)
    set(zipLib "${3RD_PARTY_INSTALL_DIR}/lib/libz.a")
elseif(MINGW)
    set(zipLib "${3RD_PARTY_INSTALL_DIR}/lib/libzlibstatic.a")
elseif (MSVC)
    set(zipLib "${3RD_PARTY_INSTALL_DIR}/lib/zlibstatic.lib")
    set(zipLibd "${3RD_PARTY_INSTALL_DIR}/lib/zlibstaticd.lib")
endif()

set_target_properties(libz PROPERTIES IMPORTED_LOCATION ${zipLib})

if(MSVC)
    set_target_properties(libz PROPERTIES IMPORTED_LOCATION_DEBUG ${zipLibd})
endif()

add_dependencies(libz zlib)

