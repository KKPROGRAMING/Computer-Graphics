#include<GL/glut.h>
#include<vector>
#include<math.h>
using namespace std;
//贝塞尔曲线实现

#define bezier_t 0.001

typedef  struct _POINT {
	double x;
	double y;
}P;

vector <P> points2 = { {15,-15},{15,-5},{15,5},{15,15}};
vector <P> points1 = { {-50,-70},{-50,20},{60,60}};
vector <P> points3 = { {},{},{},{} };

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

//求某个t时刻下贝塞尔曲线的点的位置
P Bezier_pos(vector<P> tmp , double t) {
	P pos = {0,0};
	int n = tmp.size() - 1;//对应贝塞尔曲线的多项式和公式
	for (int i = 0; i <= n; i++) {
		pos.x += combination(n, i) * tmp[i].x * pow((1 - t), (n - i)) * pow(t, i);
		pos.y += combination(n, i) * tmp[i].y * pow((1 - t), (n - i)) * pow(t, i);
	}
	return pos;
}

void bezier() {
	
	glClear(GL_COLOR_BUFFER_BIT);

	//画出控制点
	glPointSize(5);
	glColor3f(255, 0, 0);//以上这两项设置要放在glBegin之前有效果！
	//glBegin(GL_LINE_STRIP);
	glBegin(GL_POINTS);

	for (int i = 0; i < points2.size(); i++) {
		glVertex2d(points2[i].x, points2[i].y);
	}

	glEnd();
	glFlush();

	//画出贝塞尔曲线
	glColor3f(0, 0, 255);
	glBegin(GL_LINE_STRIP);
	
	for (double t = 0; t <= 1; t += bezier_t) {
		P now = Bezier_pos(points2, t);
		glVertex2d(now.x,now.y);
	}

	glEnd();
	glFlush();
}

void init() {
	glClearColor(1, 1, 1, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-70, 120, -100, 100);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("贝塞尔曲线");

	init();
	glutDisplayFunc(bezier);
	glutMainLoop();
	return 0;
}
