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
}

void specialKeyHandler(int key, int x, int y)
{
    if (key == GLUT_KEY_F11)
    {
        // Screen size
        int screen_h = glutGet(GLUT_SCREEN_HEIGHT);
        int screen_w = glutGet(GLUT_SCREEN_WIDTH);

        // Toggle fullscreen
        glutFullScreenToggle();

        // Set new window width and height
        if (glutGet(GLUT_SCREEN_HEIGHT) == glutGet(GLUT_WINDOW_HEIGHT))
        {
            // Get out of fullscreen
            glutReshapeWindow(800, 600);
        }
        else {
            // Go fullscreen
            glutReshapeWindow(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
        }
    }
}

void mouseHandler(int button, int state, int x, int y)
{
    /*
        Buttons:
            0: LMB
            1: MMB
            2: RMB
        State:
            0: Clicked
            1: Unclicked
    */
    cout << "Button: " << button << " - State: " << state << " - X: " << x << " - Y: " << y << endl;
}

void motionHandler(int x, int y)
{
    static int center_x = glutGet(GLUT_WINDOW_WIDTH) / 2;
    static int center_y = glutGet(GLUT_WINDOW_HEIGHT) / 2;

    if (center_x != x || center_y != y) {
        camera->Rotate(1, glm::vec3(y-center_y, x-center_x, 0.0f));
    }

    glutWarpPointer(center_x, center_y);
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
    glutInitWindowSize(800, 600);
    glutCreateWindow("Hello OpenGL");
    glutDisplayFunc(Render);

    glutKeyboardFunc(keyboardHandler);
    glutSpecialFunc(specialKeyHandler);
    glutPassiveMotionFunc(motionHandler);

    glutTimerFunc(DELTA_TIME, Render, 0);

    glutSetCursor(GLUT_CURSOR_NONE);

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
