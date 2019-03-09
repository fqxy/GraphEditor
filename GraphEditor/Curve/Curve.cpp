#include "Curve.h"
#include <GL/glu.h>
#include <cmath>

Curve::Curve()
{
}

Curve::Curve(const vector<Point> &vertices)
{
	this->vertices = vertices;
	calculatePoints();
	calculateCenter();
	handle.setHandlePoint(center-Point(-1,0), center-Point(1,0), h);
}

Curve::~Curve()
{
}

vector<Point> Curve::getVertices() const
{
	return vertices;
}

Point Curve::getCenter() const
{
	return center;
}

Point Curve::getHandle() const
{
	return handle;
}

void Curve::setVertex(int idx, const Point &p)
{
	if(idx>=vertices.size())
		return;
	vertices[idx] = p;
    clear();            //清除之前的已经画的点
    calculatePoints();  //计算曲线上所有的点
    calculateCenter();  //计算中心点
                        //计算把手点
	handle.setHandlePoint(center-Point(-1,0), center-Point(1,0), h);
}

void Curve::setHandlePointByRef(const Point &ref)
{
    double a = handle.getX()-center.getX(), b = handle.getY()-center.getY();
    double c = ref.getX()-center.getX(), d = ref.getY()-center.getY();
    double angle = 180/3.14159265*acos((a*c+b*d)/sqrt((a*a+b*b)*(c*c+d*d)));
    if(b*c>a*d) //逆时针旋转，角度取负
        angle = -angle;
    handle.rotate(center, angle);
    for(Point &v:vertices)
        v.rotate(center, angle);
    clear();
    calculatePoints();
}

void Curve::draw()
{
    SimpleFigure::draw();
}

void Curve::markDraw()
{
    for(Point p:vertices)       //画出4个控制点
        p.markDraw();
    center.centerMarkDraw();    //画出中心点
    handle.handleDraw(center);  //画出把手
                                //画出曲线的范围
    glLineWidth(1.0f);          //宽度为1
    glEnable(GL_LINE_STIPPLE);  //开启虚线绘制功能
    glLineStipple(1, 0x0f0f);   //虚线
    glColor3f(0.0, 0.0, 1.0);   //蓝色
    glBegin(GL_LINES);
        for(int i=0;i+1<(int)vertices.size();i++)
        {
            glVertex2i(vertices[i].getX(), vertices[i].getY());
            glVertex2i(vertices[i+1].getX(), vertices[i+1].getY());
        }
    glEnd();
	glFlush();
	glDisable(GL_LINE_STIPPLE); //关闭虚线绘制功能	
}

void Curve::clear()
{
    SimpleFigure::clear();
}

int Curve::isFigure()
{
    return 3;
}

void Curve::translate(const Point &offset)
{
    center.translate(offset);   //平移中心点
    handle.translate(offset);   //平移把手点
    for(Point &v:vertices)      //平移4个控制点
		v.translate(offset);
    for(Point *p:points)        //平移曲线上的所有点
		p->translate(offset);
}

void Curve::rotate(double angle)
{
    handle.rotate(center, angle);   //旋转把手点
    for(Point &v:vertices)          //旋转4个控制点
		v.rotate(center, angle);
	clear();
	calculatePoints();
}

void Curve::scale(double s)
{
    for(Point &v:vertices)
        v.scale(center, s, s);
    clear();
    calculatePoints();
}

bool Curve::isOn(const Point &p)
{
    //在曲线上 || 在中心点上 || 在把手点上
	if(isOnPlain(p) || p.distanceTo(center)<=5 || p.distanceTo(handle)<=5)
		return true;
    //在4个控制点上
	for(Point &v:vertices)
		if(p.distanceTo(v)<=5)
			return true;
	return false;
}

bool Curve::isOnPlain(const Point &p)
{
    return SimpleFigure::isOn(p);
}

const int Curve::h = 30; //handle长度
void Curve::calculatePoints()
{
	if(vertices.size()<4)
		return;
	//根据vertices前4个点计算曲线各点
	for(double u=0;u<=1;u+=0.001)
	{
		double a = pow(1-u,3);
		double b = 3*u*pow(1-u,2);
		double c = 3*pow(u,2)*(1-u);
		double d = pow(u,3);
        //每构造1个点，就将其压入points向量中
		points.push_back(new Point(a*vertices[0].getX() + b*vertices[1].getX() + c*vertices[2].getX() + d*vertices[3].getX() + 0.5,
								   a*vertices[0].getY() + b*vertices[1].getY() + c*vertices[2].getY() + d*vertices[3].getY() + 0.5));
	}
}

void Curve::calculateCenter()
{
	//根据vertices计算center
	int minX = vertices[0].getX();
	int maxX = vertices[0].getX();
	int minY = vertices[0].getY();
	int maxY = vertices[0].getY();
	for(Point v:vertices)
	{
		minX = min(minX, v.getX());
		maxX = max(maxX, v.getX());
		minY = min(minY, v.getY());
		maxY = max(maxY, v.getY());
	}
	center.setPoint((minX+maxX)/2, (minY+maxY)/2);
}

