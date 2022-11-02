#include "context.h"

#include "glew/glew.h"	 //ALWAYS INCLUDED BEFORE GLFW
#include "glfw/glfw3.h"

#include <iostream>
bool context::init(int width, int height, const char* title)
{
	// initialization
	//TODO: check for failure
	glfwInit();
	window = glfwCreateWindow(640, 480, "Hello Window", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	//TODO: check for failure
	glewInit();

	//print out diagnostics
	std::cout << "OpenGL Version: " << (const char*)glGetString(GL_VERSION) << '\n';
	std::cout << "Renderer: " << (const char*)glGetString(GL_RENDERER) << '\n';
	std::cout << "Vendor: " << (const char*)glGetString(GL_VENDOR) << '\n';
	std::cout << "GLSL: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// how to blend two objects
	glDepthFunc(GL_LEQUAL);								// how to determine what's "closer"
	glFrontFace(GL_CCW);								// winding order
	glCullFace(GL_BACK);								// which side to cull

	//set background camera
	glClearColor(0.25f, .25f, 0.25f, 1);

	return true;
}

void context::tick()
{
	glfwSwapBuffers(window);	// swap frame buffer
	glfwPollEvents();			// work through event queue


}

void context::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void context::term()
{
	// clean-up
	glfwDestroyWindow(window);
	window = nullptr;

	// de-initialization
	glfwTerminate();
}

bool context::shouldClose() const
{
	return glfwWindowShouldClose(window);
}
