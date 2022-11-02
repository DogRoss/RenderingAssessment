// vertex shader
#version 430 core

// vertex attribs - data from the model
layout (location = 0) in vec4 position;	// index 0 - position
layout (location = 1) in vec4 color;	// index 1 - color
layout (location = 2) in vec2 uv;		// index 2 - uv
layout(location = 3) in vec3 normal;    // index 3 - nromal

// uniforms (shader program globals)
layout(location = 0) uniform mat4 proj;		// to CLIP SPACE
layout(location = 1) uniform mat4 view;		// to VIEW SPACE
layout(location = 2) uniform mat4 model;	// to WORLD SPACE

//
out vec4 vPos;
out vec4 vCol;
out vec2 vUV;
out vec3 vNormal;


//each shader needs a main() func
void main()
{
	vPos = model * position;
	vCol = color;
	vUV = uv;
	vNormal = mat3((model)) * normal;

	// "MVP" Matrix
	gl_Position = proj * view * model * position;
}