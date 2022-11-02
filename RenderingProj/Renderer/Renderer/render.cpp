#include "render.h"

#include <fstream>

#include "glm/ext.hpp"
#include "stb/stb_image.h"

Geometry MakeGeometry(const Vertex* const verts, GLsizei vertCount, const GLuint* const indices, GLsizei indxCount)
{
	//create an instance of geo
	Geometry newGeo = {}; // zero-init
	newGeo.size = indxCount;

	//generate the buffers
	glGenVertexArrays(1, &newGeo.vao); // place one VAO at this memory location
	glGenBuffers(1, &newGeo.vbo);	   // VBO
	glGenBuffers(1, &newGeo.ibo);	   // IBO
	
	//bind the buffers (to specify which one we're working w/)
	glBindVertexArray(newGeo.vao);
	glBindBuffer(GL_ARRAY_BUFFER, newGeo.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newGeo.ibo);
	
	//buffer the buffers
	// - what buffer to buffer to?
	// - how much data are we buffering?
	// - where is the data we are buffering?
	// - how will the data be used?
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vertex), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indxCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

	//describe the buffers
	glEnableVertexAttribArray(0);          // attribute index to enable
	glVertexAttribPointer(0,	           // attrib index
						  4,	           // no. of components
						  GL_FLOAT,        // type of component
						  GL_FALSE,        // should be normalized?
						  sizeof(Vertex),  // stride (in bytes)
						  (void*)0);	   // offset from the beginning of the vertex type

	glEnableVertexAttribArray(1);          // attribute index to enable
	glVertexAttribPointer(1,	           // attrib index
						  4,	           // no. of components
						  GL_FLOAT,        // type of component
						  GL_FALSE,        // should be normalized?
						  sizeof(Vertex),  // stride (in bytes)
						  (void*)offsetof(Vertex, color));	   // offset from the beginning of the vertex type

	glEnableVertexAttribArray(2);          // attribute index to enable
	glVertexAttribPointer(2,	           // attrib index
						  2,	           // no. of components
						  GL_FLOAT,        // type of component
						  GL_FALSE,        // should be normalized?
						  sizeof(Vertex),  // stride (in bytes)
						  (void*)offsetof(Vertex, uv));	   // offset from the beginning of the vertex type

	glEnableVertexAttribArray(3);          // attribute index to enable
	glVertexAttribPointer(3,	           // attrib index
						  3,	           // no. of components
						  GL_FLOAT,        // type of component
						  GL_FALSE,        // should be normalized?
						  sizeof(Vertex),  // stride (in bytes)
						  (void*)offsetof(Vertex, normal));	   // offset from the beginning of the vertex type

	//unbind the buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//return the geo
	return newGeo;
}

Geometry MakeGeometry(const std::vector<Vertex> const verts, const std::vector<GLuint> const indices)
{
	return MakeGeometry(&verts[0], (GLsizei)verts.size(), indices.data(), (GLsizei)indices.size());
}

void FreeGeometry(Geometry& geo)
{
	glDeleteBuffers(1, &geo.vbo);
	glDeleteBuffers(1, &geo.ibo);
	glDeleteVertexArrays(1, &geo.vao);

	// zero out geo to prevent use after free
	geo = {};
}

void DrawGeometry(const Shader& shad, const Geometry& geo)
{
	// specify which shader to use
	glUseProgram(shad.program);

	// specify which geometry to use
	glBindVertexArray(geo.vao);

	// do the draw! DO IT DO IT RIGHT NOW
	glDrawElements(GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, nullptr);
}

