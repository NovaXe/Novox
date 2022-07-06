#version 330 core
out vec4 FragColor;

// exists so that the light source always is bright like a lamp

uniform vec3 lightColor;

void main()
{

    FragColor = vec4 (lightColor, 1.0);

}