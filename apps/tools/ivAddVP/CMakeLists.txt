add_executable(ivAddVP ivAddVP.c++)
target_link_libraries(ivAddVP Inventor)
set_target_properties(ivAddVP PROPERTIES FOLDER "apps/tools")

set_target_properties(ivAddVP PROPERTIES FOLDER bin/tools)
