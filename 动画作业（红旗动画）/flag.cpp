#include <windows.h>  
#include<GL\glew.h>
#include <gl\glut.h>
#include <gl\gl.h> 
#include <cstdlib>  
#include<vector>
#include<math.h>
using namespace std;

//控制点  
GLint nNumPoints = 4;
GLfloat currentPoints[3][4][3] = { {{  -4.0f, 0.0f, 4.0f},
								{ -2.0f, 1.0f, 4.0f},
								{ 2.0f, 0.0f, 4.0f},
								{  4.0f, -1.0f, 4.0f }},

{{  -4.0f, 0.0f, 0.0f},
{ -2.0f, 1.0f, 0.0f},
{ 2.0f, 0.0f, 0.0f},
{  4.0f, -1.0f, 0.0f }},

{{  -4.0f, 0.0f, -4.0f},
{ -2.0f, 1.0f, -4.0f},
{ 2.0f, 0.0f, -4.0f},
{  4.0f, -1.0f, -4.0f }}
};

//画控制点  
void DrawPoints(void) {
	int i, j;

	glColor3f(0, 0, 1);
	glPointSize(5);
	glBegin(GL_POINTS);
	for (i = 0; i < nNumPoints; i++)
		for (j = 0; j < 4; j++)
			glVertex3fv(currentPoints[i][j]);
	glEnd();
}

//画杆子
void drawPole(void) {
	glColor3f(0.9, 0.4, 0.4);
	glBegin(GL_LINE_STRIP);
	glVertex3f(-4.0f, 0, -4.0f);
	glVertex3f(-4.0f, 0, -2.0f);
	glVertex3f(-4.0f, 0, 2.0f);
	glVertex3f(-4.0f, 0, 4.0f);
	glVertex3f(-4.0f, 0, 14.0f);
	glEnd();
}

void RenderScene(void)
{
	// Clear the window with current clearing color 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//DrawPoints();
	drawPole();

	glColor3f(1, 0, 0);
	//设置映射方式，只需要设置一次可以在SetupRC中调用。  
	//glMap2f 替换了之前的 glMap1f, 这个函数指定了 u 和 v 两个域上的点
	glMap2f(GL_MAP2_VERTEX_3, //生成的数据类型  
		0.0f, // u的下界 
		10.0f, //u的上界  
		3, //数据中点的间隔  
		4, //u方向上的阶  
		0.0f, //v的下界  
		10.0f, //v的上界  
		12, // 控制点之间的间隔 ，三维数组，每一组u都有3个点，3*3=9
		3, // v方向上的阶  
		&currentPoints[0][0][0]); //控制点数组 

	//启用求值器  
	glEnable(GL_MAP2_VERTEX_3);

	GLfloat ambient[3] = { 0.7,0.1,0 };
	GLfloat diffuse[3] = { 0.2,0.2,0.0 };//a liitle yellow
	GLfloat specular[3] = { 1,1,1 };
	//开启灯光
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	GLfloat position[] = { -10.0f,0.0f,0.f,1.f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);

	//映射网格，在 u 的值域的范围内创建一个包含各个点的空间对称的网格
	glMapGrid2f(20, 0.0f, 10.0f, 20, 0.0f, 10.0f);
	// 计算网格，使用指定的图元来连接各个点
	//glEvalMesh2(GL_LINE, 0, 20, 0, 20);
	glEvalMesh2(GL_FILL, 0, 20, 0, 20);

	/*同时使用 glMapGrid 和 glEvalMesh 来有效地生成和评估一系列均匀分布的地图域值。
	glEvalMesh 函数单步执行一维或二维网格的整数域，
	其范围是 glMap1和 glMap2指定的计算映射的域。
	mode 参数确定生成的顶点是作为点、线条还是填充多边形进行连接。*/

	glutSwapBuffers();
}

