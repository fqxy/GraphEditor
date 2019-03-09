#include "Ellipse.h"
#include <cmath>
#include <cassert> //for

//default contributor
Ellipse::Ellipse()
{
    a = b= -1; //default value
}

//input begin point and end point to create a ellipse
Ellipse::Ellipse(const Point &begin_p,const Point &end_p)
{
    this->begin_p = begin_p;
    this->end_p = end_p;
    a = abs((end_p.getX() - begin_p.getX())/2); //half long axis
    b = abs((end_p.getY() - begin_p.getY())/2); //half short axis
    center_p.setPoint((begin_p.getX() + end_p.getX())/2,(begin_p.getY() + end_p.getY())/2); //get the center point according begin and end points
    calculatePoints(); //calculate the points on the perimeter of the ellipse with bresenham method
    calculateHandle(); //update handle if no rotation

}

//input center point and a,b to create a ellipse
Ellipse::Ellipse(const Point &center_p,int a,int b)
{
    this->center_p = center_p;
    this->a = a;
    this->b = b;
    calculatePoints(); //calculate the points on the perimeter of the ellipse with bresenham method
    calculateHandle(); //update handle if no rotation
}

Point Ellipse::getCenterPoint()const
{
    return center_p;
}

vector<Point> Ellipse::getMarkPoints()const
{
    return markPoints;
}

Point Ellipse::getHandlePoint()const
{
    return handle_p;
}

//reset the long axis and short axis based on original center point
void Ellipse::setTwoAxes(int xa,int xb)
{
    clear(); //why?
    this->begin_p.setPoint(center_p.getX()-xa,center_p.getY()-xb); //把begin_p设置为外接矩形左下角顶点
    this->end_p.setPoint(center_p.getX()+xa,center_p.getY()+xb); //把end_p设为外接矩形右上角顶点
    this->a = xa;
    this->b = xb;
    calculatePoints(); //redraw the ellipse
    calculateHandle();
}

//set an arbitary end point and get a new ellipse
void Ellipse::setEndpoint(const Point &c_end_p)
{
    clear();
    this->end_p = c_end_p;
    a = abs(end_p.getX() - begin_p.getX())/2;
    b = abs(end_p.getY() - begin_p.getY())/2;
    center_p.setPoint((begin_p.getX()+end_p.getX())/2,(begin_p.getY()+end_p.getY())/2);  //get the center point
    calculatePoints(); //redraw the ellipse
    calculateHandle();
}

//
void Ellipse::setHandlepoint(const Point &ref_p)
{
    if(ref_p.getX()==center_p.getX()||ref_p.getY()==center_p.getY()) //if vector(center_p,ref_p) is horizontal or vertical
        setTwoAxes(b,a); //rotate the ellipse with 90 degree
    //handle_p.rotateToParallel(center_p,ref_p,h);
}

//rotate the ellipse with angle and angle must be degree(not radian)
void Ellipse::rotate(double angle)
{
    int temp = abs((int)angle)%180; //the temp is [0,180)
    if(temp>=0 && temp<90)  //if temp is [0,90),not rotate
        return;
    else   //if temp is [90,180),rotate 90 degrees
        setTwoAxes(b,a); //exchange long axis and short axis
}

void Ellipse::translate(const Point &move_p) //translate the ellipse along the vector: (0,0)-> move_p
{
    clear();
    center_p.translate(move_p);
    begin_p.translate(move_p);
    end_p.translate(move_p);
    calculatePoints(); //redraw the ellipse
    calculateHandle();

}

//scale the ellipse with the given ratio r
void Ellipse::scale(double &r)
{
    setTwoAxes(a*r,b*r);
}

void Ellipse::draw()
{
    SimpleFigure::draw(); //轮廓
    Area::fillColor(); //填充
}

