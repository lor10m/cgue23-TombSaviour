#version 450 core

in float Height;
in vec4 textPos;

uniform sampler2D surfaceTexture;

out vec4 FragColor;

void main()
{
    vec2 texCoord = textPos.xz;// / (1024 / 20);
    float h = (Height) / 205.0f;
    vec4 terrainColor = texture(surfaceTexture, texCoord) * h;
    FragColor = vec4(terrainColor.rgb, 1.0);
    //FragColor = vec4(h, h, 0, 1.0) * vec4(lightIntensity, 1.0f);
}