set(HEADERS
    Interface.h
    LineManip.h
    MyGrids.h
    NoodleSlider.h
    NoodleSurfaceGizmo.h
    NoodleTextureGizmo.h
    PullDowns.h
    WorldInfo.h
)

set(SOURCES
    NoodleSlider.c++
    Interface.c++
    NoodleSurfaceGizmo.c++
    WorldInfo.c++
    LineManip.c++
    NoodleTextureGizmo.c++
    noodle.c++
    LineManipHilight.c++
    profile.c++
)

add_executable(noodle ${HEADERS} ${SOURCES})
target_link_libraries(noodle Inventor InventorXt GeneralizedCylinder)
install(TARGETS noodle DESTINATION bin/demos)
