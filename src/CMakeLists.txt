set( VLIGHT_SOURCES
    Aeroplane.cpp
    Circle.cpp
    dice.cpp
    Engine.cpp
    Engine_drawing.cpp
    Engine_logic.cpp
    Face.cpp
    Font.cpp
    fpsMeter.cpp
    includeShortcut.cpp
    Label.cpp
    main.cpp
    Quaternion.cpp
    SHADER.cpp
    Terrain.cpp
    Texture.cpp
    Vertex.cpp
)

set( VLIGHT_BINNAME "vlight" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

add_executable( ${VLIGHT_BINNAME} ${VLIGHT_SOURCES} )
target_link_libraries( ${VLIGHT_BINNAME} SDL GL GLEW pthread )
