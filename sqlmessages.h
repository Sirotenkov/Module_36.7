#ifndef SQLMESSAGES_H
#define SQLMESSAGES_H

#include <QString>
#include <QSqlDatabase>

struct Message
{
    QString from;
    QString to;
    QString text;
};

class SqlMessages
{
public:
    SqlMessages();
    ~SqlMessages();

    bool open(QString const& host, QString const& database, QString const& user, QString const& password);
    void close();

    bool send(QString const& from, QString const& to, QString const& text);
    bool recv(QString const& to, QVector<Message>& text);

private:
    QSqlDatabase db_;

};

#endif // SQLMESSAGES_H
