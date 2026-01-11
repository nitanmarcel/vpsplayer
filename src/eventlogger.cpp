#include "eventlogger.h"
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>

EventLogger::EventLogger(QObject *parent)
    : QObject(parent)
    , m_enabled(false)
    , m_firstFileLoaded(false)
    , m_sessionStartTime(QDateTime::currentDateTime())
{
    generateSessionLogFileName();
}

EventLogger::~EventLogger()
{
    closeLogFile();
}

void EventLogger::generateSessionLogFileName()
{
    QString timestamp = m_sessionStartTime.toString("yyyy-MM-dd_HH-mm-ss");
    m_sessionLogFileName = QString("vpsplayer_%1.log").arg(timestamp);
}

QString EventLogger::getDefaultLogDirectory()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) 
           + QDir::separator() + "logs";
}

QString EventLogger::getLogDirectory() const
{
    if (!m_customLogPath.isEmpty()) {
        return m_customLogPath;
    }
    return getDefaultLogDirectory();
}

void EventLogger::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;

    if (m_enabled) {
        openLogFile();
    } else {
        closeLogFile();
    }
}

bool EventLogger::isEnabled() const
{
    return m_enabled;
}

void EventLogger::setCustomLogPath(const QString &path)
{
    if (m_customLogPath == path)
        return;

    bool wasEnabled = m_enabled;
    
    if (m_logFile.isOpen()) {
        closeLogFile();
    }
    
    m_customLogPath = path;
    
    if (wasEnabled) {
        openLogFile();
    }
}

QString EventLogger::getCustomLogPath() const
{
    return m_customLogPath;
}

QString EventLogger::getActiveLogFilePath() const
{
    return getLogDirectory() + QDir::separator() + m_sessionLogFileName;
}

bool EventLogger::openLogFile()
{
    if (m_logFile.isOpen())
        return true;

    QString logDir = getLogDirectory();
    
    QDir dir(logDir);
    if (!dir.exists()) {
        if (!QDir().mkpath(logDir)) {
            return false;
        }
    }

    QString logFilePath = logDir + QDir::separator() + m_sessionLogFileName;
    m_logFile.setFileName(logFilePath);
    
    if (!m_logFile.open(QIODevice::Append | QIODevice::Text)) {
        return false;
    }

    m_stream.setDevice(&m_logFile);
    
    m_stream << "Session started: " << m_sessionStartTime.toString(Qt::ISODate) << "\n";
    m_stream.flush();
    
    return true;
}

void EventLogger::closeLogFile()
{
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
}

QString EventLogger::getCurrentTimestamp() const
{
    return QDateTime::currentDateTime().toString(Qt::ISODate);
}

QString EventLogger::formatPosition(int positionMs) const
{
    int totalSeconds = positionMs / 1000;
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;
    int ms = positionMs % 1000;

    return QString("%1:%2:%3.%4")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'))
        .arg(ms, 3, 10, QChar('0'));
}

void EventLogger::writeLogEntry(const QString &eventName, const QString &details)
{
    if (!m_enabled)
        return;

    if (!m_logFile.isOpen() && !openLogFile())
        return;

    QString entry = QString("[%1] %2").arg(getCurrentTimestamp(), eventName);
    
    if (!details.isEmpty()) {
        entry += QString(" | %1").arg(details);
    }

    m_stream << entry << "\n";
    m_stream.flush();
}

void EventLogger::logFileLoaded(const QString &filePath, bool isFirstFile)
{
    QString eventName = isFirstFile ? "FIRST_FILE_LOADED" : "FILE_LOADED";
    writeLogEntry(eventName, QString("Path: %1").arg(filePath));
    
    if (isFirstFile) {
        m_firstFileLoaded = true;
    }
}

void EventLogger::logPlay(int positionMs)
{
    writeLogEntry("PLAY", QString("Position: %1").arg(formatPosition(positionMs)));
}

void EventLogger::logPause(int positionMs)
{
    writeLogEntry("PAUSE", QString("Position: %1").arg(formatPosition(positionMs)));
}

void EventLogger::logStop(int positionMs)
{
    writeLogEntry("STOP", QString("Position: %1").arg(formatPosition(positionMs)));
}

void EventLogger::logSpeedChange(int speedValue, double speedRatio)
{
    writeLogEntry("SPEED_CHANGE", QString("Value: %1, Ratio: x%2").arg(speedValue).arg(speedRatio, 0, 'f', 2));
}

void EventLogger::logPitchChange(int pitchValue)
{
    writeLogEntry("PITCH_CHANGE", QString("Value: %1 semitones").arg(pitchValue));
}

void EventLogger::logSkip(int deltaMs, int newPositionMs)
{
    QString direction = deltaMs > 0 ? "forward" : "backward";
    int absDelta = qAbs(deltaMs) / 1000;
    writeLogEntry("SKIP", QString("%1 %2s | New position: %3").arg(direction).arg(absDelta).arg(formatPosition(newPositionMs)));
}

void EventLogger::logWaveformClick(int positionMs)
{
    writeLogEntry("WAVEFORM_CLICK", QString("Position: %1").arg(formatPosition(positionMs)));
}

void EventLogger::logBreakpointSet(int positionMs)
{
    writeLogEntry("BREAKPOINT_SET", QString("Position: %1").arg(formatPosition(positionMs)));
}

void EventLogger::logBreakpointRemoved()
{
    writeLogEntry("BREAKPOINT_REMOVED");
}
