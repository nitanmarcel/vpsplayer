#ifndef EVENTLOGGER_H
#define EVENTLOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDateTime>

class EventLogger : public QObject
{
    Q_OBJECT
public:
    explicit EventLogger(QObject *parent = nullptr);
    ~EventLogger();

    void setEnabled(bool enabled);
    bool isEnabled() const;

    void setCustomLogPath(const QString &path);
    QString getCustomLogPath() const;
    QString getActiveLogFilePath() const;
    static QString getDefaultLogDirectory();

    void logFileLoaded(const QString &filePath, bool isFirstFile);
    void logPlay(int positionMs);
    void logPause(int positionMs);
    void logStop(int positionMs);
    void logSpeedChange(int speedValue, double speedRatio);
    void logPitchChange(int pitchValue);
    void logSkip(int deltaMs, int newPositionMs);
    void logWaveformClick(int positionMs);
    void logBreakpointSet(int positionMs);
    void logBreakpointRemoved();
    void logAltPause(int positionMs, bool jumpedToBreakpoint);
    void logAltResume(int positionMs, bool jumpedToBreakpoint);

private:
    bool m_enabled;
    QFile m_logFile;
    QTextStream m_stream;
    bool m_firstFileLoaded;
    QString m_customLogPath;
    QString m_sessionLogFileName;
    QDateTime m_sessionStartTime;

    void writeLogEntry(const QString &eventName, const QString &details = QString());
    QString formatPosition(int positionMs) const;
    QString getCurrentTimestamp() const;
    bool openLogFile();
    void closeLogFile();
    QString getLogDirectory() const;
    void generateSessionLogFileName();
};

#endif // EVENTLOGGER_H
