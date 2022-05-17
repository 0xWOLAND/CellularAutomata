#include "./headers/Camera.h"
#include "./headers/Objects.h"
#include "./headers/Shader.h"
#include "./headers/buffer.h"
#include "./headers/Mesh.h"
#include "./headers/Arguments.h"
#include "./levenshtein.h"
#include "./headers/TimerUtil.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <GL/glew.h>
#include <GL/glext.h>
#include "./headers/glm/gtx/string_cast.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define GLM_ENABLE_EXPERIMENTAL
using namespace std;
float fstep = 0.9f;
float near = 50.0f;
float far = 300.0f;
float fovy = 45.0f;
float aspect = SCREEN_HEIGHT / SCREEN_WIDTH;
bool rotation = false, animation = false;
bool flag = 0;
bool LIGHTING_ENABLED = true;
string VERTEX_SHADER_PATH = "./shaders/shader.vs";
string FRAG_SHADER_PATH = "./shaders/shader.fs";
int ANIM_STEP = 1;
int RULE;
int tt = 0;
double avg_time = 0.0;
double fc = 0.0;
int SIDE_LENGTH = 0;

TimerUtil *tmr;
Shader *sh;
Camera *c;
Object *cubes = new Object();
bufferIndex *wire_vb;
Shader *wire_sh;
Mesh *wf;
Mesh *light_wf;

std::vector<std::string> possible_rules = {
"445",
"CLOUDS",
"AMOEBA",
"PULSE_WAVE",
"von_Neumann_Builder",
"Architecture",
"Custom1",
"Custom2",
"Custom3",
"Custom4",
"DA_BRAIN",
"vonN",
"test",
"test",
"678"
};

void display()
{
    tmr->startTimer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.82745098039, 0.82745098039, 0.82745098039, 1.0);

    cubes->draw();
    if (animation && tt % ANIM_STEP == 0)
    {
        tt = 0;
        cubes->update();
    }
    if (rotation && flag)
        c->rotate(rotation);
    wf->draw();

    glutSwapBuffers();
    double ms_double = tmr->stopTimer();
    avg_time += ms_double;
    fc += 1;
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'i':
        c->rotateCamera(0.01f, glm::vec3(1.0f, 0.0f, 0.0f));
        break;
    case 'k':
        c->rotateCamera(0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    case 'j':
        c->rotateCamera(0.01f, glm::vec3(0.0f, 0.0f, 1.0f));
        break;
    case 'z':
        c->zoomCamera(1 / fstep);
        break;
    case 'x':
        c->zoomCamera(fstep);
        break;
    case 'u':
        cubes->update();
        break;
    case 'p':
        animation = !animation;
        break;
    case 't':
        rotation = !rotation;
        break;
    case 'a':

        c->translate(glm::vec3(1.0f, 0.0f, 0.0f));

        break;
    case 'd':

        c->translate(glm::vec3(-1.0f, 0.0f, 0.0f));
        break;
    case 'w':

        c->translate(glm::vec3(0.0f, 0.0f, -1.0f));
        break;
    case 's':

        c->translate(glm::vec3(0.0f, 0.0f, 1.0f));
        break;
    case 'q':

        c->translate(glm::vec3(0.0f, -1.0f, 0.0f));
        break;
    case 'e':

        c->translate(glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    case 27:
        glutLeaveMainLoop();
        cout << "[avg frame rate]: " << 1000 * fc / (avg_time) << " f/s"
             << endl;
        return;
    };
    glutPostRedisplay();
}

void Timer(int value)
{
    glutTimerFunc(10, Timer, 0);
    tt += 1;
    glutPostRedisplay();
}

void readConfig(string filename)
{
    ifstream istr(filename);
    istr >> SIDE_LENGTH;
    istr >> LIGHTING_ENABLED;
    istr >> RULE;
    istr >> animation;
    istr >> rotation;
    VERTEX_SHADER_PATH = LIGHTING_ENABLED ? "./shaders/shader.vs" : "./shaders/shader_nl.vs";
    FRAG_SHADER_PATH = LIGHTING_ENABLED ? "./shaders/shader.fs" : "./shaders/shader_nl.fs";
}

void procesArguments(int argc, char **argv)
{

    string arg1, arg2, arg3;
    Levenshtein L;
   switch (argc)
    {

    case 2:
        arg1 = string(argv[1]);
        RULE = L.levenshtein(possible_rules, arg1);
        break;
    case 3:
        arg1 = string(argv[1]);
        arg2 = string(argv[2]);
        animation = arg1 == "true";
        rotation = arg2 == "true";
        break;
    case 4:
        arg1 = string(argv[1]);
        arg2 = string(argv[2]);
        animation = arg1 == "true";
        rotation = arg2 == "true";
        RULE = L.levenshtein(possible_rules, argv[3]);
        break;
    case 5:
        arg1 = string(argv[1]);
        arg2 = string(argv[2]);
        animation = arg1 == "true";
        rotation = arg2 == "true";
        RULE = L.levenshtein(possible_rules, argv[3]);
        SIDE_LENGTH = atoi(argv[4]);
        break;
    case 6:
        arg1 = string(argv[1]);
        arg2 = string(argv[2]);
        animation = arg1 == "true";
        rotation = arg2 == "true";
        RULE = L.levenshtein(possible_rules, argv[3]);
        SIDE_LENGTH = atoi(argv[4]);
        ANIM_STEP = atoi(argv[5]);
        break;
    };
    printf("RULE %d: \n", RULE);
}

void init()
{
    int argc = 1;
    rotation = false;
    flag = true;
    glutInit(&argc, 0);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(1.00, 1.00);
    glutCreateWindow("Cellular Automata");
    glutKeyboardFunc(keyboard);
    glClearColor(0.82745098039, 0.82745098039, 0.82745098039, 1.0);
    glutTimerFunc(0, Timer, 0);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, 1);
    glEnable(GL_ALPHA_TEST);
    glutDisplayFunc(display);
    glewInit();
    readConfig("./configs/config");
};

int main(int argc, char **argv)
{
    if (cmdOptionExists(argv, argv + argc, "-h"))
    {
        printHelpMenu();
    }
    init();
    tmr = new TimerUtil();
    procesArguments(argc, argv);
    c = new Camera(glm::vec3((SIDE_LENGTH / 2.0f), SIDE_LENGTH + 50.0f, SIDE_LENGTH + 50.0f),
                   glm::vec3((SIDE_LENGTH / 2.0f), 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), fovy,
                   aspect, near, far);
    wf = new Mesh(2.5f, (float)SIDE_LENGTH, c, "./shaders/shader_nl.vs",
                       "./shaders/shader_nl.fs");

    cubes->init(VERTEX_SHADER_PATH, FRAG_SHADER_PATH, LIGHTING_ENABLED, SIDE_LENGTH, c, RULE);
    glutMainLoop();
    return 0;
}
