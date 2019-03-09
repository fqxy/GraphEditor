#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "Area.h"
#include "Point.h"
#include "SimpleFigure.h"
class Ellipse:public SimpleFigure,public Area
{
private:
    Point begin_p;  //the start vertex point of the bounding rectangle of the ellipse
    Point end_p;  //the end vertex point of the bounding rectangle of the ellipse
    Point center_p;
    int a,b;  //half long axis and half short axis
    vector <Point> markPoints; //marked point, the 4 vertex of bounding rectangle of the ellipse
    //parameters of handle
    Point handle_p;
    const static int h; //handle's length
    //calculatePoints 是否可以作为虚函数，加入到point基类里面去？
    void calculatePoints(); //calculate the points on the perimeter of the ellipse with bresenham method

    void calculateHandle(); //update handle if no rotation
public:
    //constructors
    Ellipse();
    Ellipse(const Point &begin_p,const Point &end_p);
    Ellipse(const Point &center_p,int a,int b);

    Point getCenterPoint() const;  //return the center point of ellipse
    vector<Point> getMarkPoints() const; //return the 4 vertex of the bounding rectangle of the ellipse
    Point getHandlePoint()const; //return the handle point

    void setTwoAxes(int a,int b); //assign new values to 2 axis
    void setEndpoint(const Point &end_p); //assign end point
    void setHandlepoint(const Point &ref_p);


    //basic operation; virtual function based on base class Figure
    void rotate(double angle); //rotate the ellipse with an angle
    void translate(const Point &move_p); //translate along the vector move_p
    void scale(double &r); //scale the ellipse with the given ratio, a=r*a, b=r*b

    void draw(); /*******/
    void markDraw(); //mark the ellipse when it is selected
    void clear(); //clear the ellipse

    bool isOn(const Point &mp); //judge whether mp is on
    bool isOnPlain(const Point &mp); //judge whether mp is on the plain


};
#endif // ELLIPSE_H
