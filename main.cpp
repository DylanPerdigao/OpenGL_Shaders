#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__ 
    #define GL_SILENCE_DEPRECATION
    #include <GL/glew.h>
    #include <GL/glut.h>
#else
    #include <GL/glew.h>
    #include <GL/glut.h>
    #include <GL/gl.h>
#endif 
#include "colors.h"
#include "cube.h"
#include "reader.h"

#define STEP 0.1
//.................................................... Variaveis
GLint wScreen = 700;
GLint hScreen = 700;

//positions
GLfloat lightPos[3] = {0,1,1};
GLfloat  obsP[3] = {0,2,4};

//light
GLfloat isPhong;
GLfloat a = 0.05;
GLfloat s = 2;

char filenameV[] = "VertexShader.glsl";
char filenameF[] = "FragmentShader.glsl";

char* VertexShaderSource;
char* FragmentShaderSource;

GLuint  VertexShader, FragmentShader;
GLuint  ShaderProgram;

GLuint uLoc_position;
GLuint uLoc_observer;
GLuint uLoc_isPhong;
GLuint uLoc_a;
GLuint uLoc_s;


void drawScene(void){
    glColor4f(YELLOW);
    glPushMatrix();
    glTranslatef(lightPos[0],lightPos[1],lightPos[2]);
        drawCube(0.2);
    glPopMatrix();
    glColor4f(ORANGE);
    glPushMatrix();
        glutSolidTeapot(1);
    glPopMatrix();
}
void drawText(char* string, GLfloat x, GLfloat y){
    glPushMatrix();
    glTranslatef(x, 0, y);
    glRasterPos2f(0, 0);
    while (*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *string++);
    glPopMatrix();
}
void drawCommands(int x,int y,int ySpace){
    int line=0;
    char text[100];
    glColor4f(RED);
    snprintf(text, 100, "COMMANDS");
    drawText(text, x, y);line++;
    //MOVES
    glColor4f(GREEN);
    snprintf(text, 100, "- AD..............Move light left/right");
    drawText(text, x,y+line*ySpace);line++;
    snprintf(text, 100, "- WS..............Move light up/down");
    drawText(text, x,y+line*ySpace);line++;
    snprintf(text, 100, "- QE..............Move light front/back");
    drawText(text, x,y+line*ySpace);line++;
}
void draw(void){
    float wViewPort = 0.5;
    float hViewPort = 0.7;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //----------------------------------------------viewPort1
    glUniform1f(uLoc_isPhong, true);
    glUniform3fv(uLoc_position, 1, lightPos);
    glUniform3fv(uLoc_observer, 1, obsP);
    glUniform1f(uLoc_a, a);
    glUniform1f(uLoc_s, s);
    glViewport(0*wScreen, (1-hViewPort)*hScreen, wViewPort*wScreen, hViewPort*hScreen);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, (float)(wScreen*wViewPort)/(hScreen*hViewPort), 0.1, 9999);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);
    drawScene();

    //----------------------------------------------viewPort2
    glUniform1f(uLoc_isPhong, false);
    glUniform3fv(uLoc_position, 1, lightPos);
    glUniform3fv(uLoc_observer, 1, obsP);
    glUniform1f(uLoc_a, a);
    glUniform1f(uLoc_s, s);
    glViewport(0.5*wScreen, (1-hViewPort)*hScreen, wViewPort*wScreen, hViewPort*hScreen);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, (float)(wScreen*wViewPort)/(hScreen*hViewPort), 0.1, 9999);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);
    drawScene();

    //----------------------------------------------viewPort3
    glViewport(0*wScreen, 0*hScreen, 1*wScreen, (1-hViewPort)*hScreen);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 1, 0, 0, 0, 0, 0, 0,-1);
    drawCommands(-9, 0, 2);
    glColor4f(ORANGE);
    drawText("PHONG", -5.5, -9);
    drawText("GOURAUD", 5, -9);
    //----------------------------------------------
    glutSwapBuffers();
}



void event(unsigned char key, int x, int y) {
    switch (key) {
        //ESC
        case 27:
            exit(0);
            break;
        //SPACE
        case 32:
            isPhong? isPhong=0:isPhong=1;
            std::cout << isPhong;
            std::cout << "\n";
            break;
        //A
        case 'A':
        case 'a':
            lightPos[0]-=STEP;
            break;
        //D
        case 'D':
        case 'd':
            lightPos[0]+=STEP;
            break;
        //W
        case 'W':
        case 'w':
            lightPos[1]+=STEP;
            break;
        //S
        case 'S':
        case 's':
            lightPos[1]-=STEP;
            break;
        //Q
        case 'Q':
        case 'q':
            lightPos[2]-=STEP;
            break;
        //E
        case 'E':
        case 'e':
            lightPos[2]+=STEP; 
            break;
    }
    glutPostRedisplay(); 
}

int main(int argc, char** argv){
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(wScreen, hScreen);
    glutInitWindowPosition(300, 100);
    glutCreateWindow("Dylan Perdigão | Projeto | Shaders");
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Init
    glClearColor(0, 0, 0, 1);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE); 
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GLEW   
    glewInit();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Criar, compilar, linkar, e usar
    GLEW_ARB_vertex_shader; 
    GLEW_ARB_fragment_shader;
    //Criar
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    VertexShaderSource = readShaderFile(filenameV);
    FragmentShaderSource = readShaderFile(filenameF);
    const char* VS = VertexShaderSource; 
    const char* FS = FragmentShaderSource; 
    glShaderSource(VertexShader, 1, &VS, NULL);
    glShaderSource(FragmentShader, 1, &FS, NULL);
    free(VertexShaderSource);
    free(FragmentShaderSource);
    //Compilar
    glCompileShaderARB(VertexShader);
    glCompileShaderARB(FragmentShader);
    //Criar e Linkar
    ShaderProgram = glCreateProgramObjectARB(); 
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);  
    glLinkProgram(ShaderProgram); 
    //Usar
    glUseProgramObjectARB(ShaderProgram);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~UNIFORM
    uLoc_position = glGetUniformLocation(ShaderProgram, "u_lightPos"); 
    glUniform1fv(uLoc_position,1,lightPos);
    uLoc_observer = glGetUniformLocation(ShaderProgram, "u_obsPos");
    glUniform1fv(uLoc_position,1,obsP);
    uLoc_isPhong = glGetUniformLocation(ShaderProgram, "u_isPhong");
    glUniform1f(uLoc_isPhong,isPhong);
    uLoc_a = glGetUniformLocation(ShaderProgram, "u_a");
    glUniform1f(uLoc_a,a);
    uLoc_s = glGetUniformLocation(ShaderProgram, "u_s");
    glUniform1f(uLoc_s,s);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Libertar os Shaders
    glDetachShader(ShaderProgram, VertexShader);
    glDetachShader(ShaderProgram, FragmentShader);
    glDeleteShader(ShaderProgram);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    glutDisplayFunc(draw);
    glutKeyboardFunc(event);
    glutIdleFunc(draw);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    glutMainLoop();
    return 1;
}
    
   
