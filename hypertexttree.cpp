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

    const QStringList headers({tr("Title"), tr("Description")});

//    QFile file(":/default.txt");
//    file.open(QIODevice::ReadOnly);
    TreeModel *model = new TreeModel(headers, "");
//    file.close();

    ui->view->setModel(model);
    for (int column = 0; column < model->columnCount(); ++column)
        ui->view->resizeColumnToContents(column);

//    connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    connect(ui->view->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &HypertextTree::updateActions);

//    connect(actionsMenu, &QMenu::aboutToShow, this, &HypertextTree::updateActions);
//    connect(insertRowAction, &QAction::triggered, this, &HypertextTree::insertRow);
//    connect(insertColumnAction, &QAction::triggered, this, &HypertextTree::insertColumn);
//    connect(removeRowAction, &QAction::triggered, this, &HypertextTree::removeRow);
//    connect(removeColumnAction, &QAction::triggered, this, &HypertextTree::removeColumn);
//    connect(insertChildAction, &QAction::triggered, this, &HypertextTree::insertChild);

    updateActions();
}

HypertextTree::~HypertextTree()
{
    delete ui;
}

void HypertextTree::insertChild(const QString &link)
{   
    qDebug() << "inseriu no\n";
    const QModelIndex index = ui->view->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->view->model();

    if (model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index))
            return;
    }

    if (!model->insertRow(0, index))
        return;

    for (int column = 0; column < model->columnCount(index); ++column) {
        const QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant(link), Qt::EditRole);
        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, Qt::Horizontal, QVariant(tr("")), Qt::EditRole);
    }

    ui->view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                            QItemSelectionModel::ClearAndSelect);
    updateActions();
}

bool HypertextTree::insertColumn()
{
    QAbstractItemModel *model = ui->view->model();
    int column = ui->view->selectionModel()->currentIndex().column();

    // Insert a column in the parent item.
    bool changed = model->insertColumn(column + 1);
    if (changed)
        model->setHeaderData(column + 1, Qt::Horizontal, QVariant("[No header]"), Qt::EditRole);

    updateActions();

    return changed;
}

void HypertextTree::insertRow(const QString &link)
{
    const QModelIndex index = ui->view->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->view->model();

    if (!model->insertRow(index.row()+1, index.parent()))
        return;

    updateActions();

    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        const QModelIndex child = model->index(index.row() + 1, column, index.parent());
        model->setData(child, QVariant(link), Qt::EditRole);
    }
}

bool HypertextTree::removeColumn()
{
    QAbstractItemModel *model = ui->view->model();
    const int column = ui->view->selectionModel()->currentIndex().column();

    // Insert columns in each child of the parent item.
    const bool changed = model->removeColumn(column);
    if (changed)
        updateActions();

    return changed;
}

void HypertextTree::removeRow()
{
    const QModelIndex index = ui->view->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->view->model();
    if (model->removeRow(index.row(), index.parent()))
        updateActions();
}

void HypertextTree::updateActions()
{
    const bool hasSelection = !ui->view->selectionModel()->selection().isEmpty();

    const bool hasCurrent = ui->view->selectionModel()->currentIndex().isValid();

    if (hasCurrent) {
        ui->view->closePersistentEditor(ui->view->selectionModel()->currentIndex());

        const int row = ui->view->selectionModel()->currentIndex().row();
        const int column = ui->view->selectionModel()->currentIndex().column();
        if (ui->view->selectionModel()->currentIndex().parent().isValid())
            statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
        else
            statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
    }
}
