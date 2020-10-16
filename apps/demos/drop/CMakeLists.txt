set(HEADERS
    MotifHelp.h
    TsBlock.h
    TsField.h
    TsPiece.h
    TsPieces.h
    TsViewer.h
)

set(SOURCES
    TsBlock.c++
    TsPiece.c++
    TsPieces.c++
    TsField.c++
    TsTrickle.c++
    TsEvents.c++
    TsRemoval.c++
    TsViewer.c++
    drop.c++
)

add_executable(drop ${HEADERS} ${SOURCES})
target_link_libraries(drop Inventor InventorXt)
install(TARGETS drop DESTINATION bin/demos)
