#ifndef SQLUSERS_H
#define SQLUSERS_H

#include <QSqlDatabase>
#include <QString>
#include <QStringList>

class SqlUsers
{
public:
    SqlUsers();
    ~SqlUsers();

    bool open(QString const& host, QString const& database, QString const& user, QString const& password);
    void close();

    bool find(QString const& username);
    bool login(QString const& username, QString const& password);
    bool signup(QString const& username, QString const& password);
    bool list(QStringList& usernames);
    bool block(QString const& username);
    bool isBlocked(QString const& username);

    QString getLastError()const;

private:
    QSqlDatabase db_;

};

#endif // SQLUSERS_H
