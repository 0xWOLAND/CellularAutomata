#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 tPos;

out vec4 pos;
out vec2 texPos;

void main() {
	float x = -1.0 + float((gl_VertexID & 1) << 2);
	float y = -1.0 + float((gl_VertexID & 2) << 1);
	gl_Position = vec4(aPos.x, aPos.y, 0, 1);
	pos = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	texPos = tPos;
}