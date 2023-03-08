cmake_minimum_required(VERSION 3.22)
set(Header_Files
    "src/INIReader.h"
    "src/Utils.h"
    "src/Callbacks.h"
    "src/Camera.h"
    "src/Renderer.h"
    "src/Shader.h"
    "src/Transform.h"
    "src/Drawables/Drawable.h"
    "src/Drawables/Sphere.h"
    "src/Drawables/Cube.h"
    "src/Drawables/Cylinder.h"
    "src/Drawables/Torus.h"
    "src/Vertex.h"
    "src/Lights/Light.h"
    "src/Lights/PointLight.h"
    "src/Lights/DirectionalLight.h"
    "src/ShaderManager.h"
    "src/Lights/SpotLight.h"
)
source_group("Header Files" FILES ${Header_Files})
set(Source_Files
    "src/Main.cpp"
    "src/Camera.cpp"
    "src/Callbacks.cpp"
    "src/Renderer.cpp"
    "src/Shader.cpp"
    "src/Transform.cpp"
    "src/Drawables/Sphere.cpp"
    "src/Drawables/Cube.cpp"
    "src/Drawables/Cylinder.cpp"
    "src/Drawables/Torus.cpp"
    "src/Lights/PointLight.cpp"
    "src/Lights/DirectionalLight.cpp"
    "src/ShaderManager.cpp"
    "src/Lights/SpotLight.cpp"
)
source_group("Source Files" FILES ${Source_Files})
set(All_Sources ${Header_Files} ${Source_Files})
