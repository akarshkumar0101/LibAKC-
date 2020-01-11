#shader vertex

#version 330 core
layout(location = 0) in vec3 vPos;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(vPos, 1.0);
}

#shader fragment

#version 330 core

uniform vec4 solidColor;
out vec4 FragColor;

void main()
{
    FragColor = solidColor;
}