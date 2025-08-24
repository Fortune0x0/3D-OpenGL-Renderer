#VERTEX SHADER
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 vertexColor;

uniform float angle;
uniform float offset;
uniform float radius;

out vec4 vColor;

void main(){
	vec2 rotation = vec2(cos(angle), sin(angle)) * radius + offset;
	gl_Position = vec4(position + rotation, 0.0, 1.0);
	vColor = vertexColor;
	
}

#FRAGMENT SHADER
#version 330 core

layout(location = 0) out vec4 color;

in vec4 vColor;

void main(){
	color = vColor;
}