int turn = 0;
void idle() {

	if (turn % 8 == 0) {
		currentPoints[0][1][1] = 1;
		currentPoints[0][2][1] = 0;
		currentPoints[0][3][1] = -1;

		currentPoints[1][1][1] = 1;
		currentPoints[1][2][1] = 0;
		currentPoints[1][3][1] = -1;

		currentPoints[2][1][1] = 1;
		currentPoints[2][2][1] = 0;
		currentPoints[2][3][1] = -1;

	}

	else if (turn % 8 == 1) {
		currentPoints[0][1][1] = 2;
		currentPoints[0][2][1] = 1;
		currentPoints[0][3][1] = 0;

		currentPoints[1][1][1] = 2;
		currentPoints[1][2][1] = 1;
		currentPoints[1][3][1] = 0;

		currentPoints[2][1][1] = 2;
		currentPoints[2][2][1] = 1;
		currentPoints[2][3][1] = 0;
	}

	else if (turn % 8 == 2) {
		currentPoints[0][1][1] = 1;
		currentPoints[0][2][1] = 2;
		currentPoints[0][3][1] = 1;

		currentPoints[1][1][1] = 1;
		currentPoints[1][2][1] = 2;
		currentPoints[1][3][1] = 1;

		currentPoints[2][1][1] = 1;
		currentPoints[2][2][1] = 2;
		currentPoints[2][3][1] = 1;
	}

	else if (turn % 8 == 3) {
		currentPoints[0][1][1] = 0;
		currentPoints[0][2][1] = 1;
		currentPoints[0][3][1] = 2;

		currentPoints[1][1][1] = 0;
		currentPoints[1][2][1] = 1;
		currentPoints[1][3][1] = 2;

		currentPoints[2][1][1] = 0;
		currentPoints[2][2][1] = 1;
		currentPoints[2][3][1] = 2;
	}

	else if (turn % 8 == 4) {
		currentPoints[0][1][1] = -1;
		currentPoints[0][2][1] = 0;
		currentPoints[0][3][1] = 1;

		currentPoints[1][1][1] = -1;
		currentPoints[1][2][1] = 0;
		currentPoints[1][3][1] = 1;

		currentPoints[2][1][1] = -1;
		currentPoints[2][2][1] = 0;
		currentPoints[2][3][1] = 1;
	}

	else if (turn % 8 == 5) {
		currentPoints[0][1][1] = -2;
		currentPoints[0][2][1] = -1;
		currentPoints[0][3][1] = 0;

		currentPoints[1][1][1] = -2;
		currentPoints[1][2][1] = -1;
		currentPoints[1][3][1] = 0;

		currentPoints[2][1][1] = -2;
		currentPoints[2][2][1] = -1;
		currentPoints[2][3][1] = 0;
	}

	else if (turn % 8 == 6) {
		currentPoints[0][1][1] = -1;
		currentPoints[0][2][1] = -2;
		currentPoints[0][3][1] = -1;

		currentPoints[1][1][1] = -1;
		currentPoints[1][2][1] = -2;
		currentPoints[1][3][1] = -1;

		currentPoints[2][1][1] = -1;
		currentPoints[2][2][1] = -2;
		currentPoints[2][3][1] = -1;
	}

	else if (turn % 8 == 7) {
		currentPoints[0][1][1] = 0;
		currentPoints[0][2][1] = -1;
		currentPoints[0][3][1] = -2;

		currentPoints[1][1][1] = 0;
		currentPoints[1][2][1] = -1;
		currentPoints[1][3][1] = -2;

		currentPoints[2][1][1] = 0;
		currentPoints[2][2][1] = -1;
		currentPoints[2][3][1] = -2;
	}

	glRotatef(0.7, 0.2, 0, 0.2);
	RenderScene();
	Sleep(200);
	turn++;
}

void init() {
	glClearColor(0.7, 0.7, 0.7, 1);
	//glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 10);
	//glEnable(GL_AUTO_NORMAL);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(450, 450);
	glutInitWindowPosition(150, 150);
	glutCreateWindow("red flag");
	glutDisplayFunc(RenderScene);
	glutIdleFunc(idle);
	init();
	glutMainLoop();
	return EXIT_SUCCESS;
}
