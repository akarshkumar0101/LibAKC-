#shader vertex
//vec3->vec4
#version 120

uniform mat4 model;
uniform mat4 projectionView;

attribute vec3 vPos;

void main()
{
    gl_Position = projectionView * model * vec4(vPos, 1.0);
}

#shader fragment
//?->vec4(color)
#version 120

uniform vec4 solidColor;

void main()
{
    gl_FragColor = solidColor;
}