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
