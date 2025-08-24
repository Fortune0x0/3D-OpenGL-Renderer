#VERTEX SHADER
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 Color;

out vec4 vColor;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec4 cubeColor;  



void main(){
    gl_Position = projection * view * model * vec4(position, 1.0);
    vColor = Color;  
}

#FRAGMENT SHADER
#version 330 core
layout(location = 0) out vec4 color;

in vec4 vColor;

void main(){
    color = vColor;
}