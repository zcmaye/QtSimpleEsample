#include<QApplication>
#include<QDebug>
#include<QSqlQuery>
#include<QSqlRecord>
#include<QSqlDatabase>
#include<QStandardItemModel>
#include<QWidget>

#include<QPushButton>
#include<QTableView>
#include<QLayout>
#include<QThread>
#define cout qDebug()

class Worker_CreateModel:public QObject
{
    Q_OBJECT
public:
    Worker_CreateModel(QObject*parent = nullptr):QObject(parent)
    {
        m_model = new QStandardItemModel;
    }
    void connectDataBase()
    {
        static int cnt = 0;
        dataBaseConName = QString("thread_sql_%1").arg(cnt++);
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL",dataBaseConName);
        db.setHostName("81.70.201.51");
        db.setPort(3306);
        db.setUserName("root");
        db.setPassword("dunkaiedu@20190219");
        db.setDatabaseName("testqt");
        //db.setDatabaseName("yellowriver");
        if(!db.open())
        {
            qDebug()<<"数据库打开失败";
        }
    }
    void createModel()
    {
       connectDataBase();
       QSqlQuery query(QSqlDatabase::database(dataBaseConName));
       if(!query.exec(sql))
       {
           qDebug()<<"query 执行失败";
           searchFinished(m_model);
           return;
       }

       QStringList header;
       //获取表的所有字段
       QSqlRecord record(query.record());
       for(int i =0;i<record.count();i++)
       {
           //header << fieldAliasComparisonTable.value(record.fieldName(i));
           header << record.fieldName(i);
       }
       m_model->clear();
       cout<<"beg "<<m_model->rowCount()<<" "<<m_model->columnCount()<<" "<<header.size();
       m_model->setHorizontalHeaderLabels(header);
       cout<<"header "<<m_model->rowCount()<<" "<<m_model->columnCount();
       //获取所有数据
        while(query.next())
        {
            QList<QStandardItem*> items;
            for(int i = 0;i<record.count();i++)
            {
               items << new QStandardItem(query.value(i).toString());
            }
            m_model->appendRow(items);
        }
        searchFinished(m_model);
        QSqlDatabase::removeDatabase(dataBaseConName);
    }
    void setSql(const QString& sql)
    {
        this->sql = sql;
    }
signals:
    void searchFinished(QStandardItemModel* model);
private:
    QStandardItemModel* m_model;
    QString sql;
    QString dataBaseConName;
};
class MayeWidget:public QWidget
{
    Q_OBJECT
public:
    MayeWidget(QWidget*parent =nullptr):QWidget(parent)
    {
        worker = new Worker_CreateModel;
        thread = new QThread;
        worker->moveToThread(thread);
        connect(thread,&QThread::started,worker,&Worker_CreateModel::createModel);
        connect(worker,&Worker_CreateModel::searchFinished,this,[=](QStandardItemModel*model)
        {

            tableView->setModel(model);

               qDebug()<<"setmodel cols:"<<model->columnCount();
               thread->quit();
        });
        connect(thread,&QThread::finished,this,[]()
        {
            qDebug()<<"thread finished";
        });

        initUi();
    }
    void initUi()
    {
        searchBtn = new QPushButton("查询");
        clearBtn = new QPushButton("清空");
        touchBtn = new QPushButton("Touch Me");
        tableView = new QTableView;

        QVBoxLayout* v = new QVBoxLayout;
        v->addWidget(searchBtn);
        v->addWidget(clearBtn);
        v->addWidget(touchBtn);
        v->addWidget(tableView);

        setLayout(v);

        connect(searchBtn,&QPushButton::clicked,this,[=]()
        {
            QString sql = QString("select * from freecustomers where assistant='%1' and entrytime>='%2' and entrytime<='%3'")
                        .arg("叶修语").arg("2021-01-20").arg("2021-07-07");

            worker->setSql(sql);
            thread->start();
            qDebug()<<"start";
        });
        connect(touchBtn,&QPushButton::clicked,this,[]()
        {
             static int i = 0;
             qDebug()<<"touch me "<<i++;
        });
        connect(clearBtn,&QPushButton::clicked,this,[=]()
        {
            QStandardItemModel* model =  qobject_cast<QStandardItemModel*>(tableView->model());
            if(model)
            {
                model->clear();
            }
        });
    }
private:
    QPushButton* searchBtn;
    QPushButton* clearBtn;
    QPushButton* touchBtn;
    QTableView* tableView;

    Worker_CreateModel* worker;
    QThread * thread;
};


int main(int argc,char *argv[])
{
    QApplication a(argc,argv);

    MayeWidget w;
    w.show();


    return a.exec();
}
#include"main_sql.moc"
