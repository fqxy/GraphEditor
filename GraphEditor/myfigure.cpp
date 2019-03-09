#include "myfigure.h"
#include <GL/glu.h>

using namespace std;

myFigure::~myFigure()
{
    for (Point *p : points)
        delete p;
}

void myFigure::draw()
{
    glPointSize(2.0);
    glColor3f(0, 0, 1);
    glBegin(GL_POINTS);
    for (auto it : points)
        it->draw();
    glEnd();
    glFlush();
}

void myFigure::clear()
{
    for (auto it : points)
        delete it;
    points.erase(points.begin(), points.end());
}

bool myFigure::isOn(const Point &p)
{
    for (Point* pt : points)
        if (pt->distanceTo(p) < 3)
            return true;
    return false;
}

const vector<Point*>& myFigure::getPoint() const
{
    return points;
}
