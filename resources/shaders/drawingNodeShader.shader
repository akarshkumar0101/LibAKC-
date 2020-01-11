#shader vertex
#version 410 core

layout(location = 0) in vec2 vPos;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(vPos, 0.0, 1.0);
}

#shader fragment
#version 410 core

uniform vec4 color;

out vec4 FragColor;
void main()
{
    FragColor = color;
}