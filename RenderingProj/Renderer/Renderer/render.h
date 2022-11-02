#pragma once

//render.h

#include <vector>
#include <string>
#include <iostream>

#include "glm/glm.hpp"
#include "glew/glew.h"

struct Vertex {
	glm::vec4 pos;
	glm::vec4 color;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct Geometry {
	// vao - Vertex Array Object
	// vbo - Vertex Buffer Object (Contains vertex data)
	// ibo - Index Buffer Object (Contains indices data)

	GLuint vao, vbo, ibo; // openGL buffer names
	GLuint size;		  // openGL index count
};

struct Shader {
	GLuint program; // openGL shader program name
};

struct Texture {
	GLuint handle;
	unsigned width, height, channels;
};

struct Light {
	glm::vec3 color;
	glm::vec3 direction;
};


Geometry MakeGeometry(const Vertex* const verts, GLsizei vertCount,
					  const GLuint* const indices, GLsizei indxCount);
Geometry MakeGeometry(const std::vector<Vertex> const verts,
					  const std::vector<GLuint> const indices);
void FreeGeometry(Geometry& geo);
void DrawGeometry(const Shader& shad, const Geometry& geo);


Shader MakeShader(const char* vertSource, const char* fragSource);
Shader MakeShader(const std::string & const vertSource, const std::string & const fragSource);
Shader LoadShader(const char* vertPath, const char* fragPath);
void FreeShader(Shader& shad);

Texture MakeTexture(unsigned _width, unsigned _height, unsigned _channels, const unsigned char* _pixels);
void FreeTexture(Texture& tex);
Texture LoadTexture(const char* imagePath);

void SetUniform(const Shader& shad, GLuint location, const glm::mat4& value);
void SetUniform(const Shader& shad, GLuint location, const glm::vec3& value);
void SetUniform(const Shader& shad, GLuint location, const glm::vec4& value);
void SetUniform(const Shader& shad, GLuint location, const Texture& value, int textureSlot);

std::string ReadFile(const char* path);