add_executable(ivview ivviewMenus.h ivview.c++)
target_link_libraries (ivview InventorWidget Inventor InventorXt)

install(TARGETS ivview DESTINATION bin)
