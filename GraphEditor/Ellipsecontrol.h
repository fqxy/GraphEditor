#ifndef ELLIPSECONTROL_H
#define ELLIPSECONTROL_H

#include "Ellipse.h"
#include "FigureControl.h"
#include <vector>

using namespace std;

class EllipseControl:public FigureControl
{
private:
    enum SetEP{EndPoint,MarkPoint,CenterPoint,HandlePoint}setEP;
    Ellipse *curEllipse; //the pointer to the ellipse that are drawing now
    vector<Ellipse*> exEllipses; //all the ellipse have drawn before
    //void pushForward(Figure *fg); //put current ellipse at the first level

public:
    EllipseControl();
    EllipseControl(std::vector<Figure*> *figures);
    EllipseControl(int width,int height);

    bool setFocus(Figure *fg);//若fg在FigureControl内，则设置聚焦并返回true，否则返回false

    //event, user interaction
    void onMousePressEvent(QMouseEvent *event);
    void onMouseMoveEvent(QMouseEvent *event);
   //void onMousePassiveMoveEvent(QMouseEvent *event);
   //virtual void onKeyPressEvent(QKeyEvent *event);

    void onDraw(); //绘制所有图形
    void onMarkDraw(); //为当前图形添加标记

    //virtual void onCut(const Point &LeftDown,int width,int height);
    void onScale(double r);
    void onDelete();
    void onClear();

};
#endif // ELLIPSECONTROL_H
