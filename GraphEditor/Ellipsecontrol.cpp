#include "EllipseControl.h"
#include "Ellipse.h"
#include <GL/glu.h>

//default contributor
EllipseControl::EllipseControl()
{
    curEllipse = NULL;
}

//contributor
EllipseControl::EllipseControl(std::vector<Figure *> *figures):FigureControl(figures)
{
    curEllipse = NULL;
}

//contributor
EllipseControl::EllipseControl(int width,int height):FigureControl(width,height)
{
    curEllipse = NULL;
}


//judge whether fg has been drawn
bool EllipseControl::setFocus(Figure *fg)
{
    for(Ellipse* ellipse1 : exEllipses)
    {
        if(ellipse1==fg) //fg has been drawn
        {
            curEllipse = ellipse1;
            return true;
        }
    }
    return false; //not find
}

void EllipseControl::onMousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton) //when click with left key of mouse
    {
        if(curEllipse!=NULL)
        {
            Point curPoint(event->x(),height-event->y()); //curpoint is the click point
            for(Point p:curEllipse->getMarkPoints()) //search
            {
                if(p.distanceTo(curPoint)<=5)
                {
                    setEP = MarkPoint;
                    pushForward(curEllipse);
                    return;
                }
            }
            int flag = 0;
            if(curPoint.distanceTo(curEllipse->getCenterPoint())<=5)
            {
                setEP = CenterPoint;
                flag = 1;
            }
            else if(curPoint.distanceTo(curEllipse->getHandlePoint())<=5)
            {
                setEP = HandlePoint;
                flag = 1;
            }
            else if(curEllipse->isOn(curPoint))
            {
                flag = 1;
            }

            if(flag==1) //curpoint is on
            {
                pushForward(curEllipse);
                return;
            }
        }
        curEllipse = new Ellipse(Point(event->x(),height-event->y()),Point(event->x(),height-event->y()));  //create a new ellipse
        exEllipses.push_back(curEllipse);  //store the new ellipse in exEllipses
        allFigures->push_back(curEllipse); //store the new ellipse in allFigures
        setEP = EndPoint;
    }
}

void EllipseControl::onMouseMoveEvent(QMouseEvent *event)
{
    if(curEllipse==NULL)
        return;
    Point center_p = curEllipse->getCenterPoint();
    switch(setEP)
    {
        case MarkPoint: curEllipse->setTwoAxes(abs(event->x() - center_p.getX()),abs(event->y() - center_p.getY()));break;
        case CenterPoint: curEllipse->translate(Point(event->x(),height - event->y()) - center_p); break;
        case HandlePoint: curEllipse->setHandlepoint(Point(event->x(),height-event->y())); break;
        case EndPoint: curEllipse->setEndpoint(Point(event->x(),height-event->y()));break;
        default: ;
    }
}


void EllipseControl::onDraw()
{
    for(Ellipse *ellipse : exEllipses)
        ellipse->draw();
}

//mark current ellipse
void EllipseControl::onMarkDraw()
{
    if(curEllipse!=NULL)
        curEllipse->markDraw();
}

//zoom in the current ellipse with ratio r
void EllipseControl::onScale(double r)
{
    if(curEllipse!=NULL)
        curEllipse->scale(r);
}

//delete the selected ellipse
void EllipseControl::onDelete()
{
    if(curEllipse==NULL)
        return;
    for(vector<Ellipse *>::iterator t = exEllipses.begin();t!=exEllipses.end();t++) //search all the ellipses and delete it in exEllipses
    {
        if(curEllipse == *t)
        {
            exEllipses.erase(t);
            break;
        }
    }
    for(vector<Figure *>::iterator t = allFigures->begin();t!=allFigures->end();t++)  //search all the Figure and delete it in allFigures
    {
        if(curEllipse == *t)
        {
            allFigures->erase(t);
            break;
        }

    }
    delete curEllipse;
    curEllipse = NULL;
}

void EllipseControl::onClear()
{
    for(Ellipse *ellipse: exEllipses)
    {
        for(vector<Figure *>::iterator t = allFigures.begin();t!=allFigures.end();t++)
        {
            if(ellipse == *t)
                t = allFigures->erase(t);
            else
                t++; //?
        }
        delete ellipse;
    }
    exEllipses.clear();//?
    curEllipse = NULL;
}

