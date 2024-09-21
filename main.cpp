#include <GL/glut.h>  // OpenGL graphics and input
#include "chip.h"    // Your CPU core implementation
#include <stdio.h>

chip8 myChip8;

int modifier = 10;

void drawPixel(int x, int y);
void updateQuads(const chip8& c8);
void display();
void reshape(int width,int height);
void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);   

void display() // updateQuads is inside of this function
{
    myChip8.emulateCycle();
	if(myChip8.drawFlag)
	{
		// Clear framebuffer
		glClear(GL_COLOR_BUFFER_BIT);

        updateQuads(myChip8);

        // Swap buffers!
		glutSwapBuffers();


		// Processed frame
		myChip8.drawFlag = false;
	}

};

void reshape(GLsizei width, GLsizei height) //reshapes the size of the window
{
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

    // Adjust the projection matrix to maintain aspect ratio
    glMatrixMode(GL_PROJECTION); //we move to the projection matrix mode
    glLoadIdentity(); //resets the parameters of the projection matrix
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW); //important for moving objeccts and rendering.
    glViewport(0, 0, width, height); // it tells OpenGL how to map the rendering output to the new window size.
};

void drawPixel(int x, int y)
{
	glBegin(GL_QUADS);
		glVertex3f((x * modifier) + 0.0f,     (y * modifier) + 0.0f,	 0.0f);
		glVertex3f((x * modifier) + 0.0f,     (y * modifier) + modifier, 0.0f);
		glVertex3f((x * modifier) + modifier, (y * modifier) + modifier, 0.0f);
		glVertex3f((x * modifier) + modifier, (y * modifier) + 0.0f,	 0.0f);
	glEnd();
};

void updateQuads(const chip8& c8) //drawPixel is inside of this function
{
	// Draw
	for(int y = 0; y < 32; ++y)
		for(int x = 0; x < 64; ++x)
		{
			if(c8.gfx[(y*64) + x] == 0)
				glColor3f(0.0f,0.0f,0.0f);
			else
				glColor3f(1.0f,1.0f,1.0f);

			drawPixel(x, y);
		}
};

void keyboardDown(unsigned char key, int x, int y) //is responsible for detecting when a key is pressed on your keyboard and then updating the myChip8.key[] array accordingly
{
	if(key == 27)    // esc
		exit(0);

	if(key == '1')		myChip8.key[0x1] = 1;
	else if(key == '2')	myChip8.key[0x2] = 1;
	else if(key == '3')	myChip8.key[0x3] = 1;
	else if(key == '4')	myChip8.key[0xC] = 1;

	else if(key == 'q')	myChip8.key[0x4] = 1;
	else if(key == 'w')	myChip8.key[0x5] = 1;
	else if(key == 'e')	myChip8.key[0x6] = 1;
	else if(key == 'r')	myChip8.key[0xD] = 1;

	else if(key == 'a')	myChip8.key[0x7] = 1;
	else if(key == 's')	myChip8.key[0x8] = 1;
	else if(key == 'd')	myChip8.key[0x9] = 1;
	else if(key == 'f')	myChip8.key[0xE] = 1;

	else if(key == 'z')	myChip8.key[0xA] = 1;
	else if(key == 'x')	myChip8.key[0x0] = 1;
	else if(key == 'c')	myChip8.key[0xB] = 1;
	else if(key == 'v')	myChip8.key[0xF] = 1;

	//printf("Press key %c\n", key);
};

void keyboardUp(unsigned char key, int x, int y) //responsible for key releasing
{
	if(key == '1')		myChip8.key[0x1] = 0;
	else if(key == '2')	myChip8.key[0x2] = 0;
	else if(key == '3')	myChip8.key[0x3] = 0;
	else if(key == '4')	myChip8.key[0xC] = 0;

	else if(key == 'q')	myChip8.key[0x4] = 0;
	else if(key == 'w')	myChip8.key[0x5] = 0;
	else if(key == 'e')	myChip8.key[0x6] = 0;
	else if(key == 'r')	myChip8.key[0xD] = 0;

	else if(key == 'a')	myChip8.key[0x7] = 0;
	else if(key == 's')	myChip8.key[0x8] = 0;
	else if(key == 'd')	myChip8.key[0x9] = 0;
	else if(key == 'f')	myChip8.key[0xE] = 0;

	else if(key == 'z')	myChip8.key[0xA] = 0;
	else if(key == 'x')	myChip8.key[0x0] = 0;
	else if(key == 'c')	myChip8.key[0xB] = 0;
	else if(key == 'v')	myChip8.key[0xF] = 0;
};

int main(int argc, char **argv)
{

    //Load Game
    myChip8.loadGame("C:\\Users\\Monster\\Desktop\\CPLUSPLUS ALISTIRMAAAAA\\glut_projectdenemesi\\pong2.c8"); //initialization made in this game

    //myChip8.loadGame("C:\\Users\\Monster\\Desktop\\CPLUSPLUS ALISTIRMAAAAA\\glut_projectdenemesi\\tetris.c8"); //initialization made in this game

    // Initialize GLUT and process any command-line arguments
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //GLUT_DOUBLE : ensures smooth frame transitions by using double buffering

    glutInitWindowPosition(320,320); //arranges the position of first display.
    glutInitWindowSize(640,320);

    glutCreateWindow("myChip8 BY ERAY ERDEM ZEYBEK");

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);

	updateQuads(myChip8);

    glutMainLoop();

    return 0;
}


