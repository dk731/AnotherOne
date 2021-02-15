#include "AOObject.h"

glm::mat4 AOObject::indMat(1.0f);
objl::Loader AOObject::objLoader;

AOObject::AOObject(glm::vec3 pos, std::string name):
	wPos(pos), scale(glm::vec3(1.0f)), name(name), shaderID(0)
{
	rot = glm::rotate(0.0f, glm::vec3(1.0f));
}

void AOObject::InitMesh(std::string objPath)
{
	if (objPath.size() < 5)
		objPath = "def_cube.obj";

	if (!objLoader.LoadFile(objPath))
		throw std::logic_error("Not able to load object");

	for (auto mesh = objLoader.LoadedMeshes.begin(); mesh != objLoader.LoadedMeshes.end(); mesh++)
	{
		MeshObj* tmpMeshObj = new MeshObj();
		for (auto vert = mesh->Vertices.begin(); vert != mesh->Vertices.end(); vert++)
		{
			tmpMeshObj->vert.push_back(glm::vec3(vert->Position.X, vert->Position.Y, vert->Position.Z));
			tmpMeshObj->uvs.push_back(glm::vec2(vert->TextureCoordinate.X, vert->TextureCoordinate.Y));
			tmpMeshObj->normals.push_back(glm::vec3(vert->Normal.X, vert->Normal.Y, vert->Normal.Z));
		}


		glGenVertexArrays(1, &tmpMeshObj->vaoID);
		glBindVertexArray(tmpMeshObj->vaoID);


		glGenBuffers(1, &(tmpMeshObj->vertexBufID));
		glBindBuffer(GL_ARRAY_BUFFER, tmpMeshObj->vertexBufID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tmpMeshObj->vert.size(), &tmpMeshObj->vert[0], GL_STATIC_DRAW);


		glGenBuffers(1, &(tmpMeshObj->uvBufID));
		glBindBuffer(GL_ARRAY_BUFFER, tmpMeshObj->uvBufID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * tmpMeshObj->uvs.size(), &tmpMeshObj->uvs[0], GL_STATIC_DRAW);


		glGenBuffers(1, &(tmpMeshObj->normalBufID));
		glBindBuffer(GL_ARRAY_BUFFER, tmpMeshObj->normalBufID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tmpMeshObj->normals.size(), &tmpMeshObj->normals[0], GL_STATIC_DRAW);


		meshList.push_back(tmpMeshObj);
	}
}


glm::mat4 AOObject::getModelMat()
{
	//return glm::mat4(1.0f);
	return glm::translate(indMat, wPos) * rot * glm::scale(scale);
}
