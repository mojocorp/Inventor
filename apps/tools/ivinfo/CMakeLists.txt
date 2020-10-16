if(WIN32)
include_directories(../../compat)
endif()

add_definitions(-D_USE_MATH_DEFINES)
add_executable(ivinfo ivinfo.c++)
target_link_libraries(ivinfo Inventor)
set_target_properties(ivinfo PROPERTIES FOLDER "apps/tools")

install(TARGETS ivinfo DESTINATION bin/tools)
