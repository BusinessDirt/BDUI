#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;

layout(push_constant) uniform ShapeRendererPushConstant {
    vec2 viewportSize;
} pushConstant;

layout(location = 0) out vec4 fragColor;

void main() 
{
    // Convert to clip space
    vec3 pixelToClip = vec3(2.0 / pushConstant.viewportSize.x, 2.0 / pushConstant.viewportSize.y, 1.0);
    vec3 posClip = inPosition * pixelToClip - vec3(1.0, 1.0, 0.0);

    gl_Position = vec4(posClip, 1.0);
    fragColor = inColor;
}