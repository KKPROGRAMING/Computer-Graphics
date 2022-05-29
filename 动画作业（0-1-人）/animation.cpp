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

//vector <P> points2 = { {15,-15},{15,-5},{15,5},{15,15}};
//vector <P> points1 = { {-50,-70},{-50,20},{60,60}};

vector <P> points3 = { {-25,0},{0,20},{25,0}}; //top
vector <P> points4 = { {25,0},{45,-25},{25,-50} }; //right
vector <P> points5 = { {-25,0},{-45,-25},{-25,-50} }; //left
vector <P> points6 = { {-25,-50},{0,-70},{25,-50} }; //bottom

vector <P> new3 = { {-20,-10},{0,0},{10,20} }; //left_up
vector <P> new4 = { {-20,-10},{-5,5},{10,20} }; //left_down
vector <P> new5 = { {0,0},{20,-10},{40,-20} }; //right_down
vector <P> new6 = { {0,0},{25,0},{40,-20} }; //right_up


int time = 50;
int oneFinished = 4;

int checkFinished[4] = { 0 };


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

//画出控制点
void paintPoints(vector<P> tmp) {
	for (int i = 0; i < tmp.size(); i++) {
		glVertex2d(tmp[i].x, tmp[i].y);
	}
}

void zero() {	
	
	glClear(GL_COLOR_BUFFER_BIT);

	//画出控制点
	glPointSize(5);
	glColor3f(255, 0, 0);
	//glBegin(GL_LINE_STRIP);
	glBegin(GL_POINTS);

	paintPoints(points3);
	paintPoints(points4);
	paintPoints(points5);
	paintPoints(points6);

	glEnd();
	glFlush();

	Sleep(70);

	//画出"0"
	glColor3f(0, 0, 255);

	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 1; t += bezier_t) {
		P now = Bezier_pos(points3, t);
		glVertex2d(now.x,now.y);
	}
	glEnd();
	glFlush();

	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 1; t += bezier_t) {
		P now = Bezier_pos(points4, t);
		glVertex2d(now.x, now.y);
	}
	glEnd();
	glFlush();

	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 1; t += bezier_t) {
		P now = Bezier_pos(points5, t);
		glVertex2d(now.x, now.y);
	}
	glEnd();
	glFlush();

	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 1; t += bezier_t) {
		P now = Bezier_pos(points6, t);
		glVertex2d(now.x, now.y);
	}
	glEnd();
	glFlush();

	glutSwapBuffers();

}

int ifSleep = 0;

void myIdle(void)
{

	//time += 0.5 ;
	/*if (time >= 60)
		time = 0;*/
	if (!ifSleep) {
		Sleep(500);
		ifSleep = 1;
	}

	if (oneFinished!=0) {
		int judge;
		//top
		if (!checkFinished[0]) {
			judge = 3;
			for (int i = 0; i < points3.size(); i++) {
				if (points3[i].x != 0) {
					points3[i].x -= 0.1 * points3[i].x;
					if (abs(points3[i].x) <= 0.02)
						points3[i].x = 0;
				}
				else
					judge -= 1;
			}
			if (judge == 0) {
				checkFinished[0] = 1;
				oneFinished -= 1;
			}
		}
		//right
		if (!checkFinished[1]) {
			judge = 3;
			for (int i = 0; i < points4.size(); i++) {
				if (points4[i].x != 0) {
					points4[i].x -= 0.1 * points4[i].x;
					if (abs(points4[i].x) <= 0.02)
						points4[i].x = 0;
				}
				else
					judge -= 1;
			}
			if (judge == 0) {
				checkFinished[1] = 1;
				oneFinished -= 1;
			}
		}
		//left
		if (!checkFinished[2]) {
			judge = 3;
			for (int i = 0; i < points5.size(); i++) {
				if (points5[i].x != 0) {
					points5[i].x -= 0.1 * points5[i].x;
					if (abs(points5[i].x) <= 0.02)
						points5[i].x = 0;
				}
				else
					judge -= 1;
			}
			if (judge == 0) {
				checkFinished[2] = 1;
				oneFinished -= 1;
			}
		}
		//bottom
		if (!checkFinished[3]) {
			judge = 3;
			for (int i = 0; i < points6.size(); i++) {
				if (points6[i].x != 0) {
					points6[i].x -= 0.1 * points6[i].x;
					if (abs(points6[i].x) <= 0.02)
						points6[i].x = 0;
				}
				else
					judge -= 1;
			}
			if (judge == 0) {
				checkFinished[3] = 1;
				oneFinished -= 1;
			}
		}
		zero();
	}
	else {
		//Sleep(1500);
		for (int i = 0; i < points3.size(); i++) {
			if (points3[i].x != new3[i].x)
				points3[i].x -= 0.1 * (points3[i].x - new3[i].x);
			if (points3[i].y != new3[i].y)
				points3[i].y -= 0.1 * (points3[i].y - new3[i].y);
		}
		for (int i = 0; i < points4.size(); i++) {
			if (points4[i].x != new4[i].x)
				points4[i].x -= 0.1 * (points4[i].x - new4[i].x);
			if (points4[i].y != new4[i].y)
				points4[i].y -= 0.1 * (points4[i].y - new4[i].y);
		}
		for (int i = 0; i < points5.size(); i++) {
			if (points5[i].x != new5[i].x)
				points5[i].x -= 0.1 * (points5[i].x - new5[i].x);
			if (points5[i].y != new5[i].y)
				points5[i].y -= 0.1 * (points5[i].y - new5[i].y);
		}
		for (int i = 0; i < points6.size(); i++) {
			if (points6[i].x != new6[i].x)
				points6[i].x -= 0.1 * (points6[i].x - new6[i].x);
			if (points6[i].y != new6[i].y)
				points6[i].y -= 0.1 * (points6[i].y - new6[i].y);
		}
		zero();
	}
}



void init() {
	glClearColor(1, 1, 1, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-70, 77, -77, 77);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("动画");

	init();
	glutDisplayFunc(zero);
	glutIdleFunc(myIdle);
	glutMainLoop();
	return 0;
}
