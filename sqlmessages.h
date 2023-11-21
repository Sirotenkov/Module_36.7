#ifndef SQLMESSAGES_H
#define SQLMESSAGES_H

#include <QString>
#include <QSqlDatabase>

class SqlMessages
{
public:
    SqlMessages();
    ~SqlMessages();

    bool open(QString const& host, QString const& database, QString const& user, QString const& password);
    void close();

private:
    QSqlDatabase db_;

};

#endif // SQLMESSAGES_H
