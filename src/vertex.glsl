#version 330 core

layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec3 Color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec4 VertexColor;

void main() {

    gl_Position = Projection * View * Model * vec4(VertexPos, 1.0f);
    VertexColor = vec4(Color, 1.0f);
}
