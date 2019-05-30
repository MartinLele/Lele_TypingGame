#ifndef _BASE_H_
#define _BASE_H_
#include"Model.h"
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
using namespace std;
class ShaderColor{
public:
    float R,G,B;
    ShaderColor(float a,float b,float c);
    ShaderColor();
    bool operator =(const ShaderColor &X);
};
class Node{
public:
    float x,y;
    Node();
    Node(float a,float b);
    bool operator =(const Node& tmp);
};
class Line{
public:
    float x1,y1,x2,y2;
    float Size;
    ShaderColor Shader;
    Line(float a,float b,float c,float d,float S,ShaderColor& SH);
};
class Triangle{
public:
    vector<Node> vec;
    ShaderColor Shader;
    Triangle(vector<Node> &tmp,ShaderColor& tmp2);
};
class Sphere{
public:
    float R,G,B;
    GLfloat len,cx,cy;
    Sphere(GLfloat x,GLfloat y,GLfloat r,float a,float b,float c);
};
class Poly{
public:
    vector<Node> vec;
    ShaderColor Shader;
    Poly(vector<Node> &tmp,ShaderColor& tmp2);
};
class Rigid{
public:
    Node LD,RU;
    Rigid(Node a,Node b);
    void Draw();
};
ShaderColor::ShaderColor(float a,float b,float c){
    R=a,G=b,B=c;
}
ShaderColor::ShaderColor(){
    R=G=B=0;
}
bool ShaderColor::operator =(const ShaderColor &X) {
    R=X.R;
    G=X.G;
    B=X.B;
}
Node::Node(float a,float b){
    x=a,y=b;
}
Node::Node(){
    x=y=0;
}
bool Node::operator =(const Node& tmp){
    x=tmp.x;
    y=tmp.y;
}
Rigid::Rigid(Node a,Node b){
    LD=a;
    RU=b;
}
void Rigid::Draw(){
    glBegin(GL_LINE_LOOP);
    glColor3f(0.2,1.0,1.0);
    glVertex2f(LD.x,LD.y);
    glVertex2f(LD.x,RU.y);
    glVertex2f(RU.x,RU.y);
    glVertex2f(RU.x,LD.y);
    glEnd();
}
Line::Line(float a,float b,float c,float d,float S,ShaderColor& SH){
    x1=a,y1=b,x2=c,y2=d;
    Size=S,Shader=SH;
}
Sphere::Sphere(GLfloat x,GLfloat y,GLfloat r,float a,float b,float c){
    cx=x;cy=y;len=r;
    R=a;G=b;B=c;
}
Triangle::Triangle(vector<Node> &tmp,ShaderColor& tmp2){
    vec=tmp;
    Shader=tmp2;
}
Poly::Poly(vector<Node> &tmp,ShaderColor& tmp2){
    vec=tmp;
    Shader=tmp2;
}
bool Judge(Rigid a,float Sax,float Say,float Max,float May,Rigid b,float Sbx,float Sby,float Mbx,float Mby){
    float Ua=a.RU.y+May,Da=a.LD.y+May,La=a.LD.x+Max,Ra=a.RU.x+Max;
    float Ub=b.RU.y+Mby,Db=b.LD.y+Mby,Lb=b.LD.x+Mbx,Rb=b.RU.x+Mbx;
//    cout<<a.LD.x<<' '<<b.LD.x<<endl;
//    cout<<Max<<' '<<Mbx<<endl;
//    cout<<Sax<<' '<<Sbx<<endl;
//    cout<<Ua<<' '<<Da<<' '<<La<<' '<<Ra<<endl;
//    cout<<Ub<<' '<<Db<<' '<<Lb<<' '<<Rb<<endl;
    bool tag1=false,tag2=false;
    if(Ra>=Lb&&Ra<=Rb)
        tag1=true;
        if(La>=Lb&&La<=Rb)
        tag1=true;
    if(Ua>=Db&&Ua<=Ub)
        tag2=true;
    if(Da>=Db&&Da<=Ub)
        tag2=true;
    return tag1&&tag2;
}
#endif // _BASE_H_
