#include <QDateTime>
#include <QDebug>
#include <QMutex>
#include <QFile>
#include <QDir>
#include <QApplication>

#include "logFile.h"

const QString CODE_SYSTEM_BACKER_PATH = ".";

static QString logDirectory;
static QString logFileName;
static QFile file;
static QMutex mutex;

//系统日志文件名
QString syslog = "system.log";



void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    //加锁
    //static QMutex mutex;
    mutex.lock();

    QByteArray localMsg = msg.toLocal8Bit();
    QString level;
    switch(type) {
    case QtDebugMsg:
        level = QString("Debug: ");
        break;
    case QtWarningMsg:
        level = QString("Warning: ");
        break;
    case QtCriticalMsg:
        level = QString("Critical: ");
        break;
    case QtFatalMsg:
        level = QString("Fatal: ");
        break;
    default:
        level = QString("Debug: ");
    }

    //设置输出信息格式
    QString context_info = QString("File:%1 Line:%2").arg(QString(context.file)).arg(context.line); // F文件L行数
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString strMessage = QString("[%1] [%2] [%3] \t%4").arg(strDateTime).arg(level).arg(context_info).arg(msg);
    // 输出信息至文件中（读写、追加形式）,这种形式每个qDebug都会生成一个日志文件，这显然不是我们需要的！
    //QFile file(CODE_SYSTEM_BACKER_PATH + QString(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss").append("-log.txt")));
    //QFile file("debuglog.txt");
    //qint64 filesize = file.size();
    //qDebug() << "file size:" << filesize << endl;
    //file.open(QIODevice::ReadWrite | QIODevice::Append);
    file.setFileName(logFileName);
    file.open(QIODevice::WriteOnly | QIODevice::Append);


    QTextStream stream(&file);
    stream << strMessage << "\r\n";
    file.flush();
    file.close();
    // 解锁
    mutex.unlock();
}

void logInit()
{
    //logDirectory  = QApplication::applicationDirPath() + "/Log/";
    logDirectory = QDir::currentPath() + "/LOG/";
    //
    QDir dir(logDirectory);
    if (!dir.exists())
    {
        dir.mkdir(logDirectory);
    }
    //logFileName = logDirectory + (QApplication::applicationName() + ".log");
    logFileName = logDirectory + syslog;
    //qDebug() << "log path:" << logDirectory << "log:" << logFileName << endl;

    /*以下这段代码的含义是初始化时检查日志文件是否存在一个月之前的日志，如果存在删除之*/
    {
        QMutexLocker locker(&mutex);
        QFile file(logFileName);
        QTextStream textStream(&file);
        QString temp;
        QStringList tempList;
        QRegExp regExp(".*(20\\d\\d-\\d\\d-\\d\\d).*");
        while((temp = textStream.readLine()).isEmpty() == false)
        {
            if(temp.indexOf(regExp) >= 0)
            {
                QDate date = QDate::fromString(regExp.cap(1), "yyyy-MM-dd");
                QDate currentDate = QDate::currentDate();
                /*判断当前行所记载的日期和现今的日期天数之差是否大于记录该条日志时的那个月的天数*/
                if(date.daysTo(currentDate) < date.day())
                {
                    tempList << temp;
                    tempList << textStream.readLine();
                }
            }
        }
        file.close();
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        textStream.setDevice(&file);
        for(auto iterator = tempList.begin(); iterator != tempList.end(); iterator++)
        {
            textStream << *iterator << endl;
        }
        file.close();

    }
    qInstallMessageHandler(outputMessage);
}
