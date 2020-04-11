#pragma once
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "objloader.h"
#include "texture.h"
#include "Camera.h"

using namespace std;

class Object
{
public:

	const int WIDTH = 800, HEIGHT = 600; // TODO: THIS SUCKS
	Camera * camera;

	// ID's
	GLuint vao;
	GLuint shader_id;
	GLuint texture_id;

	GLuint uniform_mv;

	// Mesh variables
	vector<glm::vec3> normals;
	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;

	// Matrices
	glm::mat4 model, view, projection;
	glm::mat4 mv; 

	// Material variables
	glm::vec3 light_position, ambient_color, diffuse_color;
	glm::vec3 specular;
	float power;

	Object(const char* obj_path, const char* texture_path, GLuint shader_id, Camera * camera);

	void Render();

	// Transformations
	void Rotate(float d, glm::vec3 axis);
	void Scale(float s);
	void Scale(glm::vec3 s);
	void Translate(float x, float y, float z);
	void Translate(glm::vec3 t);

private:
	bool transformed;

	void InitMatrices();
	void InitBuffers();
	void InitMaterial();
};