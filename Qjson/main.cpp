#include <QApplication>
#include<QJsonDocument>
#include<QJsonParseError>

#include<QWidget>
#include<QFile>
#include<QMessageBox>

class Widget :public QWidget
{
public:
    Widget(QWidget*parent = nullptr):QWidget(parent)
    {
        QByteArray jsonData = readJsonFile("://res/weather.json");

        QJsonParseError jsonErr;
        QJsonDocument jsonDoc;
        jsonDoc.fromJson(jsonData,&jsonErr);
        if(jsonDoc.isEmpty() || jsonErr.error != QJsonParseError::NoError)
        {
            QMessageBox::critical(this,"error","json数据解析失败");
        }



    }
    QByteArray readJsonFile(const QString& fileName)
    {
        QFile file(fileName);
        if(!file.open(QIODevice::Text | QIODevice::ReadOnly))
        {
            QMessageBox::critical(this,"error",QString("Cannot read file %1").arg(fileName));
            return nullptr;
        }
        return file.readAll();
    }

};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
