set(HEADERS
    ButtonBoxEvent.h
    DialEvent.h
    DialNButton.h
)

set(SOURCES
    ButtonBoxEvent.c++
    DialEvent.c++
    DialNButton.c++
    eventTest.c++
)

add_executable(eventTest ${HEADERS} ${SOURCES})
target_link_libraries(eventTest Inventor InventorXt)
install(TARGETS eventTest DESTINATION bin/examples/Toolmaker)
