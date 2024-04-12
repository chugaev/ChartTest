#include "solidprogram.h"

SolidProgram::SolidProgram() {
    initializeOpenGLFunctions();
    createProgram();
    _positionLocation = glGetAttribLocation(_program, _position);
    Q_ASSERT(_positionLocation != -1);
    _martixLocation = glGetUniformLocation(_program, _matrix);
    Q_ASSERT(_martixLocation != -1);
    _colorLocation = glGetUniformLocation(_program, _color);
    Q_ASSERT(_colorLocation != -1);
}

SolidProgram::~SolidProgram() {
    glDeleteProgram(_program);
}

GLuint SolidProgram::getProgram() {
    return _program;
}

GLint SolidProgram::getPositionLocation() {
    return _positionLocation;
}

GLint SolidProgram::getMatrixLocation() {
    return _martixLocation;
}

GLint SolidProgram::getColorLocation() {
    return _colorLocation;
}

void SolidProgram::createProgram() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
    }

    _program = glCreateProgram();
    glAttachShader(_program, vertexShader);
    glAttachShader(_program, fragmentShader);
    glLinkProgram(_program);

    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_program, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
