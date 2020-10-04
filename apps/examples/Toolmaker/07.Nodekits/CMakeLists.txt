add_executable(jumpingJackKitTest JumpingJackKit.h JumpingJackKit.c++ jumpingJackKitTest.c++)
target_link_libraries(jumpingJackKitTest Inventor InventorXt)

add_executable(pyramidKitTest PyramidKit.h PyramidKit.c++ pyramidKitTest.c++ ../02.Nodes/Pyramid.c++)
target_link_libraries(pyramidKitTest Inventor InventorXt)
