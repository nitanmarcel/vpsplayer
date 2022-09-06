#ifndef WAVECONVERT_H
#define WAVECONVERT_H

#include <QString>
#include <QFileInfo>
#include <QProcess>

class WaveConvert
{
public:
    WaveConvert();
    ~WaveConvert();
    QString convertFile(QFileInfo fileName);
    bool needsConversion(QFileInfo fileName);
private:
    QProcess *process;
};

#endif // WAVECONVERT_H