//bool loadOBJ(const char* path, std::vector<glm::vec4>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
//{
//	//TODO: this  will probably not work with vec3's
//
//	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
//	std::vector< glm::vec3 > temp_vertices;
//	std::vector< glm::vec2 > temp_uvs;
//	std::vector< glm::vec3 > temp_normals;
//
//	FILE* file;
//	file = freopen(path, "r", file);
//	if (file == NULL) {
//		std::cout << "ERROR: File was unable to be opened/accessed!" << std::endl;
//		return false;
//	}
//
//	while (1) {
//		char lineHeader[128];
//		// read first word of line
//		int res = fscanf(file, "%s", lineHeader);
//		if (res == EOF)
//			break;
//
//		if (strcmp(lineHeader, "v") == 0) {
//			glm::vec3 vertex;
//			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
//			temp_vertices.push_back(vertex);
//		}
//		else if (strcmp(lineHeader, "vt") == 0) {
//			glm::vec2 uv;
//			fscanf(file, "%f %f\n", &uv.x, &uv.y);
//			temp_uvs.push_back(uv);
//		}
//		else if (strcmp(lineHeader, "vn") == 0) {
//			glm::vec3 normal;
//			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
//			temp_normals.push_back(normal);
//		}
//		else if (strcmp(lineHeader, "f") == 0) {
//			std::string vertex1, vertex2, vertex3;
//			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
//						&vertexIndex[0], &uvIndex[0], &normalIndex[0], 
//						&vertexIndex[1], &uvIndex[1], &normalIndex[1], 
//						&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
//
//			if (matches != 9) {
//				std::cout << "ERROR: file cannot be read by parser" << std::endl;
//				return false;
//			}
//
//			vertexIndices.push_back(vertexIndex[0]);
//			vertexIndices.push_back(vertexIndex[1]);
//			vertexIndices.push_back(vertexIndex[2]);
//			uvIndices.push_back(uvIndex[0]);
//			uvIndices.push_back(uvIndex[1]);
//			uvIndices.push_back(uvIndex[2]);
//			normalIndices.push_back(normalIndex[0]);
//			normalIndices.push_back(normalIndex[1]);
//			normalIndices.push_back(normalIndex[2]);
//		}
//	}
//
//	// Vertex
//	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
//		unsigned int vertexIndex = vertexIndices[i];
//		glm::vec4 vertex = glm::vec4(temp_vertices[vertexIndex - 1], 1);
//		out_vertices.push_back(vertex);
//	}
//	// UV
//	for (unsigned int i = 0; i < uvIndices.size(); i++) {
//		unsigned int uvIndex = uvIndices[i];
//		glm::vec2 uv = temp_uvs[uvIndex - 1];
//		out_uvs.push_back(uv);
//	}
//	// Normal
//	for (unsigned int i = 0; i < normalIndices.size(); i++) {
//		unsigned int normalIndex = normalIndices[i];
//		glm::vec3 normal = temp_normals[normalIndex - 1];
//		out_normals.push_back(normal);
//	}
//
//	return true;
//}

Shader MakeShader(const char* vertSource, const char* fragSource)
{
	// create an instance of Shader Program
	Shader shad = {};

	// create the shader
	shad.program = glCreateProgram();

	GLuint vertShad = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShad = glCreateShader(GL_FRAGMENT_SHADER);

	// load the shader source code
	glShaderSource(vertShad, 1, &vertSource, 0);
	glShaderSource(fragShad, 1, &fragSource, 0);

	// compile the shader
	glCompileShader(vertShad);
	GLint compileStatus = 0;
	glGetShaderiv(vertShad, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) {
		std::cout << "ERROR: couldnt compile vertShader!" << std::endl;
	}

	glCompileShader(fragShad);
	compileStatus = 0;
	glGetShaderiv(fragShad, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) {
		std::cout << "ERROR: couldnt compile fragShader!" << std::endl;
	}

	// attatch the shader to the shader program
	glAttachShader(shad.program, vertShad);
	glAttachShader(shad.program, fragShad);

	// link the shader program
	glLinkProgram(shad.program);
	compileStatus = 0;
	glGetProgramiv(shad.program, GL_LINK_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) {
		GLchar errorLog[1024];
		glGetProgramInfoLog(shad.program, 1024, nullptr, errorLog);

		std::cout << "ERROR: couldnt Link shader!" << std::endl;
		std::cout << errorLog << std::endl;
	}

	// delete shaders (not the program)
	glDeleteShader(vertShad);
	glDeleteShader(fragShad);

	// return the Shader object
	return shad;
}

