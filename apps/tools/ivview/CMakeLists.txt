find_package(X11 REQUIRED)

include_directories (${X11_INCLUDE_DIR})
add_executable(ivview ivviewMenus.h ivview.c++)
target_link_libraries (ivview InventorWidget Inventor InventorXt ${X11_LIBRARIES})

install(TARGETS ivview DESTINATION bin)