#version 330 core


in vec4 fragcolor;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;
uniform sampler2D texture_one;

void main()
{
vec4 texcolor = texture(texture_one, TexCoord);

FragColor = fragcolor * texcolor;
FragColor - fragcolor;
}
