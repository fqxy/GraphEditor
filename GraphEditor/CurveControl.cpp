#include "CurveControl.h"

CurveControl::CurveControl()
{
	curIdx = -1;
}

CurveControl::CurveControl(std::vector<Figure *> *figures):FigureControl(figures)
{
	curIdx = -1;
}

CurveControl::CurveControl(int width, int height):FigureControl(width,height)
{
	curIdx = -1;
}

CurveControl::~CurveControl()
{
	for(Curve *curve:curves)
		delete curve;
}

bool CurveControl::setFocus(Figure *fg)
{
	for(int i=0;i<curves.size();i++)
		if(curves[i]==fg)
		{
			curIdx = i;
			return true;
		}
	return false;
}

void CurveControl::onMousePressEvent(QMouseEvent *event)
{
    //判断是否按下键
	if(event->button()==Qt::LeftButton)
	{
        //接下来有3个情况
        //1.鼠标点中的点之前还有1个Curve还没画完，所以该点要承接上一个点
        //2.鼠标点中的点在已经绘制完成的其中1个曲线上
        //3.鼠标点中的点作为新曲线的第一个点

        //curPoint为鼠标点中的当前点
		Point curPoint(event->x(), height-event->y());

        //第1种情况
        if(!curVtxs.empty())
		{
			if(curVtxs.size()==3) //最后一个点
			{
                //压入最后1个点
				curVtxs.push_back(curPoint);
                //压入当前绘制的曲线
				curves.push_back(new Curve(curVtxs));
				curIdx = curves.size()-1;
                //让allFigures压入当前曲线
				allFigures->push_back(curves.back());
				curVtxs.clear();
				return;
			}
			else if(curVtxs.size()<3)
			{
                //压入前3个点
				curVtxs.push_back(curPoint);
				return;
			}
		}

        //第2种情况
		else if(curVtxs.empty() && !curves.empty()) //选中控制点或曲线本身：编辑curves[curIdx]
		{
            //选中4个控制点中的一个
			vector<Point> vtxs = curves[curIdx]->getVertices();
            for(int i=0;i<vtxs.size();i++)
            {
				if(curPoint.distanceTo(vtxs[i])<=5)
				{
					setCV = i;
                    pushForward(curves[curIdx]);//将当前曲线置为最顶层
					return;
                }
            }

            //选中中心点
			if(curPoint.distanceTo(curves[curIdx]->getCenter())<=5)
			{
				setCV = -2;
                pushForward(curves[curIdx]);    //将当前曲线置为最顶层
				return;
			}

            //选中把手点
			else if(curPoint.distanceTo(curves[curIdx]->getHandle())<=5)
			{
				setCV = -3;
                pushForward(curves[curIdx]);    //将当前曲线置为最顶层
				return;
			}

            //在
			else if(curves[curIdx]->isOn(curPoint))
			{
                pushForward(curves[curIdx]);    //将当前曲线置为最顶层
				return;
			}
		}

        //第3种情况
		curVtxs.push_back(curPoint);
		setCV = -1;
	}
}

void CurveControl::onMouseMoveEvent(QMouseEvent *event)
{
	if(!curves.empty())
	{
        //curPoint为鼠标点中的当前点
		Point curPoint(event->x(), height-event->y());

        //根据鼠标点中的点(改变setCV)决定下一步操作
        if(setCV>=0)        //改变形状
			curves[curIdx]->setVertex(setCV, curPoint);
        else if(setCV==-2)  //平移
			curves[curIdx]->translate(curPoint - curves[curIdx]->getCenter());
        else if(setCV==-3)  //旋转
			curves[curIdx]->setHandlePointByRef(curPoint);
	}
}

//void CurveControl::onDraw()
//{
//    for(Point &v:curVtxs)       //对象是点，画出4个控制点
//		v.draw();
//    for(Curve *curve:curves)    //对象是曲线，画出曲线
//		curve->draw();
//}


//在glwidget.cpp中，每当鼠标事件完成后
//都会调用updateGL()函数，从而调动paitGL()函数
//从而调动on
void CurveControl::onMarkDraw()
{
    if(!curVtxs.empty())        //4个控制点还未画完
        for(Point &p:curVtxs)   //对象是点，画出标记点
			p.markDraw();
                                //4个控制点画完了
    else if(curIdx>=0)          //对象是曲线，画出标记点
		curves[curIdx]->markDraw();
}



void CurveControl::deleteCurve(int idx)
{
	if(idx<0)
		return;
	curves.erase(curves.begin()+idx);
    for(vector<Figure*>::iterator it=allFigures->begin();
                                    it!=allFigures->end();
                                                    it++)
		if(*it==curves[idx])
		{
			allFigures->erase(it);
			break;
		}
	delete curves[idx];
}

