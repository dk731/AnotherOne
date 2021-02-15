#pragma once
#include "AOObject.h"

class AOCamera: public AOObject
{
public:
	glm::mat4 projMat;
	glm::mat4 viewMat;
	glm::vec2 rotation;


	AOCamera(float fovY, float aspectRat, float near, float far);

	glm::mat4 getModelMat() override;
};

