set(HEADERS
    MFDouble.h
    SFDouble.h
)

set(SOURCES
    MFDouble.c++
    SFDouble.c++
)

add_library(fields_objects OBJECT ${SOURCES})
target_link_libraries(fields_objects Inventor )