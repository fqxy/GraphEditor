#include "Circle.h"
#include <GL/glu.h>
#include <algorithm>

Circle::Circle()
{
}

Circle::Circle(Point center, int radius)
{
    this->center = center;
    this->radius = radius;
    calculatePoints(); //计算轮廓点
    calculateHandle(); //计算handle点
}

Circle::~Circle()
{
    for(Point *p : fillPoints)
        delete p;
    for(Point *p : points)
        delete p;
}

Point Circle::getCenter() const
{
    return center;
}

vector<Point> Circle::getMarkPoints() const
{
    return markPoints;
}

Point Circle::getHandlePoint() const
{
    return handle;
}

void Circle::setRadius(int radius)  //set radius 后重新计算轮廓点和handle点
{
    if( fillPoints.empty() )
    {
        clear();
        this->radius = radius;
        calculatePoints();
        calculateHandle();
    }
    else
    {
        clear();
        this->radius = radius;
        calculatePoints();
        calculateHandle();
        fillColor();
    }
}

const int Circle::h = 30; //初始化handle长度
void Circle::setHandlePointByRef(const Point &ref)
{
    if(center==ref)
        return;
    //向量base->ref
    double c = ref.getX() - center.getX();
    double d = ref.getY() - center.getY();
    double rRef = center.distanceTo(ref); //center-ref向量的长度
    int x = int(center.getX()+h*c/rRef+0.5);
    int y = int(center.getY()+h*d/rRef+0.5);
    handle.setPoint(x,y);
}

void Circle::translate(const Point &offset)
{
    center.translate(offset);
    for(Point *p:points)
        p->translate(offset);
    if( !fillPoints.empty() )
    {
        for(Point *p:fillPoints)
            p->translate(offset);
    }
    calculateHandle();
}

void Circle::rotate(double angle)
{
}

void Circle::scale(double s)
{
    setRadius(int(radius*s+0.5));
    calculateHandle();
}

bool Circle::isOn(const Point &p) //在圆上，标记点，handle点上都可
{
    if(isOnPlain(p) || p.distanceTo(center)<=5 || p.distanceTo(handle)<=5) //圆内，圆心，handle点
        return true;
    for(Point mp:markPoints) //markpoint
        if(mp.distanceTo(p)<=5)
            return true;
    return false;
}

bool Circle::isOnPlain(const Point &p)
{
    if(SimpleFigure::isOn(p)) //在轮廓上
        return true;
    if(!fillPoints.empty())  //如果被填充，在圆内即可
        return center.distanceTo(p)<=radius;
    return false;
}

void Circle::draw()
{
    SimpleFigure::draw(); //轮廓
    Area::fillColor(); //填充
}

void Circle::clear()
{
    SimpleFigure::clear(); //轮廓
    Area::clearColor(); //填充
}

void Circle::markDraw()
{
    markPoints.erase(markPoints.begin(), markPoints.end());
    markPoints.push_back(Point(center.getX()-radius, center.getY()-radius));
    markPoints.push_back(Point(center.getX()-radius, center.getY()+radius));
    markPoints.push_back(Point(center.getX()+radius, center.getY()+radius));
    markPoints.push_back(Point(center.getX()+radius, center.getY()-radius));
    Area::drawRect(markPoints[0], markPoints[1], markPoints[2], markPoints[3]);
    for(Point p:markPoints)
        p.markDraw();
    center.centerMarkDraw();
    handle.handleDraw(center);
}

void Circle::fillColor() //生成填充点
{
    for(Point *p : points) //遍历轮廓点
    {
        int x = p->getX();
        int y = p->getY();
        while( x < this->center.getX() ) //生成与轮廓点一条线上的圆内的点
        {
            fillPoints.push_back(new Point(x++, y));
            fillPoints.push_back(new Point(2*center.getX()-x, y));
        }
    }
}

void Circle::calculatePoints() //生成轮廓点
{
    int x ,y,d,r;
    x = 0;
    r = this->radius;
    if(r<=0)
        return;
    y = r;
    d = 3 - 2 * r;
    //八分之一
    points.push_back(new Point(center.getX(), center.getY() + radius)); //最上
    points.push_back(new Point(center.getX() + radius, center.getY())); //最右
    points.push_back(new Point(center.getX(), center.getY() - radius)); //最下
    points.push_back(new Point(center.getX() - radius, center.getY())); //最左
    while(x<y)
    {
        if(d<0) //取P1，d为下一个点的判别式
        {
            d = d + 4 *x +6;
        }
        else //取P2，d为下一个点的判别式
        {
            d = d + 4 *(x-y)+10;
            y--; //选P2后--
        }
        x++; //每次选了下一个点都要x++
        points.push_back(new Point(center.getX()+x, center.getY() + y));
        points.push_back(new Point(center.getX()+y, center.getY() +x));
        points.push_back(new Point(center.getX()+y, center.getY() -x));
        points.push_back(new Point(center.getX()+x, center.getY() - y));
        points.push_back(new Point( center.getX()-x, center.getY()-y));
        points.push_back(new Point( center.getX()-y, center.getY()-x));
        points.push_back(new Point( center.getX()-y, center.getY()+x));
        points.push_back(new Point( center.getX()-x, center.getY()+y));
    }
}

void Circle::calculateHandle()
{
    handle.setHandlePoint(center-Point(radius,0), center+Point(radius,0), h);
}
