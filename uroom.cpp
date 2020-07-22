#include <gl/glut.h>
#include<stdio.h>
#include<math.h>
#include <Windows.h>


int angle = -100;
int tx = 40, tz = 40;
float ratio;

float x = 100.0f, y = 150.0f, z = 150.0f;//摄像机位置
float ox = 50.0f, oy = 0.0f, oz = 0.0f;//摄像机观察的物体的位置



//纹理名字
static GLuint wall1,wall2,wall3,table,teapot,ground;
GLuint WALL=wall1;
int flag = 1;
bool light0 = false;
bool light1 = false;
bool light2 = false;
bool wenliTea = true;


bool LoadTexture(const char* szFileName, GLuint &texid)
{
	HBITMAP hBMP;
	BITMAP BMP;

	glGenTextures(1, &texid);
	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	if (!hBMP)
		return false;

	GetObject(hBMP, sizeof(BMP), &BMP);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, texid); // Bind To The Texture ID

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

	glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	DeleteObject(hBMP); // Delete The Object

	return true;

}


void reshape(int w, int h)
{

	if (h == 0)
		h = 1;
	ratio = 1.0f*w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);//设置视口为整个窗口大小
	
	//设置可视空间
	gluPerspective(45, ratio, 1, 1000);  //透视效果

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x,y,z,ox,oy,oz,0.0f,1.0f,0.0f);//设置观察点

}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);//启动光照


	GLfloat amibientLight[] = { 0.2,0.2,0.2,1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,amibientLight);//全局环境光


	//0号灯：平行绿光
	GLfloat light0_ambient[] = { 0.0f,1.0f,0.0f,1.0f };
	GLfloat light0_diffuse[] = { 1.0f,0.5f,0.5f,0.5f };
	GLfloat light0_specular[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat light0_position[] = { 50.0f,50.0f,50.0f,0.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);


	//1号灯：红色点光源
	GLfloat light1_ambient[] = { 1.0f,0.0f,0.0f,1.0f };
	GLfloat light1_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat light1_specular[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat light1_position[] = { 100.0f,70.0f,30.0f,1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	//2号灯：白色平行光
	GLfloat light2_ambient[] = { 0.7f,0.7f,0.7f,1.0f };
	GLfloat light2_diffuse[] = { 1.0f,0.5f,0.5f,0.5f };
	GLfloat light2_specular[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat light2_position[] = { 50.0f,50.0f,50.0f,0.0f };

	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

	



	/*
	
	//聚光灯
	GLfloat spot_position[] = { 100.0, 70.0,30.0 };
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.2);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_position);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 2.0);

	glEnable(GL_LIGHT3);
	*/

	glShadeModel(GL_SMOOTH);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	LoadTexture("ground.bmp", ground);
	LoadTexture("wall1.bmp",wall1);
	LoadTexture("wall2.bmp", wall2);
	LoadTexture("wall3.bmp", wall3);
	LoadTexture("table.bmp", table);
	LoadTexture("teapot.bmp", teapot);
}


void Keys(unsigned char key, int x, int y)
{

	switch (key)
	{
	case '1':
		light0 = !light0;
		if (light0)
		{
			glEnable(GL_LIGHT0);
		}
		else 
		{
			glDisable(GL_LIGHT0);
		}
		break;
	case '2':
		light1 = !light1;
		if (light1)
		{
			glEnable(GL_LIGHT1);
		}
		else
		{
			glDisable(GL_LIGHT1);
		}
		break;
	case '3':
		light2 = !light2;
		if (light2)
		{
			glEnable(GL_LIGHT2);
		}
		else
		{
			glDisable(GL_LIGHT2);
		}
		break;
	case '4':
		wenliTea = !wenliTea;
		break;
	case '5':
		if(flag%3==0)
			WALL = wall1;
		if (flag % 3 == 1)
			WALL = wall2;
		if (flag % 3 == 2)
			WALL = wall3;
		flag++;
		break;
	case '6':
		angle += 10;
		angle %= 360;
		break;
	case 'W':
	case 'w':
		x += 1.0f;
		break;
	case 'S':
	case 's':
		x -= 1.0f;
		break;
	case 'A':
	case 'a':
		z -= 1.0f;
		break;
	case 'D':
	case 'd':
		z += 1.0f;
		break;
	case 'Q':
	case 'q':
		y += 1.0f;
		break;
	case 'E':
	case 'e':
		y -= 1.0f;
		break;
	default:
		break;
	}
	glutPostRedisplay();
	glLoadIdentity();
	gluLookAt(x, y, z,  ox,  oy,  oz, 0.0f, 1.0f, 0.0f);
	glFlush();
}

void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if(tx-35>=-100)
		tx -= 5;
		break;
	case GLUT_KEY_DOWN:
		if (tx+35 <= 100)
			tx += 5;
		break;
	case GLUT_KEY_LEFT:
		if (tz+35 <= 100)
			tz += 5;
		break;
	case GLUT_KEY_RIGHT:
		if (tz-35 >= -100)
			tz -= 5;
		break;


	default:
		break;
	}
	glutPostRedisplay();
	glFlush();
}


void drawPillar()
{
	glPushMatrix();
	glTranslatef(0, 15, 0);
	glScalef(2, 30, 2);
	glutSolidCube(1);
	glPopMatrix();
}

void drawDesk()
{
	//桌面
	
	glPushMatrix();	
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, table);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTranslatef(5, 30, 5);
	glScalef(60, 2, 60);
	glutSolidCube(1);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	//桌腿
	glColor3f(0.0f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(27.5, 0, 27.5);
	drawPillar();
	glTranslatef(0, 0, -45);
	drawPillar();
	glTranslatef(-45, 0, 45);
	drawPillar();
	glTranslatef(0, 0, -45);
	drawPillar();
	glPopMatrix();
}


void drawBed()
{
	glPushMatrix();
	//床头
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.54f, 0.5f);
	glVertex3f(-80, 0, -50);
	glVertex3f(0, 0, -50);
	glVertex3f(0, 50, -50);
	glVertex3f(-80, 50, -50);
	glEnd();

	//床板
	glBegin(GL_QUADS);
	glColor3f(1.f, 0.54f, 0.5f);
	glVertex3f(0, 25, -50);
	glVertex3f(-80, 25, -50);
	glVertex3f(-80, 25, 50);
	glVertex3f(0, 25, 50);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1.f, 0.54f, 0.5f);
	glVertex3f(-80, 25, -50);
	glVertex3f(-80, 0, -50);
	glVertex3f(-80, 0, 50);
	glVertex3f(-80, 25, 50);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1.f, 0.54f, 0.5f);
	glVertex3f(0, 25, -50);
	glVertex3f(0, 0, -50);
	glVertex3f(0, 0, 50);
	glVertex3f(0, 25, 50);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1.f, 0.54f, 0.5f);
	glVertex3f(-80, 25, 50);
	glVertex3f(0, 25, 50);
	glVertex3f(0, 0, 50);
	glVertex3f(-80, 0, 50);
	glEnd();

	//枕头
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.84f, 0.5f);
	glVertex3f(-60, 26, -40);
	glVertex3f(-60, 26, -20);
	glVertex3f(-30, 26, -20);
	glVertex3f(-30, 26, -40);
	glEnd();


	//被子
	glPushMatrix();
	glTranslatef(-40, 25, 20);
	glScalef(80, 1, 60);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();
}

