#include<bits/stdc++.h>
#include"windows.h"
#include"Model.h"
#define GLUT_DISABLE_ATEXIT_HACK
#define MAX_CHAR 128
#include <GL/glut.h>
const int Height=1200,Width=1960,MaxLength=40;
const float SpeedBaseX=1.0,SpeedBaseY=0.12;
Model2d Tree[20],Stars[40],Road,Line[3],Sun,Police,Thief;
string AimStr;
GLuint lists;
float Px,Py,Dis;
int cnt=0,Count=3;
void init();
void Display();
void Reshape(int w,int h);
void NormalKey(unsigned char ch,int x,int y);
void Win();
void Lose();
void drawString(string str);
void drawChar(const char ch);
void Draw_Aim();
void ReadText(char * File);
void FixDistance();
void selectFont(int size,int charset,const char*face);
void CountDown();
int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitWindowSize(Width,Height);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");
    init();
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(NormalKey);
    glutMainLoop();
    return EXIT_SUCCESS;
}
void init(){
    glClearColor(0.6f,0.6f,0.8f,1.0f);
    srand(time(0));

    //Tree
    for(int i=0;i<20;i++){
        float SCALE=rand()%20*0.1+8;
        float Trans=rand()%100*0.001+(10-i)*1.5;
        Tree[i].Read("Model\\Tree.txt",Trans,-1,SCALE,SCALE);
    }

    //Stars
//    for(int i=0;i<40;i++){
//        float SCALE=0.012*(rand()%1000+500);
//        float Trans=0.1f*(rand()%80)+(i/5-4)*12.0f;
//        float Y=rand()%80*0.2;
//        Stars[i].Read("Model\\Star.txt",Trans,Y,SCALE,SCALE);
//    }

    //Road
    Road.Read("Model\\Road.txt",0,-2,100,10);
    Line[0].Read("Model\\Line.txt",0,-19,100,1);
    Line[1].Read("Model\\Line.txt",0,-21,100,1.2);
    Line[2].Read("Model\\Line.txt",0,-23,100,1.4);

    //Sun
    Sun.Read("Model\\Sun.txt",27,27,1,1);

    //Police
    Px=-20,Py=-6;
    Police.Read("Model\\Police.txt",Px,Py,3,3);
    Police.AddRigid(2,4.5);
    //Police.SetRigidVisible(true);
    //Thief
    Dis=20;
    float Tx=Px+Dis,Ty=Py;
    Thief.Read("Model\\Thief.txt",Tx,Ty,3,3);
    Thief.AddRigid(2,4.5);
    //Thief.SetRigidVisible(true);

    //AimStr
    ReadText("Text\\Text.txt");
}
void Display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);
    glLoadIdentity();
    gluLookAt(0.0,0.0,100.0, 0,0,0, 0,1,0);

    //Draw All the Objects
    Road.Draw();
    for(int i=0;i<3;i++){
        Line[i].Draw();
    }
    for(int i=0;i<20;i++){
        glPushMatrix();
        Tree[i].AddMov(-0.001,0);
        Tree[i].Draw();
        if(Tree[i].Mov_X<-15){
            Tree[i].Mov_X=15;
            float SCALE=(rand()%1000+500)*0.003+5.0;
            Tree[i].SetScale(SCALE,SCALE);
        }
        glPopMatrix();
    }
    Sun.AddRotated(0.02);
    Sun.Draw();
    if(Count==0){
        Draw_Aim();
        FixDistance();
    }
    Police.SetMov(Px,Py);
    Police.Draw();
    Thief.SetMov(Px+Dis,Py);
    Thief.Draw();
    if(Count!=0){
        CountDown();
        Count--;
    }
	glLoadIdentity();
    glutPostRedisplay();
    glutSwapBuffers();
}
void Reshape(int w,int h){
    glViewport(0,0, (GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0,(GLfloat)w/(GLfloat)h,0.0,10000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void drawString(string str) {
    static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) {
		isFirstCall = 0;
		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);
		// 把每个字符的绘制命令都装到对`应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for(int i=0;i<str.size();i++)
		glCallList(lists + str[i]);
}
void drawChar(const char ch){
    static int isFirstCall = 1;
	static GLuint lists;
	if (isFirstCall) {
		isFirstCall = 0;
		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);
		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
    glCallList(lists+ch);
}
void NormalKey(unsigned char ch,int x,int y){
    switch(ch){
        case 27:exit(0);break;
    }
    if(Count!=0)
        return;
    if(ch==AimStr[cnt]){
        cnt++;
        Px+=SpeedBaseX;
        Dis-=SpeedBaseX;
    }
    if(Police.Coll(Thief)){
        Win();
    }
}
void Win(){
    glClear(GL_COLOR_BUFFER_BIT);
    selectFont(400, ANSI_CHARSET, "Comic Sans MS");
    glColor3f(0,0,1);
    glRasterPos3f(-6,-1,-20);
    drawString("You win");
    glutPostRedisplay();
    glutSwapBuffers();
    Sleep(1000);
    exit(0);
}
void Lose(){
    glClear(GL_COLOR_BUFFER_BIT);
    selectFont(400, ANSI_CHARSET, "Comic Sans MS");
    glColor3f(0,0,1);
    glRasterPos3f(-50,0,-20);
    drawString("You Lose");
    glutPostRedisplay();
    glutSwapBuffers();
    Sleep(1000);
    exit(0);
}
void FixDistance(){
    Px-=SpeedBaseX*0.027;
    Dis+=SpeedBaseX*0.027;
    if(Px<-20){
        float delta=-20-Px;
        Px=-20;
    }
    if(Dis>40||cnt==AimStr.size()){
        Lose();
    }
}
void Draw_Aim(){
    selectFont(100, ANSI_CHARSET, "Comic Sans MS");
    glColor3f(1,0,0);
    glRasterPos3f(-40,10,-1);
    for(int i=cnt;i<cnt+MaxLength&&AimStr[i]!='\0';i++)
        drawChar(AimStr[i]);
}
void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}
void ReadText(char * File){
    ifstream input(File);
    if(!input.is_open()){
        cout<<"TextFile Open Fail\n";
        return;
    }
    string tmp;
    while(getline(input,tmp)){
        AimStr+=tmp;
    }
    return;
}
void CountDown(){
    string str;
    int tmp=Count;
    while(tmp){
        char ch='0'+tmp%10;
        str+=ch;
        tmp/=10;
    }
    selectFont(400, ANSI_CHARSET, "Comic Sans MS");
    glColor3f(0,0,1);
    glRasterPos3f(0,0,-20);
    drawString(str);
    glutPostRedisplay();
    glutSwapBuffers();
    Sleep(1000);
}
