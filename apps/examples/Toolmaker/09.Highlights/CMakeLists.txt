add_executable(overlayTest overlayTest.c++ OverlayHL.c++)
target_link_libraries(overlayTest Inventor InventorXt)

add_executable(selectionTest selectionTest.c++ ShowSelectionRA.c++)
target_link_libraries(selectionTest Inventor InventorXt)