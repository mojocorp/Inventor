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

add_executable(drop ${SOURCES})
target_link_libraries(drop Inventor InventorXt)