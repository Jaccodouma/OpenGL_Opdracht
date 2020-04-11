#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp> 

class Camera
{
public:
	Camera(float fov, int WIDTH, int HEIGHT, glm::vec3 cameraPos, glm::vec3 lookAtPos) {
		this->fov = fov;
		this->WIDTH = WIDTH;
		this->HEIGHT = HEIGHT;
		this->cameraPos = cameraPos;
		this->lookAtPos = lookAtPos;

		this->Update();
	}

	void Translate(glm::vec3 t) {
		this->cameraPos += t; 
		this->lookAtPos += t; 
		this->Update();
	}

	void Rotate(float degrees, glm::vec3 r) {
		glm::vec3 newCameraPos = this->cameraPos;
		newCameraPos = glm::rotate(newCameraPos, glm::radians(degrees), r);
		this->cameraPos = newCameraPos; 

		this->Update();
	}

	glm::mat4 getView() { return this->view; };
	glm::mat4 getProjection() { return this->projection; };
private:
	glm::vec3 cameraPos;
	glm::vec3 lookAtPos;

	glm::mat4 view; 
	glm::mat4 projection;

	float fov;
	int WIDTH; 
	int HEIGHT;

	void Update() {
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

