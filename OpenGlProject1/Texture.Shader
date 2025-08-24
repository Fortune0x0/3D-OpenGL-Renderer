#VERTEX SHADER
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoordValues;

out vec2 texCoords;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;



void main(){
	gl_Position = projection * view * model * vec4(position, 1.0);
	texCoords = texCoordValues;
}


#FRAGMENT SHADER
#version 330 core

layout(location = 0) out vec4 tex;

in vec2 texCoords;
uniform sampler2D texUnit;

void main(){
	vec4 sampTex = texture(texUnit, texCoords);
	tex = sampTex;
}