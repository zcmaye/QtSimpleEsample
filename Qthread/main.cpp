#include <QApplication>
#include <QThread>
#include <QDebug>
#include <QObject>
#include <QWidget>
#include<QPushButton>
#include<QEventLoop>

#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlRecord>

class MayeWorker:public QObject
{
    Q_OBJECT
public:
    MayeWorker(QObject*parent = nullptr):QObject(parent)
    {
        buf = new char[100];
        strcpy(buf,"my name is MayeWorker");
    }
    void setBuf(const char*str)
    {
        strcpy(buf,str);
    }
public slots:
    void doWork()
    {

        qDebug()<<"doWork...begin "<<buf;
        QThread::sleep(5);
        qDebug()<<"doWork...end";
        emit finished(buf);
    }
signals:
    void finished(char * buf);
private:
    char * buf;
};

class MayeWidget:public QWidget
{
    Q_OBJECT
public:
    MayeWidget(QWidget*parent=nullptr):QWidget(parent)
    {
        QThread* thread = new QThread;
        MayeWorker*worker = new MayeWorker;
        worker->moveToThread(thread);
       // connect(thread,&QThread::started,worker,&MayeWorker::doWork);
        connect(thread,&QThread::started,worker,&MayeWorker::doWork);
        connect(worker,&MayeWorker::finished,thread,[=](char*buf)
        {
            thread->quit();
            qDebug()<<"buf->: " <<buf;
            pbuf = buf;
        });

        connect(thread,&QThread::finished,this,[=]()
        {
            qDebug()<<"finished";
            //thread->deleteLater();
            //worker->deleteLater();
        });


        QPushButton*btn = new QPushButton("我是线程开启按钮",this);

        connect(btn,&QPushButton::clicked,this,[=]()
        {
            worker->setBuf("你是个jib");
            qDebug()<<"started";
            thread->start();
        });

        QPushButton* newbtn = new QPushButton("touch me",this);
        newbtn->move(250,0);
        connect(newbtn,&QPushButton::clicked,this,[=]()
        {
            qDebug()<<"touch me"<<pbuf;
            if(pbuf)
            {
                //delete pbuf;
                //pbuf=nullptr;
            }
        });
    }
private:
    char * pbuf =nullptr;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MayeWidget w;

    w.show();
    return a.exec();
}
#include"main.moc"





