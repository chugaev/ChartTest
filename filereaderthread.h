#pragma once

#include <QFile>
#include <QThread>
#include <QDebug>

#include <vector>

class FileReaderThread : public QThread {
    Q_OBJECT
    QString _filePath;
    std::vector<std::pair<double, double>> _data;
public:
    explicit FileReaderThread(QObject *parent = nullptr)
        : QThread(parent) {}

    void run() override;
    const std::vector<std::pair<double, double>>& getData();
    void setFilePath(const QString& filePath);

private:
    bool parse(QFile& file, std::vector<std::pair<double, double>>& data, QString& errorMessage);
signals:
    void fileRead();
    void error(const QString& message);
};
