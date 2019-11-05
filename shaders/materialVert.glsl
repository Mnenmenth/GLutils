#version 330
layout(location = 0) in vec3 VertPos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 UV;

out vec2 UVCoord;

uniform mat4 VPMat;
uniform mat4 MMat;

void main() {

    UVCoord = UV;

    gl_Position = VPMat * MMat * vec4(VertPos, 1.0f);
}
