#pragma once

#include "solidprogram.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QDebug>
#include <QPainter>

#include <vector>
#include <thread>
#include <iostream>
#include <chrono>


class ChartWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    std::vector<std::pair<double, double>> _data;
    std::unique_ptr<SolidProgram> _program;
    GLfloat* _matrix = nullptr;
    GLfloat* _vertices = nullptr;
    GLfloat _color[3] { 1.0, 0.0, 0.0 };
    bool _isLoading = false;

public:
    ChartWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) { }
    void loadData(std::vector<std::pair<double, double>> data);
    void setLoading(bool isLoading);
    ~ChartWidget() override;

private:
    void createVertices();
    void createMatrix();
    void reset();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};
