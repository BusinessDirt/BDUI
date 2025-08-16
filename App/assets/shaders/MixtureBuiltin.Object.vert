#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

layout(set = 0, binding = 0) uniform GlobalUniformBuffer {
    mat4 View;
    mat4 Projection;
} globalUBO;

void main() 
{
    gl_Position = globalUBO.Projection * globalUBO.View * vec4(inPosition, 1.0);
    fragColor = inColor;
}