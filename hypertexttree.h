#ifndef HYPERTEXTTREE_H
#define HYPERTEXTTREE_H

#include <QMainWindow>

namespace Ui {
class HypertextTree;
}

class HypertextTree : public QMainWindow
{
    Q_OBJECT

public:
    explicit HypertextTree(QWidget *parent = nullptr);
    ~HypertextTree();

public slots:
    void updateActions();
    void insertChild(const QString &link);
    void insertRow(const QString &link);

private slots:
    bool insertColumn();
    bool removeColumn();
    void removeRow();

private:
    Ui::HypertextTree *ui;
};

#endif // HYPERTEXTTREE_H
