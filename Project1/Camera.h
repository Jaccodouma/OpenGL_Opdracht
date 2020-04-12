#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp> 

using namespace std;

class Camera
{
public:
	Camera(float fov, int WIDTH, int HEIGHT, glm::vec3 cameraPos, glm::vec3 lookAtPos) {
		this->fov = fov;
		this->WIDTH = WIDTH;
		this->HEIGHT = HEIGHT;
		this->cameraPos = cameraPos;
		this->cameraDir = lookAtPos - cameraPos;
		this->cameraDir = normalize(this->cameraDir);

		this->Update();
	}

	void Translate(glm::vec3 t) {
		glm::vec3 translation = glm::vec3(0,0,0); 
		translation += this->cameraDir * -t.z;

		// Move this up to not move Y coord when moving forward and back 
		glm::vec3 t_side = glm::rotate(this->cameraDir, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		t_side.y = 0; 
		t_side = normalize(t_side);
		translation +=  t_side * -t.x;

		this->cameraPos += translation;

		this->Update();
	}

	void Rotate(float degrees, glm::vec3 r) {
		this->cameraDir = glm::rotate(cameraDir, glm::radians(degrees), -r);
		this->Update();
	}

	glm::mat4 getView() { return this->view; };
	glm::mat4 getProjection() { return this->projection; };
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraDir;

	glm::mat4 view; 
	glm::mat4 projection;

	float fov;
	int WIDTH; 
	int HEIGHT;

	void Update() {
		glm::vec3 lookAtPos = this->cameraPos + this->cameraDir;

		view = glm::lookAt(
			cameraPos,
			lookAtPos,
			glm::vec3(0.0, 1.0, 0.0)
		);

		projection = glm::perspective(
			glm::radians(fov), /* FOV */
			1.0f * WIDTH / HEIGHT, /* ASPECT RATIO */
			0.1f, /* NEAR CLIPPING PANE */
			20.0f /* FAR CLIPPING PANE */
		);
	}
};

