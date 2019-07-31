set(HEADERS 
    image.h
)

set(SOURCES
    close.c
    filbuf.c
    flsbuf.c
    name.c
    open.c
    pix.c
    rdwr.c
    rle.c
    row.c
)

add_library (image STATIC ${HEADERS} ${SOURCES})
target_include_directories (image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})