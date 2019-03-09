#ifndef MYFIGURE_H
#define MYFIGURE_H

#include "Point.h"
#include <vector>

class myFigure
{
public:
    //构造
    virtual ~myFigure();
    //绘制
    virtual void draw();
    virtual void markdraw() = 0;
    virtual void clear();
    //变换
    virtual void translate(const Point& offset) = 0;
    virtual void rotate(double angle) = 0;
    //判断
    virtual bool isOn(const Point& p);
    virtual bool isOnPlain(const Point& p) = 0;
    //获取
    const std::vector<Point*>& getPoint() const;

protected:
    std::vector<Point*> points;
};
#endif // MYFIGURE_H
