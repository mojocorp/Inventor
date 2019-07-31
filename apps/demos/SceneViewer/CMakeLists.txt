set(HEADERS
    SoSceneMenu.h
    SoSceneViewer.h
    SvManipList.h
)

set(SOURCES
    SceneViewer.c++
    SoSceneViewer.c++
    SvManipList.c++
)

add_executable(SceneViewer ${HEADERS} ${SOURCES})
target_link_libraries(SceneViewer Inventor InventorXt InventorWidget)

install(TARGETS SceneViewer DESTINATION bin)