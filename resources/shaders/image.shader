#shader vertex

#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 texPos_;

out vec2 texPos;

void main()
{
    gl_Position = vec4(vPos, 1.0);
    texPos = texPos_;
}

#shader fragment

#version 330 core

in vec2 texPos;
out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, texPos);
}