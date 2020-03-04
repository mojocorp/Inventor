add_executable(ivToIncludeFile ivToIncludeFile.c++)
target_link_libraries(ivToIncludeFile Inventor)

add_executable(testResults testResults.c++)
target_link_libraries(testResults Inventor InventorXt)