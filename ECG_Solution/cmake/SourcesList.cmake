cmake_minimum_required(VERSION 3.22)
set(Header_Files
    "src/Utils/INIReader.h"
    "src/Utils/Utils.h"
    "src/Utils/Callbacks.h"
    "src/Camera.h"
    "src/Shader.h"
    "src/Utils/Transform.h"
    "src/Drawables/Drawable.h"
    "src/Drawables/Cube.h"
    "src/Utils/Vertex.h"
    "src/Lights/Light.h"
    "src/Lights/PointLight.h"
    "src/Lights/DirectionalLight.h"
    "src/Lights/SpotLight.h"
)
source_group("Header Files" FILES ${Header_Files})
set(Source_Files
    "src/Main.cpp"
    "src/Camera.cpp"
    "src/Utils/Callbacks.cpp"
    "src/Shader.cpp"
    "src/Utils/Transform.cpp"
    "src/Drawables/Cube.cpp"
    "src/Lights/PointLight.cpp"
    "src/Lights/DirectionalLight.cpp"
    "src/Lights/SpotLight.cpp"
)
source_group("Source Files" FILES ${Source_Files})
set(All_Sources ${Header_Files} ${Source_Files})
