#include "hypertexttree.h"
#include "ui_hypertexttree.h"
#include "treemodel.h"

#include <QFile>
#include <QtDebug>

HypertextTree::HypertextTree(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HypertextTree)
{
    ui->setupUi(this);
}

HypertextTree::~HypertextTree()
{
    delete ui;
}

void HypertextTree::addTreeRoot(QTreeWidgetItem *rootItem)
{
//    QTreeWidgetItem *rootItem = new QTreeWidgetItem();
//    rootItem->setText(0, name);
    ui->view->addTopLevelItem(rootItem);
}

void HypertextTree::addTreeChild(QTreeWidgetItem *parent, QTreeWidgetItem *childItem)
{
//    QTreeWidgetItem *childItem = new QTreeWidgetItem();
//    childItem->setText(0, name);

    parent->addChild(childItem);
}
