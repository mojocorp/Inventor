add_executable(ivcat ivcat.c++)
target_link_libraries(ivcat Inventor)

install(TARGETS ivcat DESTINATION bin)