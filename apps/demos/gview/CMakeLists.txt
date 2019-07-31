set(SOURCES
    gview.c++
    DisplayGraph.c++
    Error.c++
    FieldEditor.c++
    GraphIcon.c++
    GraphViewer.c++
    NodeCreator.c++
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/gviewIcons.iv.h
      COMMAND $<TARGET_FILE:ivToIncludeFile> gviewIcons < ${CMAKE_CURRENT_SOURCE_DIR}/gviewIcons.iv > ${CMAKE_CURRENT_BINARY_DIR}/gviewIcons.iv.h
      VERBATIM
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/gviewIcons.iv ivToIncludeFile
      COMMENT "Creating gviewIcons.iv.h"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(gview ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/gviewIcons.iv.h)
target_link_libraries(gview Inventor InventorXt)
