set(HEADERS
    Coordinate3Manip.h
    RotTransDragger.h
    RotTransDraggerGeom.h
    RotTransManip.h
    TranslateRadialDragger.h
    TranslateRadialManip.h
    TranslateRadialDraggerGeom.h
)

set(SOURCES
    Coordinate3Manip.c++
    RotTransDragger.c++
    RotTransManip.c++
    TranslateRadialDragger.c++
    TranslateRadialManip.c++
    newManips.c++
)

add_executable(newManips ${HEADERS} ${SOURCES})
target_link_libraries(newManips Inventor InventorXt)