Shader MakeShader(const std::string& const vertSource, const std::string& const fragSource)
{
	return MakeShader(vertSource.c_str(), fragSource.c_str());
}

Shader LoadShader(const char* vertPath, const char* fragPath)
{
	std::fstream vertStream(vertPath, std::ios_base::in);
	std::string vertSource;
	//TODO: add error checking for a valid file path

	std::string thisLine;
	while (!vertStream.eof()) {

		// copy line to temp
		std::getline(vertStream, thisLine);

		// append to overall source (do we need to add newLines? probably?!)
		vertSource += thisLine + '\n';
	}

	std::fstream fragStream(fragPath, std::ios_base::in);
	std::string fragSource;
	//TODO: add error checking for a valid file path

	while (!fragStream.eof()) {

		// copy line to temp
		std::getline(fragStream, thisLine);

		// append to overall source (do we need to add newLines? probably?!)
		fragSource += thisLine + '\n';
	}

	return MakeShader(vertSource, fragSource);
}

void FreeShader(Shader& shad)
{
	// Destroy on openGL's side
	glDeleteProgram(shad.program);

	// zero-init program to prevent accidental re-use
	shad = {};
}

Texture MakeTexture(unsigned _width, unsigned _height, unsigned _channels, const unsigned char* _pixels)
{
	//what is the image format
	GLenum oglFormat = GL_RGBA;
	switch (_channels) {
	case 1:
		oglFormat = GL_RED;
		break;
	case 2:
		oglFormat = GL_RG;
		break;
	case 3:
		oglFormat = GL_RGB;
		break;
	case 4:
		oglFormat = GL_RGBA;
		break;

	}

	//generates texture
	Texture textObj = { 0, _width, _height, _channels };
	glGenTextures(1, &textObj.handle);
	//find and buffer current texture
	glBindTexture(GL_TEXTURE_2D, textObj.handle);
	glTexImage2D(GL_TEXTURE_2D, 0, oglFormat, _width, _height, 0, oglFormat, GL_UNSIGNED_BYTE, _pixels);
	//configure it
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//unbind and return the texture object
	glBindTexture(GL_TEXTURE_2D, 0);

	return textObj;
}

void FreeTexture(Texture& tex)
{
	glDeleteTextures(1, &tex.handle);
	tex = {};

}

Texture LoadTexture(const char* imagePath)
{
	//setup some variables to store
	int imageWidth = -1;
	int imageHeight = -1;
	int imageFormat = -1;
	unsigned char* imagePixels = nullptr;

	Texture newTexture = {};

	//load the texture data
	stbi_set_flip_vertically_on_load(true);
	imagePixels = stbi_load(imagePath, &imageWidth, &imageHeight, &imageFormat, STBI_default);

	//pass the texture data to OGL
	newTexture = MakeTexture(imageWidth, imageHeight, imageFormat, imagePixels);
	//unload the texture data
	stbi_image_free(imagePixels);
	imagePixels = nullptr;

	//return result
	return newTexture;
}

void SetUniform(const Shader& shad, GLuint location, const glm::mat4& value)
{
	glProgramUniformMatrix4fv(shad.program, location, 1, GL_FALSE, glm::value_ptr(value));
}

void SetUniform(const Shader& shad, GLuint location, const glm::vec3& value)
{
	//std::cout << "lcoation: " << location << std::endl;
	//std::cout << "value " << value.x << ", " << value.y << ", " << value.z << std::endl;
	glProgramUniform3fv(shad.program, location, GL_TRUE, glm::value_ptr(value));
}

void SetUniform(const Shader& shad, GLuint location, const glm::vec4& value)
{
	glProgramUniform4fv(shad.program, location, GL_FALSE, glm::value_ptr(value));
}

void SetUniform(const Shader& shad, GLuint location, const Texture& value, int textureSlot)
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, value.handle);
	glProgramUniform1i(shad.program, location, textureSlot);
}

std::string ReadFile(const char* path)
{


	return std::string();
}

