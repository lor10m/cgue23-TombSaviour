#version 450 core

in float Height;
in vec2 texCoord;

uniform sampler2D surfaceTexture;

out vec4 FragColor;

void main()
{
    float h = (Height + 16)/64.0f;
    //FragColor = vec4(h, h, h, 1.0);
    FragColor = texture(surfaceTexture, texCoord);
}