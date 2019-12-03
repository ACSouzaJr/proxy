#ifndef TOOLKIT_H
#define TOOLKIT_H

#include <vector>
#include <string>
#include <QObject>

class Toolkit : public QObject
{
    Q_OBJECT

public:
    explicit Toolkit(QObject *parent = nullptr);

// listen
public slots:
    void spider(const std::string host);
    void recursiveClient(const std::string host);

private:
    std::vector<std::string> crawler(std::string host);
    void dumper(std::vector<std::string>, std::string host);
};

#endif // TOOLKIT_H
