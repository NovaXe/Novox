#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 customColor; //set in render loop
uniform int scalingFactor;
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    //FragColor = vec4(ourColor, 1.0);
    FragColor = mix(texture(texture1, TexCoord * scalingFactor), texture(texture2, TexCoord*scalingFactor), 0.35);
    //FragColor = vec4(customColor, 1.0);
    //FragColor = texture(texture1, TexCoord) * texture(texture2, TexCoord);

}