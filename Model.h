#ifndef _MODEL_H_
#define _MODEL_H_
#include<bits/stdc++.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include"Base.h"
#define PI 3.1415926535
using namespace std;
class Model2d{
private:
    vector<Triangle> Tri;
    vector<Sphere> Spheres;
    vector<Poly> Polygons;
    vector<Line> Lines;
    vector<Rigid> Rigids;
    bool AbleVisible;
    void Clear();
    void Draw_Tri(Triangle& x);
    void Draw_Sph(Sphere& x);
    void Draw_Poly(Poly& x);
    void Draw_Line(Poly& x);
    void Read_Tri_From_File(ifstream& input);
    void Read_Sphere_From_File(ifstream& input);
    void Read_Poly_From_File(ifstream& input);
    void Read_Line_From_File(ifstream& input);
public:
    float Scale_X,Scale_Y;
    float Mov_X,Mov_Y;
    float Theta;
    void Read(char * Filename,float mx,float my,float sx,float sy);
    void Draw();
    void AddTri(vector<Node>& vec,ShaderColor SH);
    void AddSphere(GLfloat x,GLfloat y,GLfloat len,float R,float G,float B);
    void AddPoly(vector<Node>& vec,ShaderColor SH);
    void AddLine(float a,float b,float c,float d,float Size,ShaderColor SH);
    void AddMov(float a,float b);
    void AddScale(float a,float b);
    void AddRotated(float x);
    void SetRotated(float x);
    void SetScale(float a,float b);
    void SetMov(float a,float b);
    void AddRigid(float x,float y);
    void SetRigidVisible(bool tag);
    bool Coll(Model2d& x);
};
void Model2d::Clear(){
    Tri.clear();
    Spheres.clear();
    Polygons.clear();
    Rigids.clear();
    AbleVisible=false;
}
void Model2d::Draw_Tri(Triangle& x){
    vector<Node>& vec=x.vec;
    ShaderColor& SC=x.Shader;
    glColor3f(SC.R,SC.G,SC.B);
    glBegin(GL_TRIANGLES);
    for(int i=0;i<3;i++){
        glVertex2f(vec[i].x,vec[i].y);
    }
    glEnd();
}
void Model2d::Model2d::Draw_Sph(Sphere & x){
    glColor3f(x.R,x.G,x.B);
    glBegin(GL_POLYGON);
    int n=120;
    for(int i=0;i<n;i++)
    {
        float R=x.len;
        float a=R*cos(2.0*PI*i/n)+x.cx;
        float b=R*sin(2.0*PI*i/n)+x.cy;
        glVertex2f(a,b);   //¶¨Òå¶¥µã
    }
    glEnd();
}
void Model2d::Draw_Poly(Poly& x){
    vector<Node>& vec=x.vec;
    ShaderColor& SC=x.Shader;
    glColor3f(SC.R,SC.G,SC.B);
    glBegin(GL_POLYGON);
    for(int i=0;i<vec.size();i++){
        glVertex2f(vec[i].x,vec[i].y);
    }
    glEnd();
}
void Model2d::Read_Tri_From_File(ifstream& input){
    float a,b,c;
    input>>a>>b>>c;
    ShaderColor SH(a,b,c);
    vector<Node> vec;
    for(int i=0;i<3;i++){
        input>>a>>b;
        vec.push_back(Node(a,b));
    }
    AddTri(vec,SH);
}
void Model2d::Read_Sphere_From_File(ifstream& input){
    float x,y,r,R,G,B;
    input>>R>>G>>B>>x>>y>>r;
    AddSphere(x,y,r,R,G,B);
}
void Model2d::Read_Poly_From_File(ifstream& input){
    float a,b,c;
    input>>a>>b>>c;
    vector<Node> vec;
    ShaderColor SH(a,b,c);
    int n;
    input>>n;
    for(int i=0;i<n;i++){
        input>>a>>b;
        vec.push_back(Node(a,b));
//        cout<<a<<' '<<b<<endl;
    }
    AddPoly(vec,SH);
}
void Model2d::Read_Line_From_File(ifstream& input){
    float a,b,c,d,e;
    input>>a>>b>>c;
    ShaderColor SH(a,b,c);
    input>>a>>b>>c>>d>>e;
    AddLine(a,b,c,d,e,SH);
}
void Model2d::Read(char * Filename,float mx,float my,float sx,float sy){
    Clear();
    ifstream input;
    input.open(Filename);
    if(!input){
        cout<<Filename<<" is Open_Fail"<<endl;
    }
    else{
        SetMov(mx,my);
        SetScale(sx,sy);
        Theta=0;
        int type;
        while(input>>type){
            switch(type){
                case 1:Read_Tri_From_File(input);break;
                case 2:Read_Sphere_From_File(input);break;
                case 3:Read_Poly_From_File(input);break;
                case 4:Read_Line_From_File(input);break;
            }
        }
        input.close();
    }
}
void Model2d::Draw(){
    int len=Tri.size();
    glPushMatrix();
    glScalef(Scale_X,Scale_Y,0.0f);
    glTranslatef(Mov_X,Mov_Y,0.0f);
    glRotatef(Theta,0,0,1);
    for(int i=0;i<len;i++){
        Draw_Tri(Tri[i]);
    }
    for(int i=0;i<Spheres.size();i++){
        Draw_Sph(Spheres[i]);
    }
    for(int i=0;i<Polygons.size();i++){
        Draw_Poly(Polygons[i]);
    }
    if(AbleVisible){
        for(int i=0;i<Rigids.size();i++)
            Rigids[i].Draw();
    }
    glPopMatrix();
}
void Model2d::AddTri(vector<Node>& vec,ShaderColor SH){
    Tri.push_back(Triangle(vec,SH));
}
void Model2d::AddSphere(GLfloat x,GLfloat y,GLfloat len,float R,float G,float B){
    Spheres.push_back(Sphere(x,y,len,R,G,B));
}
void Model2d::AddPoly(vector<Node>& vec,ShaderColor SH){
    Polygons.push_back(Poly(vec,SH));
}
void Model2d::AddLine(float a,float b,float c,float d,float Size,ShaderColor SH){
     Lines.push_back(Line(a,b,c,d,Size,SH));
}

void Model2d::SetScale(float a,float b){
    Scale_X=a;
    Scale_Y=b;
   // cout<<a<<' '<<b<<' '<<c<<endl;
}
void Model2d::SetMov(float a,float b){
    Mov_X=a;
    Mov_Y=b;
   // cout<<a<<' '<<b<<' '<<c<<endl;
}
void Model2d::SetRotated(float x){
    Theta=x;
}
void Model2d::AddRotated(float x){
    Theta+=x;
}
void Model2d::AddMov(float a,float b){
    Mov_X+=a;
    Mov_Y+=b;
}
void Model2d::AddScale(float a,float b){
    Scale_X+=a;
    Scale_Y+=b;
}
void Model2d::AddRigid(float x,float y){
    Rigids.push_back(Rigid(Node(-x/2,-y/2),Node(x/2,y/2)));
}
void Model2d::SetRigidVisible(bool tag){
    AbleVisible=tag;
}
bool Model2d::Coll(Model2d & x){
    for(int i=0;i<Rigids.size();i++){
        for(int j=0;j<x.Rigids.size();j++){
            Rigid & a=Rigids[i];
            Rigid & b=x.Rigids[j];
            if(Judge(a,Scale_X,Scale_Y,Mov_X,Mov_Y, b,x.Scale_X,x.Scale_Y,x.Mov_X,x.Mov_Y)){
                return true;
            }
        }
    }
    return false;
}
#endif // _MODEL_H_
