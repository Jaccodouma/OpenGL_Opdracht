#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glsl.h"
#include "objloader.h"
#include "texture.h"
#include "Object.h"
#include "Camera.h"

using namespace std;


//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

const int WIDTH = 800, HEIGHT = 600;

const char* fragshader_name = "fragmentshader.fsh";
const char* vertexshader_name = "vertexshader.vsh";

unsigned const int DELTA_TIME = 10;


//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------
GLuint shader_id;

Camera * camera;
Object * teapot, * torus, * cube;


//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboardHandler(unsigned char key, int a, int b)
{
    if (key == 27)
        glutExit();

    if (key == 'w')
        camera->Translate(glm::vec3(0.0f, 0.0f, -0.1f));
    if (key == 'a')
        camera->Translate(glm::vec3(-0.1f, 0.0f, 0.0f));
    if (key == 's')
        camera->Translate(glm::vec3(0.0f, 0.0f, 0.1f));
    if (key == 'd')
        camera->Translate(glm::vec3(0.1f, 0.0f, 0.0f));

    if (key == 'i')
        camera->Rotate(-1, glm::vec3(1.0f, 0.0f, 0.0f));
    if (key == 'j')
        camera->Rotate(-1, glm::vec3(0.0f, 1.0f, 0.0f));
    if (key == 'k')
        camera->Rotate(1, glm::vec3(1.0f, 0.0f, 0.0f));
    if (key == 'l')
        camera->Rotate(1, glm::vec3(0.0f, 1.0f, 0.0f));
}


//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    teapot->Rotate(1, glm::vec3(0.0f, 1.0f, 0.0f));

    teapot->Render();
    torus->Render();
    cube->Render();

    glutSwapBuffers();
}


//------------------------------------------------------------
// void Render(int n)
// Render method that is called by the timer function
//------------------------------------------------------------

void Render(int n)
{
    Render();
    glutTimerFunc(DELTA_TIME, Render, 0);
}


//------------------------------------------------------------
// void InitGlutGlew(int argc, char **argv)
// Initializes Glut and Glew
//------------------------------------------------------------

void InitGlutGlew(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Hello OpenGL");
    glutDisplayFunc(Render);
    glutKeyboardFunc(keyboardHandler);
    glutTimerFunc(DELTA_TIME, Render, 0);

    glewInit();
}


//------------------------------------------------------------
// void InitShaders()
// Initializes the fragmentshader and vertexshader
//------------------------------------------------------------

void InitShaders()
{
    char* vertexshader = glsl::readFile(vertexshader_name);
    GLuint vsh_id = glsl::makeVertexShader(vertexshader);

    char* fragshader = glsl::readFile(fragshader_name);
    GLuint fsh_id = glsl::makeFragmentShader(fragshader);

    shader_id = glsl::makeShaderProgram(vsh_id, fsh_id);
}

//------------------------------------------------------------
// void InitObjects()
//------------------------------------------------------------

void InitObjects()
{
    camera = new Camera(
        45.0f, 
        WIDTH, 
        HEIGHT, 
        glm::vec3(0.0, 1.7, 7.0),
        glm::vec3(0.0, 1.0, 0.0));

    teapot = new Object(
        "Resources/Objects/teapot.obj", 
        "Resources/Textures/uvtemplate.bmp",
        shader_id, camera);
    torus = new Object(
        "Resources/Objects/torus.obj",
        "Resources/Textures/uvtemplate.bmp",
        shader_id, camera);
    torus->Translate(3, 0, 0);
    cube = new Object(
        "Resources/Objects/box.obj",
        "Resources/Textures/test.bmp",
        shader_id, camera);
    cube->Translate(-3, 0, 0);
}


int main(int argc, char** argv)
{
    InitGlutGlew(argc, argv);
    InitShaders();
    InitObjects();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Hide console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_SHOW);

    // Main loop
    glutMainLoop();

    return 0;
}
