#ifndef KINECTHANDLER_H
#define KINECTHANDLER_H

#include <QEventLoop>

class KinectHandler : public QEventLoop
{
    Q_OBJECT
public:
    explicit KinectHandler(QEventLoop *parent = 0);

//signals:

//public slots:

protected:
    void timerEvent(QTimerEvent* event);
};

#endif // KINECTHANDLER_H
