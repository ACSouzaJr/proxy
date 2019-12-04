#ifndef HYPERTEXTTREE_H
#define HYPERTEXTTREE_H

#include <QMainWindow>
#include <QTreeWidget>

namespace Ui {
class HypertextTree;
}

class HypertextTree : public QMainWindow
{
    Q_OBJECT

public:
    explicit HypertextTree(QWidget *parent = nullptr);
    ~HypertextTree();
    void addTreeRoot(QTreeWidgetItem *rootItem);
    void addTreeChild(QTreeWidgetItem *parent, QTreeWidgetItem *childItem);

private:
    Ui::HypertextTree *ui;
};

#endif // HYPERTEXTTREE_H
