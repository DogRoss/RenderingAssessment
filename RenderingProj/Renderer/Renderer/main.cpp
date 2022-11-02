#include "context.h"	// window-ing
#include "render.h"		// GPU-ing

#include "glm/ext.hpp"  // for identity
#include "glfw/glfw3.h"

#include "objectLoader.h"



typedef glm::vec3 Vector3;
typedef glm::mat4 Matrix4;

int main() {
	// runs when application starts
	context context;
	context.init(640, 480, "Hellow Window");

	const char* fileName = "res/models/soulspear.obj";
	Geometry spearGeo = LoadObject(fileName);
	
	Matrix4 spearModel = glm::identity<Matrix4>();

	//// load the geo
	Vertex triangleVerts[] =
	{
		//FRONT FACE v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v
		// vertex 0 - bottom left back
		{
			//pos
			{ -.5f, -.5f, 0, 1 },
			//col
			{ 1, 1, 1, 1},
			//uv
			{0, 0},
			//mormals
			{ 0, 0, 1}
		},
		// vertex 1 - bottom right back
		{
			//pos
			{ .5f, -.5f, 0, 1 },
			//col
			{ 1, 1, 1, 1},
			//uv
			{1, 0},
			//mormals
			{ 0, 0, 1 }
		},
		// vertex 2 - top left back
		{
			//pos
			{ -.5f, .5f, 0, 1 },
			//col
			{ 1, 1, 1, 1 },
			//uv
			{0, 1},
			//mormals
			{ 0, 0, 1 }
		},
		// vertex 3 - top right back
		{
			//pos
			{ .5f, .5f, 0, 1 },
			//col
			{ 1, 1, 1, 1 },
			//uv
			{1, 1},
			//mormals
			{ 0, 0, 1}
		},
		
	};
	GLuint triangleIndices[] = { 0, 1, 2, 1, 3, 2 };
	Geometry triangleGeo = MakeGeometry(triangleVerts, 4, triangleIndices, 6);

	// load the shader
	Shader basicShad = LoadShader("res/shaders/basic.vert", "res/shaders/basic.frag");

	// load the textures
	Texture sampleTex = LoadTexture("res/textures/scobama.png");

	// setup our matrices
	// model - transform of our object
	Matrix4 triangleModel = glm::identity<Matrix4>();
	// view - camera
	Vector3 cameraPos = Vector3{ 0,0,5 };
	Matrix4 camera = glm::lookAt(cameraPos,	// pos of cam
		Vector3{ 0,0,0 },	// pos to look at
		Vector3{ 0,1,0 });	// up vector
	// project - to "flatten it down into a 2d screen" || put into clip space
	Matrix4 proj = glm::perspective(glm::radians(60.0f), 640 / (float)480, 0.01f, 20.0f);

	float rand = (float)(std::rand()) / (float)(RAND_MAX);
	int count = 0;
	Vector3 axis = { 0,1.f,0 };

	float deltaTime = 0.1f;
	float lastTime = 0.0f;

	Light light = { { 1.f,1.f,1.f },
					{ 180, 0, 30 } };

	glm::vec3 AmbientLightColor = { 1, 1, 1 };
					
	float tick = 0.0f;
	float rate = 0.01f;
	
	SetUniform(basicShad, 0, proj);
	SetUniform(basicShad, 1, camera);
	SetUniform(basicShad, 3, sampleTex, 0);
	SetUniform(basicShad, 4, light.color);
	SetUniform(basicShad, 5, light.direction);
	SetUniform(basicShad, 7, cameraPos);

	while (!context.shouldClose()) {
		if (lastTime != 0) {
			deltaTime = (glfwGetTime() - lastTime);
		}
		lastTime = glfwGetTime();

		context.tick();
		context.clear();

		tick++;

		//light.direction = glm::normalize(glm::vec3(glm::cos(tick * rate), 0,
		//	glm::sin(tick * rate)));

		rand = 1;
		//spearModel = glm::rotate(triangleModel, rand * deltaTime, axis);
		/*count++;
		if (count > 2) {
			count = 0;
		}
		rand = (float)(std::rand()) / (float)(RAND_MAX);
		switch (count) {
		case 0:
			axis.x += rand * deltaTime;
			if (axis.x > 1) {
				axis.x = 0.1f;
			}
			break;
		case 1:
			axis.y += rand * deltaTime;
			if (axis.y > 1) {
				axis.y = 0.1f;
			}
			break;

		case 2:
			axis.z += rand * deltaTime;
			if (axis.z > 1) {
				axis.z = 0.1f;
			}
			break;
		}*/

		// draw the geo - 3d triangle

		//std::cout << "light direciton: " << std::to_string(light.direction.x) << "," << std::to_string(light.direction.y) << "," << std::to_string(light.direction.z) << "," << std::endl;
		SetUniform(basicShad, 2, spearModel);
		DrawGeometry(basicShad, spearGeo);
	}

	context.term();

	return 0;
}

