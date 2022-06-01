#include <windows.h>  
#include<GL\glew.h>
#include <gl\glut.h>
#include <gl\gl.h> 
#include <cstdlib>  
#include<vector>
#include<math.h>
using namespace std;

typedef struct _point {
	double x;
	double y;
	double z;
}P;

#define bezier_u 0.05
#define bezier_v 0.1

//控制点  
GLint nNumPoints = 9;
GLfloat ctrlPoints[9][3] = { { -4.0f, 0.0f, 4.0f},
							{-2.0f, 2.0f, 4.0f },
								{ 2.0f, 0.0f, 4.0f},
	
{  -4.0f, 0.0f, 0.0f},
{ -2.0f, 2.0f, 0.0f},
{ 2.0f, 0.0f, 0.0f},

{-4.0f, 0.0f, -4.0f},
{ -2.0f, 2.0f, -4.0f},
{ 2.0f, 0.0f, -4.0f},
};

//定义控制点
int group = 3;

P test1_1 = { -4.0f, 0.0f, 4.0f };
P test1_2 = { -2.0f, 2.0f, 4.0f };
P test1_3 = { 2.0f, 0.0f, 4.0f };
vector<P> arr1 = { test1_1,test1_2,test1_3};

P test2_1 = { -4.0f, 0.0f, 0.0f };
P test2_2 = { -2.0f, 2.0f, 0.0f };
P test2_3 = { 2.0f, 0.0f, 0.0f };
vector<P> arr2 = { test2_1,test2_2,test2_3};

P test3_1 = { -4.0f, 0.0f, -4.0f };
P test3_2 = { -2.0f, 2.0f, -4.0f };
P test3_3 = { 2.0f, 0.0f, -4.0f };
vector<P> arr3 = { test3_1,test3_2,test3_3};

vector<vector<P>> all = { arr1,arr2,arr3};

//画控制点  
void DrawPoints(void) {
	for (int i = 0; i < all.size(); i++) {
		glPointSize(5);
		glBegin(GL_POINTS);
		for (int j = 0; j < all[i].size(); j++) {
			glVertex3d(all[i][j].x, all[i][j].y, all[i][j].z);
		}
		glEnd();
		glFlush();
	}
}

//画坐标轴
void DrawAxis(void) {
	glColor3d(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3d(-5, 0, 0);
	glVertex3d(5, 0, 0);
	glVertex3d(0, -5, 0);
	glVertex3d(0, 5, 0);
	glVertex3d(0, 0, -5);
	glVertex3d(0, 0, 5);
	glEnd();
	glFlush();
}

//求组合数
double combination(int n, int i) {
	if (i == 0) {
		return 1;
	}
	else {
		double result = 1;
		for (int j = 0; j < i; j++) {
			result *= (n - j);
		}
		for (int j = 1; j <= i; j++) {
			result /= j;
		}
		return result;
	}
}

//求某个t时刻下贝塞尔曲线的点的位置（三维坐标）
P Bezier_pos(vector<P> tmp, double t) {
	P pos = { 0,0,0 };
	int n = tmp.size() - 1;//对应贝塞尔曲线的多项式和公式
	for (int i = 0; i <= n; i++) {
		pos.x += combination(n, i) * tmp[i].x * pow((1 - t), (n - i)) * pow(t, i);
		pos.y += combination(n, i) * tmp[i].y * pow((1 - t), (n - i)) * pow(t, i);
		pos.z += combination(n, i) * tmp[i].z * pow((1 - t), (n - i)) * pow(t, i);
	}
	return pos;
}


void RenderScene(void)
{
	// Clear the window with current clearing color 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawPoints();
	DrawAxis();

	GLfloat ambient[3] = { 0.2,0.7,0 };
	GLfloat diffuse[3] = { 0.2,0.2,0.0 };//a liitle yellow
	GLfloat specular[3] = { 1,1,1 };
	//开启灯光
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	GLfloat position[] = { 0.0f,10.0f,10.0f,0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);

	vector<P> new_;

	for (double t1 = 0; t1 <= 1; t1 += bezier_u) {
		for (int k = 0; k < all.size(); k++) {
			P now = Bezier_pos(all[k], t1);
			new_.push_back(now);
		}
	}

	for (size_t k = 0; k+group+1 < new_.size(); k++) {
		double length = sqrt(new_[k].x * new_[k].x + new_[k].y * new_[k].y + new_[k].z * new_[k].z);
		double nx = abs(new_[k].x)/length;
		double ny = abs(new_[k].y)/length;
		double nz = abs(new_[k].z)/length;
		glEnable(GL_NORMALIZE);
		glBegin(GL_POLYGON);
		glNormal3d(nx,ny,nz);
		//glBegin(GL_LINE_LOOP);
		glVertex3d(new_[k].x, new_[k].y, new_[k].z);
		glVertex3d(new_[k+1].x, new_[k+1].y, new_[k+1].z);
		glVertex3d(new_[k+group+1].x, new_[k+group+1].y, new_[k+group+1].z);
		glVertex3d(new_[k+group].x, new_[k+group].y, new_[k+group].z);
		glEnd();
	}
	glFlush();
	glutSwapBuffers();
}

void init() {
	glClearColor(0.7, 0.7, 0.7, 0);
	//glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 10);
	glEnable(GL_AUTO_NORMAL);
}

void idle() {
	glRotatef(2, 1, 1, 1);
	RenderScene();
	Sleep(200);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(450, 450);
	glutInitWindowPosition(150, 150);
	glutCreateWindow("flag");
	glutDisplayFunc(RenderScene);
	glutIdleFunc(idle);
	init();
	glutMainLoop();
	return EXIT_SUCCESS;
}
