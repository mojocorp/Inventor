add_executable(ivinfo ivinfo.c++)
target_link_libraries(ivinfo Inventor)

install(TARGETS ivinfo DESTINATION bin)