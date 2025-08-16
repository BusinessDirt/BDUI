#version 450

layout(location = 0) in vec4 fragUV;
layout(location = 1) in vec4 fragColor;
layout(location = 2) in flat uint shapeType;

layout(location = 0) out vec4 outColor;

float LineAlpha(vec2 p)
{
    // p = local coords in edge space: x = distance along edge, y = distance perpendicular
    float sd = p.y; // signed perpendicular distance
    float alpha = 0.5 - sd; 
    return clamp(alpha, 0.0, 1.0);
}

float QuadraticAlpha(vec2 p)
{
    vec2 px = dFdx(p);
    vec2 py = dFdy(p);

    float fx = (2.0 * p.x) * px.x - px.y;
    float fy = (2.0 * p.x) * py.x - py.y;

    float sd = (p.x * p.x - p.y) / sqrt(fx * fx + fy * fy);
    return clamp(0.5 - sd, 0.0, 1.0);
}

float CubicAlpa(vec4 p)
{
    vec2 px = dFdx(p.xy);
    vec2 py = dFdy(p.xy);

    float fx = (3.0 * p.x * p.x) * px.x - px.y;
    float fy = (3.0 * p.x * p.x) * py.x - py.y;

    float sd1 = (p.x * p.x * p.x - p.y) / sqrt(fx * fx + fy * fy);

    vec2 qx = dFdx(p.zw);
    vec2 qy = dFdy(p.zw);

    float gx = (3.0 * p.z * p.z) * qx.x - qx.y;
    float gy = (3.0 * p.z * p.z) * qy.x - qy.y;

    float sd2 = (p.z * p.z * p.z - p.w) / sqrt(gx * gx + gy * gy);

    float sd = max(sd1, sd2); // Combine both sides of cubic
    return clamp(0.5 - sd, 0.0, 1.0);
}

void main() 
{
    float alpha = 0.0;
    switch(shapeType)
    {
        case 0:
            alpha = LineAlpha(fragUV.xy);
            break;
        case 1:
            alpha = QuadraticAlpha(fragUV.xy);
            break;
        case 2:
            alpha = CubicAlpa(fragUV);
            break;
        default:
            discard;
    } 

    if (alpha <= 0.0) discard;

    outColor = vec4(fragColor.rgb, fragColor.a * alpha);
}
