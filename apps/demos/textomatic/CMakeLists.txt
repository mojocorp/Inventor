find_package(Iconv REQUIRED)

set(HEADERS
    TextGraph.h
    TextWrapper.h
)
set(SOURCES
    textomatic.c++
    TextGraph.c++
    TextWrapper.c++
)

add_executable(textomatic ${SOURCES})
target_link_libraries(textomatic Inventor InventorXt common ${Iconv_LIBRARIES})

# necessary for locale_charset()
if(APPLE)
    target_link_libraries(textomatic charset)
endif()