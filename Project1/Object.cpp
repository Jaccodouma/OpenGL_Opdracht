#include "Object.h"


Object::Object(const char* obj_path, const char* texture_path, GLuint shader_id) {
	bool res = loadOBJ(obj_path, vertices, uvs, normals);
	this->texture_id = loadBMP(texture_path);
	this->shader_id = shader_id;

	InitMaterial();
	InitMatrices();
	InitBuffers();
}

void Object::Update() 
{
	this->Rotate(.01f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->Translate(.01, .01, 0);
}

void Object::Render() {
	if (transformed) {
		// Update mv
		mv = view * model;

		// Send mv
		glUseProgram(shader_id);
		glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, glm::value_ptr(mv));
	}

	// Render
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}

void Object::InitMatrices() {
	model = glm::mat4();
	view = glm::lookAt(
		glm::vec3(2.0, 2.0, 7.0),  // eye
		glm::vec3(0.0, 0.0, 0.0),  // center
		glm::vec3(0.0, 1.0, 0.0));  // up
	projection = glm::perspective(
		glm::radians(45.0f),
		1.0f * WIDTH / HEIGHT, 0.1f,
		20.0f);
	mv = view * model;
}

void Object::InitBuffers() {
	// ----------------------------------------------
	// VBOs
	// ----------------------------------------------
	GLuint vbo_vertices, vbo_normals, vbo_uvs;

	// VBO for verts
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(glm::vec3),
		&vertices[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VBO for normals 
	glGenBuffers(1, &vbo_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glBufferData(GL_ARRAY_BUFFER,
		normals.size() * sizeof(glm::vec3),
		&normals[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VBO for uvs
	glGenBuffers(1, &vbo_uvs);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
	glBufferData(GL_ARRAY_BUFFER,
		uvs.size() * sizeof(glm::vec2),
		&uvs[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ----------------------------------------------
	// Bind to VAO 
	// ----------------------------------------------
	GLuint position_id, normal_id, uv_id;

	// Get vertex attributes
	position_id = glGetAttribLocation(shader_id, "position");
	normal_id = glGetAttribLocation(shader_id, "normal");
	uv_id = glGetAttribLocation(shader_id, "uv");

	// Allocate memory for vao
	glGenVertexArrays(1, &vao);

	// Bind to vao
	glBindVertexArray(vao);

	// Bind vertices to vao
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(position_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Bind normals to vao
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normal_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// bind uvs to vao
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
	glVertexAttribPointer(uv_id, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(uv_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Stop bind to vao
	glBindVertexArray(0);

	// Define model
	mv = view * model;

	// Make uniform vars
	uniform_mv = glGetUniformLocation(shader_id, "mv");
	GLuint uniform_proj = glGetUniformLocation(shader_id, "projection");
	GLuint uniform_light_pos = glGetUniformLocation(shader_id, "light_pos");
	GLuint uniform_material_ambient = glGetUniformLocation(shader_id, "mat_ambient");
	GLuint uniform_material_diffuse = glGetUniformLocation(shader_id, "mat_diffuse");

	GLuint uniform_specular = glGetUniformLocation(shader_id, "mat_specular");
	GLuint uniform_material_power = glGetUniformLocation(shader_id, "mat_power");

	// Fill uniform vars
	glUseProgram(shader_id);

	glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(uniform_proj, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(uniform_light_pos, 1, glm::value_ptr(light_position));
	glUniform3fv(uniform_material_ambient, 1, glm::value_ptr(ambient_color));
	glUniform3fv(uniform_material_diffuse, 1, glm::value_ptr(diffuse_color));
	glUniform3fv(uniform_specular, 1, glm::value_ptr(specular));
	glUniform1f(uniform_material_power, power);
}

void Object::InitMaterial() {
	light_position = glm::vec3(20, 20, 20);
	ambient_color = glm::vec3(0.05, 0.05, 0.05);
	diffuse_color = glm::vec3(1.0, 1.0, 1.0);

	specular = glm::vec3(0.0, 1.0, 1.0);
	power = 500;
}

// ---------------------------------------------------------------------
// TRANSFORMATIONS 
// ---------------------------------------------------------------------
void Object::Rotate(float d, glm::vec3 axis) {
	model = glm::rotate(model, d, axis);
	transformed = true; 
}

void Object::Scale(glm::vec3 s) {
	model = glm::scale(model, s);
	transformed = true;
}

void Object::Translate(glm::vec3 t) {
	model = glm::translate(model, t);
	transformed = true;
}

void Object::Scale(float s) {
	this->Scale(glm::vec3(s, s, s));
}

void Object::Translate(float x, float y, float z) {
	this->Translate(glm::vec3(x, y, z));
}