#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QDebug>

class SolidProgram : protected QOpenGLFunctions {
    const char* vertexShaderSource = R"(
        #version 330 core
        attribute highp vec4 position;
        uniform highp mat4 matrix;
        uniform vec3 color;

        void main() {
            gl_Position = matrix * vec4(position.x, position.y, 0.0, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        uniform vec3 color;
        out vec4 FragColor;

        void main() {
            FragColor = vec4(color, 1.0f);
        }
    )";

    GLuint _program = 0;
    GLint _positionLocation = 0;
    GLint _martixLocation = 0;
    GLint _colorLocation = 0;

    const char* _position = "position";
    const char* _matrix = "matrix";
    const char* _color = "color";

    void createProgram();
public:
    SolidProgram();
    ~SolidProgram();
    GLuint getProgram();
    GLint getPositionLocation();
    GLint getMatrixLocation();
    GLint getColorLocation();
};