void drawLamp()
{
	
	glPushMatrix();
	glTranslatef(100, 35, 30);
	glScalef(3, 70, 3);
	glColor3f(0.7f, 0.7f, 0.7f);
	glutSolidCube(1);
	glPopMatrix();

	glColor3f(1.0f, 0.6f, 0.2f);
	glPushMatrix();
	glTranslatef(100, 70, 30);
	glRotatef(190, 180, 200, 270);
	glutSolidCone(15, 25, 30, 25);
	glPopMatrix();
}

void drawWall()
{
	//画墙

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, WALL);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-100.0f, 200.0f, -100.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-100.0f, 200.0f, 100.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-100.0f, 200.0f, -100.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(100.0f, 200.0f, -100.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}

void drawGround()
{
	//画地板
	
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ground);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}

void drawTeapot()
{
	//画茶壶

	glPushMatrix();
	if (wenliTea)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, teapot);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}

	GLfloat mat_shininess[] = { 40.0f };
	GLfloat mat_specular[] = { 1.0f,1.0f,1.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glColor3f(0.6f, 0.6f, 0.6f);
	glTranslatef(60, 38, 60);
	glRotatef(angle, 0, 1, 0);
	glutSolidTeapot(10);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Display()
{
	//清除已经存在的画面
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_COLOR_MATERIAL);//启用颜色追踪
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


	drawWall();

	drawGround();

	drawBed();

	drawLamp();

	glPushMatrix();
	glTranslatef(tx, 0, tz);
	drawDesk();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	drawTeapot();

	glutSwapBuffers();
	glFlush();


}



int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("uroom");
	init();
	glutReshapeFunc(reshape);
	glutKeyboardFunc(Keys);
	glutSpecialFunc(specialKeys);
	glutDisplayFunc(Display);

	glutMainLoop();
	return 0;
}
