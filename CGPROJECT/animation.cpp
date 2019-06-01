#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

#define TRUE 1
#define FALSE 0



#define ANGLE_STEP 10


#define MAGNITUDE 120
#define PHASE 270.112
#define FREQ_DIV 58
#define ARC_LENGHT 2.7
#define ARC_RADIUS 0.15


GLdouble view_h = 270, view_v = 0, head_angle = 0;
GLint crank_angle = 0;


GLdouble crank_step = 5;


GLdouble head_look_up_table[361];

GLUquadricObj *obj;



void myBox(GLdouble x, GLdouble y, GLdouble z)
{
	glPushMatrix();
	glScalef(x, y, z);
	glutSolidCube(1);
	glPopMatrix();
}

void myCylinder(GLUquadricObj * object, GLdouble outerRadius,GLdouble innerRadius, GLdouble lenght)
{
	glPushMatrix();
	gluCylinder(object, outerRadius, outerRadius, lenght, 20, 1);
	glPushMatrix();
	glRotatef(180, 0.0, 1.0, 0.0);
	gluDisk(object, innerRadius, outerRadius, 20, 1);
	glPopMatrix();
	glTranslatef(0.0, 0.0, lenght);
	gluDisk(object, innerRadius, outerRadius, 20, 1);
	glPopMatrix();
}

void draw_piston(void)
{
	glPushMatrix();
	glColor4f(0.3, 0.6, 0.9, 1.0);
	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -0.07);
	myCylinder(obj, 0.125, 0.06, 0.12);
	glPopMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 0.05);
	myCylinder(obj, 0.06, 0.0, 0.6);
	glTranslatef(0.0, 0.0, 0.6);
	myCylinder(obj, 0.2, 0.0, 0.5);
	glPopMatrix();
}

