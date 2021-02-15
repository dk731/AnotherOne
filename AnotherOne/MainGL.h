#pragma once

#include <iostream>
#include <stdexcept>
#include <thread> 

#include <stdlib.h>  
#include <time.h>


#include <fstream>

#include <string>
#include <vector>
#include <stack>

#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "AOObject.h"
#include "AOScene.h"
#include "AOCamera.h"

#include "AOCollisionManager.h"


struct ShaderProgram
{
	GLuint id;
	GLuint mvpID;

	ShaderProgram(GLuint id, GLuint mvpID) :
		id(id), mvpID(mvpID)
	{
	}
};

class MainGL
{
private:
	static void renderLoop();

	static void physicLoop();
	static void eventsLoop();

	static std::thread *physicThread;
	static std::thread* eventsThread;
	

	static bool finish;

public:
	static AOCamera *mainCamera;

	static std::vector<AOScene*> sceneList;
	static std::vector<ShaderProgram*> shaderList;

	static int wWidth, wHeight;
	static GLFWwindow* windowHandle;
	static int fixedStep, renderStep;

	static void initMainGL(int wWidth, int wHeight, std::string wName);
	static void loadScene(std::string folderPath);
	static void unloadScene(int id);
	static void selectScene(int id);

	static GLuint compileShader(const char** shaderString, int shaderType);
	static GLuint linkShaders(GLuint vertS, GLuint fragS);
	static int loadShader(std::string pathVert, std::string pathFrag);

	static void startGame();
};

