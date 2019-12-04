#ifndef TOOLKIT_H
#define TOOLKIT_H

#include <vector>
#include <string>
#include <QObject>
#include <QTreeWidgetItem>
#include <string>

class Toolkit : public QObject
{
    Q_OBJECT

    struct hostTree {
        std::string link;
        QTreeWidgetItem *parent;
        QTreeWidgetItem *linkRef;
    };

public:
    explicit Toolkit(QObject *parent = nullptr);

// emit
signals:
    void newAcessedLink(QTreeWidgetItem *parent, QTreeWidgetItem *childItem);
    void appendRoot(QTreeWidgetItem *);

// listen
public slots:
    void spider(std::string host);
    void recursiveClient(const std::string host);

private:
    std::vector<std::string> crawler(std::string host);
    void dumper(std::vector<std::string>, std::string host);
};

#endif // TOOLKIT_H
