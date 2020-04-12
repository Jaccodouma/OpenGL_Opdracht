#include <iostream>
#include <list>

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

list<Object> objects;

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

        // Set new window width and height
        if (glutGet(GLUT_SCREEN_HEIGHT) == glutGet(GLUT_WINDOW_HEIGHT))
        {
            // Get out of fullscreen
            glutReshapeWindow(1280, 720);
        }
        else {
            // Go fullscreen
            glutReshapeWindow(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
        }

        glutFullScreenToggle();
    }
}

void motionHandler(int x, int y)
{
    static int center_x = glutGet(GLUT_WINDOW_WIDTH) / 2;
    static int center_y = glutGet(GLUT_WINDOW_HEIGHT) / 2;

    float dx = (float) x- center_x;
    float dy = center_y - (float) y;

    cout << dx << " - " << dy << endl;

    if (dx != 0.0f && dy != 0.0f) {
        camera->Rotate(dx, dy);
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

    for (Object obj : objects)
    {
        obj.Render();
    }

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
    glutInitWindowSize(1280/2, 720/2);
    glutCreateWindow("AAAAAAAAAAAAAAAAH");
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
        75.0f, 
        glm::vec3(0.0, 1.7, 1.5),
        glm::vec3(0.0, 1.0, 0.0));

    // ------------------------------------------------------
    // WALLS, FLOOR & CEILING
    // ------------------------------------------------------
    objects.push_back(Object(
        "Resources/Objects/room_walls.obj",
        "Resources/Textures/wall.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/room_ceiling.obj",
        "Resources/Textures/wall.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/chimney.obj",
        "Resources/Textures/wall.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/room_floor.obj",
        "Resources/Textures/carpet.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/windows.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/door.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));

    // ------------------------------------------------------
    // BED
    // ------------------------------------------------------
    objects.push_back(Object(
        "Resources/Objects/bedframe.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera,
        glm::vec3(-0.859137, 0, -0.894849)));
    objects.push_back(Object(
        "Resources/Objects/bedframe_drawer.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera,
        glm::vec3(-0.859137, 0, -0.894849)));
    objects.push_back(Object(
        "Resources/Objects/mattress.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera,
        glm::vec3(-0.859137, 0, -0.894849)));

    // ------------------------------------------------------
    // Fridge
    // ------------------------------------------------------
    objects.push_back(Object(
        "Resources/Objects/fridge.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/fridge_door.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));

    // ------------------------------------------------------
    // Kitchen cabinets
    // ------------------------------------------------------
    objects.push_back(Object(
        "Resources/Objects/KitchenCabinets.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/KitchenCabinets_Door_L.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/KitchenCabinets_Door_R.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));

    // ------------------------------------------------------
    // Kitchen sink
    // ------------------------------------------------------
    objects.push_back(Object(
        "Resources/Objects/KitchenSink_Top.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/KitchenSink_Bottom.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/KitchenSink_Door_L.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/KitchenSink_Door_R.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));

    // ------------------------------------------------------
    // TV Cabinet
    // ------------------------------------------------------
    objects.push_back(Object(
        "Resources/Objects/Cabinet.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/Cabinet_Drawer.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));


    // ------------------------------------------------------
    // MISC. SINGLE ITEMS
    // ------------------------------------------------------
    objects.push_back(Object(
        "Resources/Objects/desk.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera,
        glm::vec3(-0.624001, 0, 1.5851)));
    objects.push_back(Object(
        "Resources/Objects/couch.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera,
        glm::vec3(1.37328, 0, -1.18505)));
    objects.push_back(Object(
        "Resources/Objects/radiator.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera,
        glm::vec3(1.81458, 0, 0.765988)));
    objects.push_back(Object(
        "Resources/Objects/shelves_1.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/shelves_2.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/TV.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/table.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/bin.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));
    objects.push_back(Object(
        "Resources/Objects/clock.obj",
        "Resources/Textures/paintedwood.bmp",
        shader_id, camera));
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
