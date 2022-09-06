#include <QProcess>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

#include "waveconvert.h"


WaveConvert::WaveConvert()
{
  process = new QProcess;
}

WaveConvert::~WaveConvert()
{

}

QString WaveConvert::convertFile(QFileInfo fileName)
{
    if (!needsConversion(fileName))
        return fileName.canonicalFilePath();
    QString newFileName = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + fileName.completeBaseName() + QString(".wav");
    process->start("/usr/bin/ffmpeg", QStringList()
                   << "-y"
                   << "-i"
                   << fileName.canonicalFilePath()
                   << "-acodec"
                   << "pcm_u8"
                   << "-ar"
                   << "22050"
                   << newFileName);
    process->waitForFinished();
    qDebug() << process->program() << " " << process->arguments();
    qDebug() << "Error " << process->readAllStandardError();
    return newFileName;
}

bool WaveConvert::needsConversion(QFileInfo fileName)
{
    return fileName.completeSuffix() != "wav";
}
