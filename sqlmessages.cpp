#include "sqlmessages.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

SqlMessages::SqlMessages()
{

}

SqlMessages::~SqlMessages()
{
    close();
}

bool SqlMessages::open(QString const& host, QString const& database, QString const& username, QString const& password)
{
    db_ = QSqlDatabase::addDatabase("QMYSQL");
    db_.setHostName(host);
    db_.setDatabaseName(database);
    db_.setUserName(username);
    db_.setPassword(password);
    auto ok = db_.open();
    if(!ok)
        qDebug() << db_.lastError().text();
    return ok;
}

void SqlMessages::close()
{
    if(db_.isOpen())
        db_.close();
}

bool SqlMessages::send(QString const& from, QString const& to, QString const& text)
{
    QSqlQuery query(db_);
    return query.exec(QString(R"(
        insert into messages(login_from, login_to, text)
        values ('%1', '%2', '%3')
    )").arg(from).arg(to).arg(text));
}

bool SqlMessages::recv(QString const& to, QVector<Message>& messages)
{
    QSqlQuery query(db_);
    auto const success = query.exec(QString(R"(
        select login_from, login_to, text from messages
        where login_to = '%1'
    )").arg(to));
    while (query.next())
    {
        messages.push_back({
           query.value(0).toString(),
           query.value(1).toString(),
           query.value(2).toString()
       });
    }
    return success;
}
