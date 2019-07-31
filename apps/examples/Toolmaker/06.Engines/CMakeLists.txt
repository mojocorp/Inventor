set(HEADERS
    ConvertSFShortToSFFloat.h
    FanIn.h
    MultFloatVec3f.h
    SoComposeVec2f.h
    SoDecomposeVec2f.h
    Ticker.c++
)

set(SOURCES
    ConvertSFShortToSFFloat.c++
    FanIn.c++
    MultFloatVec3f.c++
    SoComposeVec2f.c++
    SoDecomposeVec2f.c++
    Ticker.c++
)

add_library(engines_objects OBJECT ${SOURCES})
target_link_libraries(engines_objects Inventor)