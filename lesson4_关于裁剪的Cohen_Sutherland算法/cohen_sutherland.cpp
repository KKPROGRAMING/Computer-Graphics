#include<GL/glut.h>
#include<vector>
using namespace std;

#define EXTEND 40 //延长线长度

//定义视野范围
#define BOTTOM -50
#define TOP 50
#define LEFT -50
#define RIGHT 50
//Cohen_Sutherland裁剪算法

//定义检测两点间关系的函数
int check_y(int y1, int y2);
int check_x(int x1, int x2);

//定义点结构体存放点的坐标
typedef struct {
	int x;
	int y;
}point;

void cut() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.5, 0.2, 0.4);

	//定义两个动态数组All[]和New[],并初始化
	//前者用于存放待检测的点，后者存放检测后得到的点

	point p0 = { 40,-30};
	point p1 = { 50,60 };
	point p2 = { 20,70 };
	point p3 = { -80,0 };


	vector <point> All;
	vector <point> New ;

	All.push_back(p0);
	All.push_back(p1);
	All.push_back(p2);
	All.push_back(p3);


	//先画出视野区域
	glBegin(GL_LINES);

	glVertex2i(LEFT, TOP +EXTEND);
	glVertex2i(LEFT, BOTTOM - EXTEND);
	glVertex2i(RIGHT, TOP + EXTEND);
	glVertex2i(RIGHT, BOTTOM - EXTEND);

	glVertex2i(LEFT-EXTEND, TOP);
	glVertex2i(RIGHT +EXTEND, TOP);
	glVertex2i(LEFT - EXTEND, BOTTOM);
	glVertex2i(RIGHT + EXTEND, BOTTOM);

	glEnd();
	glFlush();

	//延时1秒后，画出待裁剪的图形
	Sleep(1000);

	glBegin(GL_LINE_LOOP);

	glVertex2i(p0.x,p0.y);
	glVertex2i(p1.x, p1.y);
	glVertex2i(p2.x, p2.y);
	glVertex2i(p3.x, p3.y);

	glEnd();
	glFlush();

	//延时1秒后，开始接下来的对于y的裁剪操作
	Sleep(1000);

	/*后来思考发现，以下这一步是多余的！会导致多余的点进入New中*/
	//对All中是否有点位于视野范围内进行检测
	//for (int i = 0; i < All.size(); i++) {
	//	if (All[i].x >= LEFT && All[i].x <= RIGHT) {
	//		if (All[i].y >= BOTTOM && All[i].y <= TOP) {
	//			New.push_back(All[i]);
	//			//有则将该点推入到New数组中
	//		}
	//	}
	//}

	//对top和bottom之间的范围进行检测
	//嵌套循环实现遍历边
	//for (int i = 0; i < All.size(); i++) {
		for (int j = 0; j <= All.size()-1; j++) {
			point pa = All[j];
			point pb;

			//因为要对遍历的边形成闭环，所以要记得All[All.size()-1]点与All[0]点之间的边也要检测到！
			if (j == All.size() - 1)
				pb = All[0];
			else
				pb = All[j + 1];

			int flag = 0;//判断在推入New时两点是否要交换顺序
			if (check_y(pa.y, pb.y) == 2) {

				if (pa.y < pb.y) {
					point chg = pa;
					pa = pb;
					pb = chg;

					flag = 1;//要交换顺序
				}
				//让高的点作为pa，便于后续的计算

				if (pb.y <= TOP && pb.y>=BOTTOM) {
					//此时pa在TOP以上，pb在视野范围高度内
					//返回一个交点
					point tmp;
					tmp.x = pa.x + (pb.x - pa.x) * (pa.y - TOP) / (pa.y - pb.y);
					tmp.y = TOP;
					New.push_back(tmp);
				}
				else if (pa.y <= TOP && pa.y >= BOTTOM) {
					//此时pa在视野范围高度内，pb在BOTTOM以下
					//返回一个交点
					point tmp;
					tmp.x = pa.x + (pb.x - pa.x) * (pa.y - BOTTOM) / (pa.y - pb.y);
					tmp.y = BOTTOM;
					New.push_back(tmp);
				}
				else {
					//最后一种情况，即是pa在TOP以上，pb在BOTTOM以下（此处用else便于计算）
					//返回两个交点
					point tmp1;
					tmp1.x = pa.x + (pb.x - pa.x) * (pa.y - TOP) / (pa.y - pb.y);
					tmp1.y = TOP;
					//New.push_back(tmp1);

					point tmp2;
					tmp2.x = pa.x + (pb.x - pa.x) * (pa.y - BOTTOM) / (pa.y - pb.y);
					tmp2.y = BOTTOM;
					//New.push_back(tmp2);

					if (flag == 1) {
						New.push_back(tmp2);
						New.push_back(tmp1);
					}
					else {
						New.push_back(tmp1);
						New.push_back(tmp2);
					}
				}
			}
			else if (check_y(pa.y, pb.y) == 1) {
				New.push_back(pa);
				New.push_back(pb);
			}
		}
	//}

	//All.~vector<point>();
	//vector <point> All(New); //销毁现有的All后，重新创建一个和New内容一样的All
	//New.clear(); //清空New中的内容

	/*由于以上的三步操作会导致报错All的重复声明，
	而我又不知道如何解决，
	所以这里采用另外定义一个Then数组来存放检测出来的点*/
	vector <point> Then(New);
	New.clear();

	//a little test!
	//glBegin(GL_LINE_LOOP);
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 1, 0);

	for (int i = 0; i < Then.size(); i++) {
		glVertex2i(Then[i].x,Then[i].y);
	}

	glEnd();
	glFlush();

	//延时1秒后，开始接下来的对于x的裁剪操作
	Sleep(1000);

	//对left和right之间的范围进行检测
	//for (int i = 0; i < Then.size(); i++) {
		for (int j = 0; j <= Then.size()-1; j++) {
			point pa = Then[j];
			point pb;
			if (j == Then.size() - 1)
				pb = Then[0];
			else
				pb = Then[j + 1];

			int flag = 0;

			if (check_x(pa.x, pb.x) == 2) {

				if (pa.x < pb.x) {
					point chg = pa;
					pa = pb;
					pb = chg;
					flag = 1;
				}
				//让高的点作为pa，便于后续的计算

				if (pb.x <= RIGHT && pb.x >= LEFT) {
					//此时pa在right往右，pb在视野范围宽度内
					//返回一个交点
					point tmp;
					tmp.x = RIGHT;
					tmp.y = pa.y + (pb.y-pa.y)*(RIGHT-pa.x)/(pb.x-pa.x);
					New.push_back(tmp);
				}
				else if (pa.y <= TOP && pa.y >= BOTTOM) {
					//此时pa在视野范围宽度内，pb在left往左
					//返回一个交点
					point tmp;
					tmp.x = LEFT;
					tmp.y = pa.y + (pb.y - pa.y) * (LEFT - pa.x) / (pb.x - pa.x);
					New.push_back(tmp);
				}
				else {
					//最后一种情况，即是pa在TOP以上，pb在BOTTOM以下（此处用else便于计算）
					//返回两个交点
					point tmp1;
					tmp1.x = RIGHT;
					tmp1.y = pa.y + (pb.y - pa.y) * (RIGHT - pa.x) / (pb.x - pa.x);
					//New.push_back(tmp1);

					point tmp2;
					tmp2.x = LEFT;
					tmp2.y = pa.y + (pb.y - pa.y) * (LEFT - pa.x) / (pb.x - pa.x);
					//New.push_back(tmp2);

					if (flag == 1) {
						New.push_back(tmp2);
						New.push_back(tmp1);
					}
					else {
						New.push_back(tmp1);
						New.push_back(tmp2);
					}
				}
			}
			else if (check_x(pa.x, pb.x) == 1) {
				New.push_back(pa);
				New.push_back(pb);
			}
		}
	//}

	//新定义一个End数组用以存放最终的结果
	vector <point> End(New);
	New.clear();

	//延时1秒，呈现最终的裁剪结果
	Sleep(1000);

	//a little test!
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 0, 1);


	for (int i = 0; i < End.size(); i++) {
		glVertex2i(End[i].x, End[i].y);
	}

	glEnd();
	glFlush();

}


void init() {
	glClearColor(1, 1, 1, 0);//white
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-100,100,-100,100);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Cohen_Sutherland算法");

	init();
	glutDisplayFunc(cut);
	glutMainLoop();
	return 0;
}

int check_y(int y1, int y2) {
	if ((y1>=TOP && y2>=TOP) || (y1<=BOTTOM && y2<=BOTTOM)) {
		return 0;//两点在同一侧，但不在视野内
	}
	else if((y1<=TOP && y1>=BOTTOM)&&(y2 <= TOP && y2 >= BOTTOM)) {
		return 1;//两点同在视野内
	}
	else {
		return 2;//两点不同区域
	}
}

int check_x(int x1, int x2) {
	if ((x1 >= RIGHT && x2 >= RIGHT) || (x1 <= LEFT && x2 <= LEFT)) {
		return 0;
	}
	else if ((x1 <= RIGHT && x1 >= LEFT) && (x2 <= RIGHT && x2 >= LEFT)) {
		return 1;
	}
	else {
		return 2;
	}
}
