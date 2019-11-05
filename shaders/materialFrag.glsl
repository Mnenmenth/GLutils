#version 330

out vec4 FragColor;

in vec2 UVCoord;

uniform sampler2D material;

void main()
{
    FragColor = texture(material, UVCoord);
}
