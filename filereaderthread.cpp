#include "filereaderthread.h"

void FileReaderThread::run() {
    QFile file(_filePath);
    _data.clear();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "error file opening";
        emit error("error file opening");
        return;
    }
    QString errorMessage;
    bool ok = parse(file, _data, errorMessage);
    file.close();
    if (!ok) {
        if (!isInterruptionRequested())
            emit error(errorMessage);
        return;
    }
    emit fileRead();
}

const std::vector<std::pair<double, double>>& FileReaderThread::getData() {
    return _data;
}

void FileReaderThread::setFilePath(const QString& filePath) {
    _filePath = filePath;
}

bool FileReaderThread::parse(QFile& file, std::vector<std::pair<double, double>>& data, QString& errorMessage) {
    auto start_time = std::chrono::steady_clock::now();
    double time, value;

    int lineCounter = 0;
    while (!file.atEnd()) {
        QString line = file.readLine();
        lineCounter++;
        line = line.simplified();
        if (line.size() == 0)
            continue;
        if (line[0] == '#')
            continue;
        QStringList lst = line.split(" ");
        if (lst.size() != 2) {
            errorMessage = "wrong format, line " + QString::number(lineCounter);
            return false;
        } else {
            bool ok;
            time = lst.at(0).toDouble(&ok);
            value = lst.at(1).toDouble(&ok);
            if (!ok) {
                errorMessage = "wrong format, line " + QString::number(lineCounter);
                return false;
            }
            data.emplace_back(time, value);
        }
        if (isInterruptionRequested()) {
            qDebug() << "interruption request";
            return false;
        }
    }
    auto end_time = std::chrono::steady_clock::now();
    auto elapsed_time = end_time - start_time;
    qDebug() << "Reading time "
             << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count()
             << " milliseconds.";
    return true;
}
