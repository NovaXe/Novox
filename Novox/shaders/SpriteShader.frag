#version 330 core

out vec4 FragColor;
in vec2 UV;

uniform sampler2D spriteSheet;
uniform vec3 spriteColor;

void main()
{
 
	vec4 texColor = texture(spriteSheet, UV) * vec4(spriteColor, 1.0);

	if (texColor.a < 0.1){
		discard;
	}
	FragColor = texColor;

}