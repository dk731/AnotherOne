#include "MainGL.h"

std::thread *MainGL::physicThread;
std::thread* MainGL::eventsThread;

std::vector<AOScene*> MainGL::sceneList;
std::vector<ShaderProgram*> MainGL::shaderList;

AOCamera *MainGL::mainCamera;

int MainGL::wWidth, MainGL::wHeight;
GLFWwindow *MainGL::windowHandle;

bool MainGL::finish = false;
int MainGL::fixedStep, MainGL::renderStep;

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

void MainGL::renderLoop()
{
    using namespace std::chrono;

    long long lastCall = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();

    while (1)
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::stack<AOObject*, std::vector<AOObject*>> tmpStack(sceneList[0]->objList);

        while (tmpStack.size() > 0)
        {
            AOObject* curObj = tmpStack.top();
            glm::mat4 curMVP = mainCamera->getModelMat() * curObj->getModelMat();

            for (auto t = curObj->meshList.begin(); t != curObj->meshList.end(); t++)
            {
                glBindVertexArray((*t)->vaoID);

                glUseProgram(shaderList[curObj->shaderID]->id);

                glUniformMatrix4fv(shaderList[curObj->shaderID]->mvpID, 1, GL_FALSE, &curMVP[0][0]);

                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, (*t)->vertexBufID);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, (*t)->uvBufID);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

                glEnableVertexAttribArray(2);
                glBindBuffer(GL_ARRAY_BUFFER, (*t)->normalBufID);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

                glDrawArrays(GL_TRIANGLES, 0, (*t)->vert.size());

            }
            for (auto t = curObj->childList.begin(); t != curObj->childList.end(); t++)
                tmpStack.push(*t);

            tmpStack.pop();
        }

        glfwSwapBuffers(windowHandle);
        glfwPollEvents();

        int delta = 0;
        do 
        {
            delta = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() - lastCall;
        } while (delta < MainGL::renderStep);

        lastCall = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
        std::cout << "\033[H" << "FPS: " << 1000000 / (float) delta << std::endl;
    }
}

void MainGL::physicLoop()
{
    using namespace std::chrono;
    time_point<system_clock> prevCall  = system_clock::now();
    while (1)
    {
        AOCollisionManager::resolveCollisions(&MainGL::sceneList[0]->objList);

        while (duration_cast<milliseconds>(system_clock::now() - prevCall).count() < MainGL::fixedStep);
        prevCall = system_clock::now();
    }
}

void MainGL::eventsLoop()
{
}

void MainGL::initMainGL(int width, int height, std::string wName)
{
    srand(time(NULL));

    wWidth = width;
    wHeight = height;
    fixedStep = 16;
    renderStep = 0;

    if (!glfwInit())
        throw std::logic_error("Was not able to initialize window");

    //glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    windowHandle = glfwCreateWindow(wWidth, wHeight, wName.c_str(), nullptr, nullptr);

    if (!windowHandle)
        throw std::logic_error("Was not able to create window");

    glfwMakeContextCurrent(windowHandle);
    glfwSwapInterval(0);

    glfwSetInputMode(windowHandle, GLFW_STICKY_KEYS, GL_TRUE);

    if (glewInit() != GLEW_OK)
        throw std::logic_error("Was not able to initialize glew");

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    MainGL::mainCamera = new AOCamera(glm::radians(45.0f), (float)wWidth / (float)wHeight, 1.0f, 100.0f);
    MainGL::mainCamera->wPos = glm::vec3(3.0f, 0.0f, 0.0f);
    loadShader("def_verts.glsl", "def_frags.glsl");
}


void MainGL::loadScene(std::string folderPath)
{
    // 1. Go to folder
    // 2. Parse scene.aos

    AOScene *newScene = new AOScene();
    
    
    for (int i = 0; i < 2000; i++)
    {
        AOObject* tmpObj = new AOObject(glm::vec3((rand() % 60) / (float)10, (rand() % 60) / (float)10, (rand() % 60) / (float)10), "test_cube");
        tmpObj->InitMesh("def_cube.obj");
        tmpObj->scale = glm::vec3((rand() % 100) / (float)1000);
        newScene->objList.push_back(tmpObj);

        sceneList.push_back(newScene);
    }
}

void MainGL::unloadScene(int id)
{
}

void MainGL::selectScene(int id)
{
}

GLuint MainGL::compileShader(const char** shaderString, int shaderType)
{
    GLuint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, shaderString, NULL);
    glCompileShader(shaderID);

    GLint   cStatus;
    GLsizei errLength;
    GLsizei aLength;
    char* errMsg;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &cStatus);

    if (cStatus != GL_TRUE)
    {
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &errLength);
        errMsg = (char*)malloc(errLength);
        glGetShaderInfoLog(shaderID, errLength, &aLength, errMsg);
        if (errLength > 0)
            std::cout << errMsg << std::endl;
        throw std::logic_error("Shader error");
    }

    return shaderID;
}

GLuint MainGL::linkShaders(GLuint vertS, GLuint fragS)
{

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertS);
    glAttachShader(shaderProgram, fragS);
    glLinkProgram(shaderProgram);

    GLint cStatus;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &cStatus);

    if (cStatus != GL_TRUE)
        throw std::logic_error("Shader linking error");

    return shaderProgram;
}

int MainGL::loadShader(std::string pathVert, std::string pathFrag)
{
    std::ifstream in(pathVert);
    std::string vsStr = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    const char* vertex_shader = vsStr.c_str();
    in.close();

    in = std::ifstream(pathFrag);
    std::string fsStr = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    const char* fragment_shader = fsStr.c_str();
    in.close();

    GLuint vsid = compileShader(&vertex_shader, GL_VERTEX_SHADER);
    GLuint fsid = compileShader(&fragment_shader, GL_FRAGMENT_SHADER);

    GLuint programID = linkShaders(vsid, fsid);
    glUseProgram(programID);
    GLuint mvpID = glGetUniformLocation(programID, "MVP");

    shaderList.push_back(new ShaderProgram(programID, mvpID));

    return shaderList.size() - 1;
}

void MainGL::startGame()
{
    //renderThread = new std::thread(MainGL::renderLoop);
    renderLoop();
}
