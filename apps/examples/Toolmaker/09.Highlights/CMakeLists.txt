add_executable(overlayTest overlayTest.c++ OverlayHL.c++ OverlayHighlightRenderAction.h)
target_link_libraries(overlayTest Inventor InventorXt)
install(TARGETS overlayTest DESTINATION bin/examples/Toolmaker)

add_executable(selectionTest selectionTest.c++ ShowSelectionRA.c++ ShowSelectionRA.h)
target_link_libraries(selectionTest Inventor InventorXt)
install(TARGETS selectionTest DESTINATION bin/examples/Toolmaker)
