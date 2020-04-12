#pragma once
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp> 

using namespace std;

class Camera
{
public:
	Camera(float fov, glm::vec3 cameraPos, glm::vec3 lookAtPos) {
		this->fov = fov;
		this->cameraPos = cameraPos;
		this->cameraDir = lookAtPos - cameraPos;
		this->cameraDir = normalize(this->cameraDir);

		pitch = 0; 
		yaw = 0;

		this->Update();
	}

	void Translate(glm::vec3 t) {
		glm::vec3 translation = glm::vec3(0,0,0); 

		// Move forward/back
		translation += this->cameraDir * -t.z;

		// Restrain sideways movement to height 
		glm::vec3 t_side = glm::rotate(this->cameraDir, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		t_side.y = 0; 
		t_side = normalize(t_side);


		// Move side to side
		translation +=  t_side * -t.x;

		this->cameraPos += translation;

		this->Update();
	}

	void Rotate(float x, float y) {
		float sensitivity = 0.05;
		x *= sensitivity;
		y *= sensitivity;
		
		yaw += x;
		pitch += y; 

		// Constrain pitch
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		// Constrain yaw
		yaw = fmod(yaw, 360.0f);

		// Calculate new cameraDir
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraDir= glm::normalize(front);

		this->Update();
	}

	void Update() {
		glm::vec3 lookAtPos = this->cameraPos + this->cameraDir;

		this->view = glm::lookAt(
			cameraPos,
			lookAtPos,
			glm::vec3(0.0, 1.0, 0.0)
		);

		this->projection = glm::perspective(
			glm::radians(fov), /* FOV */
			1.0f * glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), /* ASPECT RATIO */
			0.1f, /* NEAR CLIPPING PANE */
			20.0f /* FAR CLIPPING PANE */
		);
	}

	glm::mat4 getView() { return this->view; };
	glm::mat4 getProjection() { return this->projection; };
private:
	float yaw, pitch;

	glm::vec3 cameraPos;
	glm::vec3 cameraDir;

	glm::mat4 view; 
	glm::mat4 projection;

	float fov;
	int WIDTH; 
	int HEIGHT;
};

