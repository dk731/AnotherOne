#include "AOCamera.h"

AOCamera::AOCamera(float fovY, float aspectRat, float near, float far) :
	AOObject(glm::vec3(0.0f, 0.0f, 0.0f), "MainCamera"), rotation(0.0f)
{
	projMat = glm::perspective(fovY, aspectRat, near, far);
	float tmp = glm::cos(rotation.y);
	viewMat = glm::lookAt(wPos, wPos + glm::vec3(tmp * glm::cos(rotation.x), glm::sin(rotation.y), tmp * glm::sin(rotation.y)), glm::vec3(0, 1, 0));
}

glm::mat4 AOCamera::getModelMat()
{
	/*float tmp = glm::cos(rotation.y);
	viewMat = glm::lookAt(wPos, wPos + glm::vec3(tmp * glm::cos(rotation.x), glm::sin(rotation.y), tmp * glm::sin(rotation.y)), glm::vec3(0, 1, 0));*/
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt( glm::vec3(10, 6, 6), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0) );
	return Projection * View;
}

