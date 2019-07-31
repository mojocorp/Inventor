find_package(X11 REQUIRED)

set(HF MyExaminerViewer.h MyFullViewer.h MyViewer.h)
set(CF MyExamVwr.c++ MyFullVwr.c++ MyViewer.c++ MyBitmapButton.c++ MyBitmapButton.h)

foreach(file ${HF})
    string(REPLACE "My" "SoXt" src "../../../libSoXt/include/Inventor/Xt/viewers/${file}")
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${file}
          COMMAND sed -f ./nameScript ${src} > ${CMAKE_CURRENT_SOURCE_DIR}/${file}
          VERBATIM
          DEPENDS ${src}
          COMMENT "Creating ${file}"
          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

foreach(file ${CF})
    string(REPLACE "My" "SoXt" src "../../../libSoXt/src/viewers/${file}")
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${file}
          COMMAND sed -f ./nameScript ${src} > ${CMAKE_CURRENT_SOURCE_DIR}/${file}
          VERBATIM
          DEPENDS ${src}
          COMMENT "Creating ${file}"
          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

include_directories(${X11_INCLUDE_DIR} "../../../libSoXt/src/viewers")
add_executable(viewerTest viewerTest.c++ ${HF} ${CF})
target_link_libraries(viewerTest Inventor InventorXt ${X11_LIBRARIES})