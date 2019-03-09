#ifndef CIRCLECONTROL_H
#define CIRCLECONTROL_H

#include "Circle.h"
#include "FigureControl.h"
#include <vector>
using namespace std;

class CircleControl:public FigureControl
{
public:
    CircleControl();
    CircleControl(std::vector<Figure*> *figures); //多个图
    CircleControl(int width, int height);
    ~CircleControl();

    bool setFocus(Figure *fg); //设置为当前图

    void onMousePressEvent(QMouseEvent *event); //鼠标press
    void onMouseMoveEvent(QMouseEvent *event); //鼠标press+move

    void onDraw();
    void onMarkDraw();
    void onFill();

    void onScale(double s); //缩放
    void onDelete(); //删除当前圆
    void onClear(); //删除所有圆

private:
    int setCV;  //当前正在更改的控制点索引；
                //-1表示选中markpoint,
                //-2表示选中center,
                //-3表示选中了handle点
                //-4表示创建新的圆心
    Circle *curCircle; //当前聚焦的圆
    vector<Circle*> circles;
};

#endif // !CIRCLECONTROL_H
