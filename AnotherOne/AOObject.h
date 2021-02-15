#pragma once
#include <vector>
#include <stack>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "OBJLoader.h"


struct MeshObj 
{
	GLuint vaoID;
	GLuint vertexBufID;
	GLuint uvBufID;
	GLuint normalBufID;

	std::vector<glm::vec3> vert;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::stack<int> offsetStack;
};

class AOObject
{
public:
	static objl::Loader objLoader;
	static glm::mat4 indMat;
	glm::vec3 wPos;
	glm::vec3 scale;
	glm::mat4 rot; // temp

	std::vector<AOObject*> childList;
	std::vector<MeshObj*> meshList;

	int shaderID;

	std::string name;

	AOObject(glm::vec3 pos, std::string name);

	void InitMesh(std::string objPath);

	virtual glm::mat4 getModelMat();

};
