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

add_executable(noodle ${SOURCES})
target_link_libraries(noodle Inventor InventorXt GeneralizedCylinder)