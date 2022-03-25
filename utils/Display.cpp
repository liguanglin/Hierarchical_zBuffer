#include "Display.h"

zBuffer* Display::zbuffer;
float Display::T[4][4];

Display::Display(zBuffer* buffer)
{
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            T[i][j] = (i==j);
        }
    }
    zbuffer = buffer;
}

void Display::keyboard(int key, int x, int y)  //call for keyboard
{
    float theta = 0.0;
    const float PI = acos(-1);
    float delta = PI/12;
    
    float n1 = 1.0, n2 = 0.0, n3 = 0.0;

    // std::cout<<key<<" "<<x<<" "<<y<<std::endl;

    switch (key)
    {
    case GLUT_KEY_LEFT: //rotate the model
        theta = delta;
        n1 = 0.0;
        n2 = 1.0;
        n3 = 0.0;
        break;
    case GLUT_KEY_RIGHT:
        theta = -delta;
        n1 = 0.0;
        n2 = 1.0;
        n3 = 0.0;
        break;
    case GLUT_KEY_UP:
        theta = delta;
        n1 = 1.0;
        n2 = 0.0;
        n3 = 0.0;
        break;
    case GLUT_KEY_DOWN:
        theta = -delta;
        n1 = 1.0;
        n2 = 0.0;
        n3 = 0.0;
        break;
    case GLUT_KEY_PAGE_DOWN:  // move the view point
        zbuffer->viewPoint[2][3] = std::max(zbuffer->viewPoint[2][3] - 0.5, 0.5);
        break;
    case GLUT_KEY_PAGE_UP:
        zbuffer->viewPoint[2][3] = std::min(zbuffer->viewPoint[2][3] + 0.5, 3.0);
        break;
    default:
        break;
    }
    float cosine = cos(theta),sine = sin(theta);

	T[0][0] = n1*n1 + (1 - n1*n1)*cosine;
	T[0][1] = n1*n2*(1 - cosine) + n3*sine;
	T[0][2] = n1*n3*(1 - cosine) - n2*sine;

	T[1][0] = n1*n2*(1 - cosine) - n3*sine;
	T[1][1] = n2*n2*(1 - cosine) + cosine;
	T[1][2] = n2*n3*(1 - cosine) + n1*sine;

	T[2][0] = n1*n2*(1 - cosine) + n2*sine;
	T[2][1] = n2*n3*(1 - cosine) - n1*sine;
	T[2][2] = n3*n3 + (1 - n3*n3)*cosine;

    // printT(T);

	zbuffer->updateT(T);
    zbuffer->rerender();
    glutPostRedisplay();
}



void Display::loop()   //render
{
    int width = 800, height = 600;
    glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
    
    glBegin(GL_POINTS);

    // float mx=0;

    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            float depth = zbuffer->bufferID[i][j]->z;
            //std::cout<<i<<" "<<j<<" "<<depth<<"\n";
            // assert(1-depth<=1);
            // mx=std::max(mx,1-depth);
            float d = (1.0-depth);
            glColor3f(d, d, d);
            glVertex2i(i,j);
        }
    }
    // std::cout<<mx<<std::endl;
    glEnd();
    glFinish();
}

void Display::run()
{
    int width = 800, height = 600;
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("zBuffer");
    glutDisplayFunc(loop);
    // glutMouseFunc(dragMouse);
    glutSpecialFunc(keyboard);
    glutMainLoop();
}