#include <windows.h>  
#include <gl/GL.h>  
#include <gl/glu.h>  
#include <gl/glut.h>  
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

//定义控制点
P test1_1 = { -15,13,7 };
P test1_2 = { -13,23,-5 };
P test1_3 = { -10,12,-15 };
P test1_4 = { -14,4,5 };
vector<P> arr1 = {test1_1,test1_2,test1_3,test1_4};

P test2_1 = { -5,6,12};
P test2_2 = { -4,14,5 };
P test2_3 = { -13,3,-5 };
P test2_4 = { -2,2,15 };
vector<P> arr2 = { test2_1,test2_2,test2_3,test2_4 };

P test3_1 = { 5,-8.5,15};
P test3_2 = { 4.6,-4,5 };
P test3_3 = { 3,-1.3,-5 };
P test3_4 = { 2.8,-2,15 };
vector<P> arr3 = { test3_1,test3_2,test3_3,test3_4 };

P test4_1 = { 15,-1.5,15};
P test4_2 = { 6.4,-14,5 };
P test4_3 = { 10,-13,-5 };
P test4_4 = { 12,-12,-17 };
vector<P> arr4 = { test4_1,test4_2,test4_3,test4_4 };

vector<vector<P>> all = { arr1,arr2,arr3,arr4 };
//

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


void init()
{
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
   // glOrtho(-20.0, 20.0, -20.0, 20.0, -20.0, 20.0);
    glOrtho(-25.0, 25.0, -25.0, 25.0, -25.0, 25.0);
  //gluLookAt(12.0, 4.0, 2.0, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);//设置观测点
    gluLookAt(5, 4, -2, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);

}

void display()
{
    //贝塞尔曲面

    //画出控制点，让其连成线
    //glPointSize(3);
    //glBegin(GL_POINTS);
    double green = 0.1;
    double blue = 1;
    for (int i = 0; i < all.size(); i++) {
        glColor3d(0, green, blue);
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < all[i].size(); j++) {
            glVertex3d(all[i][j].x, all[i][j].y, all[i][j].z);
        }
        glEnd();
        glFlush();
        green += 0.2;
        blue -= 0.3;
    }
    

    Sleep(300);

    //画出贝塞尔曲线
    //临时数组，用于存放贝塞尔曲线组的各个点
    vector<vector<P>> tmp;// = { tmp1 , tmp2 , tmp3 ,tmp4 };

    //使用第一个参数bezier_u，得到第一次的贝塞尔曲线组
    
    double red2 = 0.1;
    double green2 = 1.0;
    double blue2 = 0.5;
    
        for (double t1 = 0; t1 <= 1; t1 += bezier_u) {
            //vector<P> tmp;

            vector<P> temp;
            glColor3d(red2, green2, blue2);
            glBegin(GL_LINE_STRIP);
            for (int k = 0; k < all.size(); k++) {
                P now;
                now = Bezier_pos(all[k], t1);
                temp.push_back(now);
                glVertex3d(now.x, now.y, now.z);
                printf("all[%d],%lf\n",k, t1); //for test
                Sleep(20);
            }
            glEnd();
            glFlush();
            red2 += 0.005;
            green2 -= 0.02;
            blue2 += 0.1;
            tmp.push_back(temp);
         
        }

        //printf("the first computing is finished.\n");//for test
       //Sleep(5000);
        vector<vector<P>> result;
        double red3 = 0.01;
        double green3 = 1.0;
        double blue3 = 0.7;
        //glColor3d(1, 0, 0);
        //glBegin(GL_LINE_LOOP);
        //glBegin(GL_POLYGON);
        //glBegin(GL_QUADS);
        for (double t2 = 0; t2 <= 1; t2 += bezier_v) {
            glColor3d(red3, green3, blue3);
            //glColor3d(0, 1, 0);
            //glBegin(GL_LINE_LOOP);
            glBegin(GL_POLYGON);
            for (int i = 0; i < tmp.size(); i++) {
                P now;
                now = Bezier_pos(tmp[i], t2);
                glVertex3d(now.x, now.y, now.z);
                /*glVertex3d(now.x-1, now.y, now.z);
                glVertex3d(now.x-1, now.y-1, now.z);
                glVertex3d(now.x, now.y-1, now.z);*/
                //glEnable(GL_NORMALIZE);
                //glNormal3d(now.x, now.y, now.z);


                    //printf("!\n");
            }
            Sleep(50);
            glEnd();
            glFlush();
            red3 += 0.03;
            green3 += 0.02;
            blue3 -= 0.01;
        }
        //glEnd();
        //glFlush();
        Sleep(100000);

}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(450, 450);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("rotate sphere");
    glutDisplayFunc(display);

    init();
    glutMainLoop();
    return EXIT_SUCCESS;
}
