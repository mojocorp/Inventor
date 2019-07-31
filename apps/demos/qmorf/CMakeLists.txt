# intermediate object library to avoid duplicated 'main' symbol.
add_library(qmorf_objects OBJECT QuadThing.c++ Background.c++ FlashBackground.c++)
target_link_libraries(qmorf_objects Inventor)

add_executable(qmorf qmorf.c++)
target_link_libraries(qmorf Inventor InventorXt qmorf_objects)