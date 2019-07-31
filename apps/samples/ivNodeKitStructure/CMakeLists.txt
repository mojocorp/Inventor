add_executable(ivNodeKitStructure ivNodeKitStructure.c++)
target_link_libraries(ivNodeKitStructure Inventor InventorXt)

# necessary for dlerror()
if(UNIX AND NOT APPLE)
    target_link_libraries(ivNodeKitStructure dl)
endif()