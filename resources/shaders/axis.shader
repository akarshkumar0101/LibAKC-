#shader vertex
//vec3->vec4
#version 410

uniform mat4 projectionView;

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec4 axisColor;

out vec4 vColor;

void main()
{
    vColor = axisColor;
    gl_Position = projectionView * vec4(vPos, 1.0);
}

#shader fragment
//?->vec4(color)
#version 410

in vec4 vColor;
out vec4 FragColor;

void main()
{
    FragColor = vColor;
}