void draw_engine_pole(void)
{
	glPushMatrix();
	glColor4f(0.9, 0.9, 0.9, 1.0);
	myBox(0.5, 3.0, 0.5);
	glColor3f(0.5, 0.1, 0.5);
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.9, -0.4);
	myCylinder(obj, 0.1, 0.0, 2);
	glPopMatrix();
}
void draw_cylinder_head(void)
{
	glPushMatrix();
	glColor4f(0.5, 1.0, 0.5, 0.1);
	glRotatef(90, 1.0, 0.0, 0.0);
	glTranslatef(0, 0.0, 0.4);
	glRotatef(head_angle, 1, 0, 0);
	glTranslatef(0, 0.0, -0.4);
	myCylinder(obj, 0.23, 0.21, 1.6);
	glRotatef(180, 1.0, 0.0, 0.0);
	gluDisk(obj, 0, 0.23, 20, 1);
	glPopMatrix();
}
void draw_flywheel(void)
{
	glPushMatrix();
	glColor4f(0.5, 0.5, 1.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	myCylinder(obj, 0.625, 0.08, 0.5);
	glPopMatrix();
}
void draw_crankbell(void)
{
	glPushMatrix();
	glColor4f(1.0, 0.5, 0.5, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	myCylinder(obj, 0.3, 0.08, 0.12);
	glColor4f(0.5, 0.1, 0.5, 1.0);
	glTranslatef(0.0, 0.2, 0.0);
	myCylinder(obj, 0.06, 0.0, 0.34);
	glTranslatef(0.0, 0.0, 0.22);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(crank_angle - head_angle, 1.0, 0.0, 0.0);	
	draw_piston();
	glPopMatrix();
}
void draw_crank(void)
{
	glPushMatrix();
	glRotatef(crank_angle, 1.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -1.0);
	myCylinder(obj, 0.08, 0.0, 1.4);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.28, 0.0, 0.0);
	draw_crankbell();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.77, 0.0, 0.0);	
	draw_flywheel();
	glPopMatrix();
	glPopMatrix();
}
void display(void)
{
	int pass;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();	
	glDisable(GL_ALPHA_TEST);
	pass = 0;	
	glRotatef(view_h, 0, 1, 0);
	glRotatef(view_v, 1, 0, 0);
	do {
		if (pass == 2) {
			glAlphaFunc(GL_EQUAL, 1);
			glDepthMask(GL_TRUE);
			pass--;
		}
		else if (pass != 0) {
			glAlphaFunc(GL_NOTEQUAL, 1);
			glDepthMask(GL_FALSE);
			pass--;
		}
		draw_engine_pole();
		glPushMatrix();
		glTranslatef(0.5, 1.4, 0.0);
		draw_cylinder_head();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.0, -0.8, 0.0);
		draw_crank();
		glPopMatrix();
	} while (pass > 0);
	glDepthMask(GL_TRUE);
	glutSwapBuffers();
	glPopMatrix();
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {		
	case 'a':
		if ((crank_angle += crank_step) >= 360)
			crank_angle = 0;
		head_angle = head_look_up_table[crank_angle];
		break;
	case 'z':
		if ((crank_angle -= crank_step) <= 0)
			crank_angle = 360;
		head_angle = head_look_up_table[crank_angle];
		break;
	case '4':
		if ((view_h -= ANGLE_STEP) <= 0)
			view_h = 360;
		break;
	case '6':
		if ((view_h += ANGLE_STEP) >= 360)
			view_h = 0;
		break;
	case '8':
		if ((view_v += ANGLE_STEP) >= 360)
			view_v = 0;
		break;
	case '2':
		if ((view_v -= ANGLE_STEP) <= 0)
			view_v = 360;
		break;	
	case '+':
		if ((++crank_step) > 45)
			crank_step = 45;
		break;
	case '-':
		if ((--crank_step) <= 0)
			crank_step = 0;
		break;
	default:
		return;
	}
	glutPostRedisplay();
}
void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		if ((view_h -= ANGLE_STEP) <= 0)
			view_h = 360;
		break;
	case GLUT_KEY_RIGHT:
		if ((view_h += ANGLE_STEP) >= 360)
			view_h = 0;
		break;
	case GLUT_KEY_UP:
		if ((view_v += ANGLE_STEP) >= 360)
			view_v = 0;
		break;
	case GLUT_KEY_DOWN:
		if ((view_v -= ANGLE_STEP) <= 0)
			view_v = 360;
		break;
	default:
		return;
	}
	glutPostRedisplay();
}
void menu(int val)
{
	unsigned char key;
	switch (val) {
	case 7:
		key = '+';
		break;
	case 8:
		key = '-';
		break;
	default:
		return;
	}
	keyboard(key, 0, 0);
}
void create_menu(void)
{
	glutCreateMenu(menu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	
	glutAddMenuEntry("Speed UP", 7);
	glutAddMenuEntry("Slow Down", 8);
}
void make_table(void)
{
	GLint i;
	GLdouble k;
	for (i = 0, k = 0.0; i < 360; i++, k++) {
		head_look_up_table[i] =
			MAGNITUDE * atan(
			(ARC_RADIUS * sin(PHASE - k / FREQ_DIV)) /
				((ARC_LENGHT - ARC_RADIUS * cos(PHASE - k / FREQ_DIV))));
	}
}
void myinit(void)
{ 
	glClearColor(0.5, 0.4, 0.0, 0.0);
	obj = gluNewQuadric();
	make_table();	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	draw_piston();
	draw_flywheel();
	gluQuadricTexture(obj, GL_TRUE);
	draw_piston();
	draw_flywheel();
	gluQuadricTexture(obj, GL_FALSE);
}
void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
	glScalef(1.5, 1.5, 1.5);
}
int main(int argc, char **argv)
{
	puts("Steam Engine\n");
	puts("Keypad Arrow keys (with NUM_LOCK on) rotates object.");
	puts("Rotate crank: 'a' = anti-clock wise 'z' = clock wise");
	puts("Crank Speed : '+' = Speed up by 1 '-' = Slow Down by 1");	
	puts(" Alternatively a pop up menu with all toggles is attached");
	puts(" to the left mouse button.\n");
	glutInitWindowSize(400, 400);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutCreateWindow("Steam Engine");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	create_menu();
	myinit();
	glutReshapeFunc(myReshape);
	glutMainLoop();
	return 0; 
}
