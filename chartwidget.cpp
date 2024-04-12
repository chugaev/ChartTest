#include "chartwidget.h"

void ChartWidget::loadData(std::vector<std::pair<double, double>> data) {
    _data = std::move(data);
    //sort(_data.begin(), _data.end());
    reset();
    createMatrix();
    createVertices();
    update();
}

ChartWidget::~ChartWidget() {
    reset();
}

void ChartWidget::setLoading(bool isLoading) {
    _isLoading = isLoading;
    update();
}

void ChartWidget::reset() {
    delete [] _vertices;
    _vertices = nullptr;
    delete [] _matrix;
    _matrix = nullptr;
}

void ChartWidget::createVertices() {
    _vertices = new GLfloat[2 * _data.size()];
    for (size_t i = 0; i < _data.size(); i++) {
        _vertices[2 * i + 0] = _data[i].first;
        _vertices[2 * i + 1] = _data[i].second;
    }
}

void ChartWidget::createMatrix() {
    double yMin = _data.begin()->second;
    double yMax = _data.begin()->second;
    double xMin = _data.begin()->first;
    double xMax = _data.rbegin()->first;

    for (auto [x, y] : _data) {
        yMin = std::min(y, yMin);
        yMax = std::max(y, yMax);
    }

    double xRange = xMax - xMin;
    double yRange = yMax - yMin;
    double glRange = 2.0;
    double scaleX = glRange / xRange;
    double scaleY = glRange / yRange;
    double tranlationX = -xMin * scaleX - glRange / 2.0;
    double tranlationY = -yMin * scaleY - glRange / 2.0;
    _matrix = new GLfloat[16] {
        scaleX, 0.0, 0.0, 0.0,
        0.0, scaleY, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        tranlationX, tranlationY, 0.0, 1.0
    };
}

void ChartWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    _program = std::make_unique<SolidProgram>();
}

void ChartWidget::paintGL() {
    auto start_time = std::chrono::steady_clock::now();

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    if (_isLoading) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(rect(), Qt::white);
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 12));
        painter.drawText(rect(), Qt::AlignCenter, "Loading...");
    } else if (!_data.empty()) {
        createMatrix();
        createVertices();

        glUseProgram(_program->getProgram());

        glUniformMatrix4fv(_program->getMatrixLocation(), 1, false, _matrix);
        glUniform3fv(_program->getColorLocation(), 1, _color);
        glVertexAttribPointer(_program->getPositionLocation(), 2, GL_FLOAT, GL_FALSE, 0, _vertices);
        glEnableVertexAttribArray(_program->getPositionLocation());

        glDrawArrays(GL_LINE_STRIP, 0, _data.size());

        glDisableVertexAttribArray(_program->getPositionLocation());

        reset();

        auto end_time = std::chrono::steady_clock::now();
        auto elapsed_time = end_time - start_time;
        qDebug() << "Drawing time "
                 << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count()
                 << " milliseconds.";
    }
}


void ChartWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}
