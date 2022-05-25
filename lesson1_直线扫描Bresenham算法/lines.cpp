#include<GL/glut.h>
//bresenham划线算法
//绘制斜率大于0且小于1的线段
void lines() {

	//int p1[] = { 6,4 };
	//int p2[] = { 1,1 };
	//int p3[] = { 3,7 };
	//int p4[] = { 5,1 };
	//int p5[] = { 0,4 };
  
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.5, 0.2, 0.4);
  
	//glPointSize(5);
	/*glBegin(GL_POINTS);*/
	//glBegin(GL_LINES);//绘制线，两点构成一条线段
	//glBegin(GL_LINE_STRIP);//绘制连起来的线段，顺着点相连
	//glBegin(GL_LINE_LOOP);//有闭口的GL_LINE_STRIP
	//glBegin(GL_POLYGON);//可看到有颜色的填充

	int x1 = -40, y1 = -60, x2 = 80, y2 = 30;
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int x, y;
	int temp1 = 2 * dy;
	int temp2 = 2 * (dy - dx);
	int p = temp1 - dx;
	if (x1 > x2) {
		x = x2;
		y = y2;
		x2 = x1;
	}
	else {
		x = x1;
		y = y1;
	}

	glBegin(GL_LINES);
	glVertex2i(x, y);
	while (x < x2) {
		x++;
		if (p < 0) {
			p += temp1;
		}
		else {
			y++;
			p += temp2;
		}
		glVertex2i(x, y);
	}

	glEnd();
	glFlush();
}


void init() {
	glClearColor(1, 1, 1, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-70, 90, -70, 40);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(600, 500);
	glutCreateWindow("lines");

	init();
	glutDisplayFunc(lines);
	glutMainLoop();
	return 0;
}
