#include "widget.h"
#include<QApplication>
#include<QStandardItemModel>
#include<QDebug>
#include<QItemDelegate>
#include<QObject>

#include<QLineEdit>
#include<QSpinBox>
#include<QComboBox>
#include<QPushButton>
#include<QMap>
#include<QPainter>
#include<QEvent>
#include<QMouseEvent>
#include<QPalette>
QString qss=
R"(
QTableView
{
    color:black;
    selection-color:white;
    selection-background-color:rgb(164,213,242);
    border:2px groove gray;
}
QTableView
{
    /*奇偶行交替变换背景颜色，需要在代码中调用setAlternatingRowColors(true)*/
    background-color:white;
    alternate-background-color:rgb(233,245,252);
}
QHeaderView
{
    color:black;
    font:bold 10pt " Arial";
    background-color:rgb(108,108,108);
}
)";

class ButtonDelegate:public QItemDelegate
{
public:
    ButtonDelegate(QObject* parent =nullptr):QItemDelegate(parent)
    {

    }
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        return nullptr;
    }
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyleOptionButton* button = m_btns.value(index);
        if(button == nullptr)
        {
            button = new QStyleOptionButton;
            button->rect = option.rect;
            button->text = "删除";
            button->state |= QStyle::State_Enabled;
            m_btns.insert(index,button);
        }
        painter->save();
        if (option.state & QStyle::State_Selected)
        {
              //painter->fillRect(option.rect, option.palette.highlight());
              painter->fillRect(option.rect, Qt::red);
        }
        painter->restore();
        QApplication::style()->drawControl(QStyle::CE_PushButton,button,painter);

    }
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *ev = (QMouseEvent*)(event);
            //点击删除当前行
            auto*model = (QStandardItemModel*)(index.model());
           QList<QStandardItem*> list_item = model->takeRow(index.row());
           for(auto&item : list_item)
              delete item;

             qDebug()<<"hello"<<option.index<<" "<<index;
        }
        qDebug()<<"type:"<<event->type();
        // 判断鼠标移动，得先给QtableView设置鼠标追踪
        if(event->type() == QEvent::MouseMove)
        {
            //qDebug()<<"moue move";

        }
        return true;
    }
private:
    mutable QMap<QModelIndex,QStyleOptionButton*> m_btns;
};

class MayeDeDelegate:public QItemDelegate
{
public:
    MayeDeDelegate(QObject *parent =nullptr):QItemDelegate(parent)
    {

    }
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        int col = index.column();
        if(col == 0)
        {
            return nullptr;
        }else if(col == 1)
        {
            QLineEdit* lineEdit = new QLineEdit(parent);
            QRegExp reg("[0-9]{1,12}");
            lineEdit->setValidator(new QRegExpValidator(reg));
            return lineEdit;
        }
        else if(col == 2)
        {
            QItemDelegate::createEditor(parent,option,index);
        }
        else if (col == 3)
        {
            QSpinBox* spinBox = new QSpinBox(parent);
            spinBox->setRange(0,120);
            return spinBox;
        }
        else if(col == 4)
        {
            QComboBox* comboBox = new QComboBox(parent);
            comboBox->addItems({"Female","Male"});
            return comboBox;
        }
        else if(col == 5)
        {
            QPushButton *btn = new QPushButton("删除",parent);
            return btn;
        }
    }
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        if(index.column() == 5)
        {

        }
        else
        {
            QItemDelegate::paint(painter,option,index);
        }
    }
};


MayeTableView::MayeTableView(QWidget *parent)
    : QTableView(parent)
{
    setMouseTracking(true);
    resize(840,480);
    setStyleSheet(qss);

    model = new QStandardItemModel;
    model->setHorizontalHeaderLabels({"No.","ID","Name","Age","Sex","Show"});
    setModel(model);
    model ->setRowCount(5);

    for(int i =0;i<5;i++)
    {
        for(int k = 0;k<6;k++)
        {
           model->setItem(i,k,new QStandardItem(QString("%1,%2").arg(i).arg(k)));
        }
    }

    setAttrbuite();


    QList<QStringList> datas = {
        {"0","11","aaa","4","Female"},
        {"4","14","aaa","4","Female"},
        {"7","12","aaa","4","Female"},
        {"2","16","aaa","4","Female"},
        {"1","13","aaa","4","Female"}
    };
    for(int i =0;i<datas.size();i++)
    {
        QList<QStandardItem*> item_list;
        for(QString str : datas[i])
        {
            item_list<<new QStandardItem(str);
        }
       model->appendRow(item_list);
    }
    signalAndSlots();
}

MayeTableView::~MayeTableView()
{

}

void MayeTableView::signalAndSlots()
{
    //修改了某个项的数据
    connect(model,&QStandardItemModel::itemChanged,this,[](QStandardItem* item)
    {
          qDebug()<<item->text() <<"  "<<item->index();
    });
    //setItemDelegate(new MayeDeDelegate);
    setItemDelegateForColumn(5,new ButtonDelegate);
}

void MayeTableView::setAttrbuite()
{
    //setSelectionMode(QTableView::SingleSelection);
    //设置行交替变色
    setAlternatingRowColors(true);
    QPalette palette;
    palette.setColor(QPalette::Base,QColor(255,255,255));
    palette.setColor(QPalette::AlternateBase,QColor(233,245,252));
    setPalette(palette);

    //设置格子线条样式
    setGridStyle(Qt::PenStyle::DotLine);

    //启用排序
    setSortingEnabled(true);

    //合并单元格
    setSpan(2,2,2,3);

}

