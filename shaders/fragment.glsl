#version 330 core

in vec3 fragColor;
in vec2 textureCoords;
		
out vec4 outColor;
		
uniform sampler2D ourTexture;
uniform float textureOpacity;
uniform float colorOpacity;
		
void main()
{
	vec4 color = colorOpacity * vec4(fragColor, 1.0);
	vec4 tex = colorOpacity * texture(ourTexture, textureCoords);
	outColor = mix(color, tex, textureOpacity);
};