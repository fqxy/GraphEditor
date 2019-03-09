#-------------------------------------------------
#
# Project created by QtCreator 2018-05-16T00:12:20
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Curve
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp \
        glwidget.cpp \
        Point.cpp \
        FigureControl.cpp \
        SimpleFigure.cpp \
        Line/Line.cpp \
        Line/LineControl.cpp \
        Circle/Circle.cpp \
        Circle/CircleControl.cpp \
        Ellipse/MyEllipse.cpp \
        Ellipse/MyEllipseControl.cpp \
        Curve/Curve.cpp \
        Curve/CurveControl.cpp \
        Area/Area.cpp \
        mainwindow.cpp \
    dialog.cpp \
    pen/mypen.cpp \
    pen/mypencontrol.cpp \
    Rectangular/rectangular.cpp \
    Rectangular/rectangularcontrol.cpp

HEADERS  += glwidget.h \
        Figure.h \
        Point.h \
        Figure.h \
        FigureControl.h \
        SimpleFigure.h \
        Line/Line.h \
        Line/LineControl.h \
        Circle/Circle.h \
        Circle/CircleControl.h \
        Ellipse/MyEllipse.h \
        Ellipse/MyEllipseControl.h \
        Curve/Curve.h \
        Curve/CurveControl.h \
        Area/Area.h \
        mainwindow.h \
    dialog.h \
    pen/mypen.h \
    pen/mypencontrol.h \
    Rectangular/rectangular.h \
    Rectangular/rectangularcontrol.h


FORMS    += \
    mainwindow.ui \
    dialog.ui

QMAKE_CXXFLAGS += -Wno-sign-compare -Wno-unused-parameter
LIBS += -lopengl32 \
        -lglu32 \
        -lglut \
