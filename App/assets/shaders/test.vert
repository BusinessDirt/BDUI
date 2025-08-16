#version 450

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec4 inUV;
layout(location = 3) in uint inType;

layout(location = 0) out vec4 fragUV;
layout(location = 1) out vec4 fragColor;
layout(location = 2) out flat uint shapeType;

void main() {
    fragUV = inUV;
    fragColor = inColor;
    shapeType = inType;
    gl_Position = vec4(inPos, 0.0, 1.0);
}