//being selected and to mark it
void Ellipse::markDraw()
{
    markPoints.erase(markPoints.begin(),markPoints.end()); //??
    markPoints.push_back(Point(center_p.getX()-a,center_p.getY()-b)); //外接矩形左下角顶点
    markPoints.push_back(Point(center_p.getX()-a,center_p.getY()+b)); //外接矩形左上角顶点
    markPoints.push_back(Point(center_p.getX()+a,center_p.getY()+b)); //外接矩形右上角顶点
    markPoints.push_back(Point(center_p.getX()+a,center_p.getY()-b)); //外接矩形右下角顶点
    Area::drawRect(markPoints[0],markPoints[1],markPoints[2],markPoints[3]); //draw the bounding rectangle with dotted line

    for(Point p:markPoints)
        p.markDraw();  //make 4 vertex of the bounding rectangle are selected and shown

    center_p.centerMarkDraw();  //mark the center point
    handle_p.handleDraw(center_p); //draw the line between center_p and handle_p
}

//calculate the points on the ellipse border
//bresenham method
void Ellipse::calculatePoints()
{
    if(a<0||b<0)
        return;  //is not a legal ellipse
    long sqa = a*a;
    long sqb = b*b;
    //for the 1st area and k>-1
    int x = 0,y = b; //initial point is (0,b)
    long D = 2*sqb - 2*b*sqa + sqa; //?
    while(sqb*x < sqa*y)
    {
        if(D < 0)
        {
            D += 2*sqb*(2*x+3);
        }
        else
        {
            D += 2*sqb*(2*x+3) - 4*sqa*(y-1);
            y--;
        }
        x++; //x position of next pixel
        //draw the known points in four quadrants
        //can be written a base function
        points.push_back(new Point(center_p.getX() + x,center_p.getY() + y)); //store all the points of the perimeter
        points.push_back(new Point(center_p.getX() + x,center_p.getY() - y));
        points.push_back(new Point(center_p.getX() - x,center_p.getY() - y));
        points.push_back(new Point(center_p.getX() - x,center_p.getY() + y));
    }

    D = sqb*(x*x + x) + sqa*(y*y - y) - sqa*sqb;
    while(y>=0)
    {
        points.push_back(new Point(center_p.getX() + x,center_p.getY() + y)); //store all the points of the perimeter
        points.push_back(new Point(center_p.getX() + x,center_p.getY() - y));
        points.push_back(new Point(center_p.getX() - x,center_p.getY() - y));
        points.push_back(new Point(center_p.getX() - x,center_p.getY() + y));
        y--; //y position of next pixel
        if(D < 0)
        {
            x++;
            D += 2*sqb*x + 2*sqb - sqa -2*sqa*y; //?

        }
        else
            D = D - 2*sqa*y -sqa;
    }
}

//calculate the handle point
const int Ellipse::h = 45; //initial handle's length
void Ellipse::calculateHandle()
{
    if(a<=0 || b<=0) //no ellipse created
    {
        handle_p.setPoint(center_p.getX(),center_p.getY());
        return;
    }
    double temp = sqrt(a*a+b*b);
    handle_p.setPoint(center_p.getX()+int(h*a/temp+0.2),center_p.getY()+int(h*b/temp+0.2));

}

//judge whether the point is on the ellipse(include center_P/handle_p/mark point/on the perimeter
bool Ellipse::isOn(const Point &p)
{
    if(p.distanceTo(center_p)<=5||p.distanceTo(handle_p)<=5||isOnPlain(p) ) //if p is on the perimeter or is near center_p or near handle_p
        return true;
    for(Point mp:markPoints) //这种for到底是什么用法？
    {
        if(mp.distanceTo(p)<=5)
            return true;
    }
    return false;

}

//judge whether the point is on the area of the ellipse
bool Ellipse::isOnPlain(const Point &mp)
{
    if(SimpleFigure::isOn(mp)) //on the perimeter
        return true;
    if(!fillPoints.empty()) //fillPoints is protect member,Point * vector
    {
        for(Point *pt:fillPoints)
        {
            if(*pt==mp)
                return true;
        }
    }
    return false;
}








