
//扫描线算法
#include<iostream>
#include<gl/glut.h>
#include<algorithm>
#include<vector>
using namespace std;
struct point
{
	float x, y;
};
vector<point> vertice; //顶点

typedef struct XET
{
	float x;
	float dx;  // 从当前扫描线到下一条扫描线间x的增量，即斜率的倒数
	float ymax; //该边所交的最高扫描线的坐标值ymax
	XET* next;
}AET, NET; //AET 活性边表； NET新边表

void draw_lines();
void draw_a_point(int x, int y);
void PolyScan();
void display();

void display()
{
	draw_lines();
	Sleep(2*1000);//为了将原始存在的边与后来填充的颜色做区别，延时2秒
	PolyScan();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(600, 600);
	glutCreateWindow("多边形扫描线填充算法");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-100, 500, -100, 500);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glutDisplayFunc(&display);

	glutMainLoop();
	return 0;
}

//画点函数
void draw_a_point(int x, int y)
{
	glPointSize(1);
	glBegin(GL_POINTS);
	glColor3f(1, 1, 0);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}

void PolyScan()
{
	/*得到最高点和最低点的y坐标*/
	int Max_Y = 0;
	int Min_Y = 999;
	for (int i = 0; i < vertice.size(); i++) {
		if (vertice[i].y > Max_Y)
			Max_Y = vertice[i].y;
		if (vertice[i].y < Min_Y)
			Min_Y = vertice[i].y;
	}

	//初始化AET表，链表实现
	AET* pAET = new AET;
	pAET->next = NULL;

	//初始化NET表，数组实现
	NET* pNET[800];
	for (int i = 0; i <= Max_Y; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;;
	}

	//扫描并且建立NET表
	int len = vertice.size(); //顶点个数
	for (int i = Min_Y; i <= Max_Y; i++)
	{
		for (int j = 0; j < len; j++) //扫描每个点
		{
			if (i == vertice[j].y)
			{
				//判断当前行（扫描线）上是否有点，若有点则根据列坐标（x）从小到大放入pNET[i]链表中

				if (vertice[(j - 1 + len) % len].y > vertice[j].y)
				{
					NET* p = new NET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j - 1 + len) % len].y;
					float DX = vertice[(j - 1 + len) % len].x - vertice[j].x;
					float DY = vertice[(j - 1 + len) % len].y - vertice[j].y;
					p->dx = DX / DY;

					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
				if (vertice[(j + 1) % len].y > vertice[j].y)
				{
					NET* p = new NET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j + 1) % len].y;
					float DX = vertice[(j + 1) % len].x - vertice[j].x;
					float DY = vertice[(j + 1) % len].y - vertice[j].y;
					p->dx = DX / DY;

					p->next = pNET[i]->next;//因为是插到队头，所以这里p->next不是指向NULL，而是指向pNET[i]的第一个元素
					pNET[i]->next = p;
				}
			}
		}
	}

	//建立并且更新活性边表AET
	for (int i = Min_Y; i <= Max_Y; i++)
	{
		//计算每条扫描线上不同线产生的新的交点x，更新AET
		NET* p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx; //更新x坐标
			p = p->next;
		}

		//断表排序,不再开辟空间 
		AET* tq = pAET;
		p = pAET->next;
		tq->next = NULL;
		while (p)//顺着链表往下走
		{
			//找到第一个比它大的数字tq->next->next->x，则从p->next到tq->next都是比p->x小的
			while (tq->next != NULL && tq->next->x <= p->x)
				tq = tq->next;
			//插入p到tq和tq->next之间
			NET* t = p->next;
			p->next = tq->next;
			tq->next = p;
			p = t;

			tq = pAET;//回到头
		}

		//取消求交，减少计算量
		//先从AET表中删除ymax==i的结点
		//像素的取舍问题，保证多边形的“下闭上开”，避免填充扩大化（交点的个数应保证为偶数个）
		AET* q = pAET;
		p = q->next;
		while (p)
		{
			if (p->ymax == i)
			{
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else
			{
				q = q->next;
				p = q->next;
			}
		}

		//若NET中有新点，将其用插入法插入AET，按x递增的顺序排列
		p = pNET[i]->next;
		q = pAET;
		while (p)
		{
			while (q->next && p->x >= q->next->x)
				q = q->next;
			//插入p
			NET* t = p->next;
			p->next = q->next;
			q->next = p;
			p = t;

			q = pAET;//回到头
		}

		//配对后填充颜色
		p = pAET->next;
		while (p && p->next != NULL)
		{
			for (float j = p->x; j <= p->next->x; j++)
			{
				draw_a_point(j, i);
			}
			p = p->next->next;//考虑端点情况
		}
		printf("第%d行已填充完成！\n", i);
	}
	
	glEnd();
	glFlush();
}
void draw_lines()
{
		glClearColor(1, 1, 1, 1);
		glColor3f(1, 0, 0);

		glPointSize(1);
		glBegin(GL_LINE_LOOP);

		//vertice = { {140,160},{234,376},{115,260},{80,169} };
		vertice = { {140,160},{234,376},{315,260}};
		//vertice = { {80,169},{140,160},{234,376},{315,260},{60.177}};
		for (int i = 0; i < vertice.size(); i++)
		{
			glVertex2f(vertice[i].x, vertice[i].y);
		}
		glEnd();
		glFlush();
}


