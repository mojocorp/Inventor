add_executable(jumpingJackKitTest JumpingJackKit.h JumpingJackKit.c++ jumpingJackKitTest.c++)
target_link_libraries(jumpingJackKitTest Inventor InventorXt)
install(TARGETS jumpingJackKitTest DESTINATION bin/examples/Toolmaker)

add_executable(pyramidKitTest PyramidKit.h PyramidKit.c++ pyramidKitTest.c++ ../02.Nodes/Pyramid.c++)
target_link_libraries(pyramidKitTest Inventor InventorXt)
install(TARGETS pyramidKitTest DESTINATION bin/examples/Toolmaker)
