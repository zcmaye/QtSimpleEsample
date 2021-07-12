#ifndef WIDGET_H
#define WIDGET_H

#include <QTableView>
class QStandardItemModel;

class MayeTableView : public QTableView
{
    Q_OBJECT

public:
    MayeTableView(QWidget *parent = nullptr);
    ~MayeTableView();
    void signalAndSlots();
    void setAttrbuite();
private:
    QStandardItemModel *model;
};
#endif // WIDGET_H
