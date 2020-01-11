#shader vertex
//vec3->vec4
#version 410

uniform mat4 model;
uniform mat4 projectionView;

layout(location = 0) in vec3 vPos;

void main()
{
    gl_Position = projectionView * model * vec4(vPos, 1.0);
}

#shader fragment
//?->vec4(color)
#version 410

uniform vec4 solidColor;

out vec4 FragColor;

void main()
{
    //gl_FragColor = solidColor;
    FragColor = solidColor;
}