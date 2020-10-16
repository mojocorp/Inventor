if(WIN32)
include_directories(../../compat)
endif()

add_executable(ivcat ivcat.c++)
target_link_libraries(ivcat Inventor)
set_target_properties(ivcat PROPERTIES FOLDER "apps/tools")

install(TARGETS ivcat DESTINATION bin/tools)
