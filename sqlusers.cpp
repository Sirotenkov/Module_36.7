#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <exception>
#include <sqlusers.h>
#include "sqlusers.h"

SqlUsers::SqlUsers()
{

}

SqlUsers::~SqlUsers()
{
    close();
}

bool SqlUsers::open(QString const& host, QString const& database, QString const& username, QString const& password)
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

void SqlUsers::close()
{
    if(db_.isOpen())
        db_.close();
}

bool SqlUsers::find(QString const& username)
{
    QSqlQuery query(QString(R"(
        select login from users
        where username = '%1'
    )").arg(username), db_);
    return query.size() == 1;
}

bool SqlUsers::login(QString const& username, QString const& password)
{
    QSqlQuery query(QString(R"(
        select login, password from users
        where login = '%1' and password = '%2'
    )").arg(username).arg(password), db_);
    return query.size() == 1;
}

bool SqlUsers::signup(QString const& username, QString const& password)
{
    QSqlQuery query(db_);
    return query.exec(QString(R"(
        insert into users(login, password)
        values ('%1', '%2')
    )").arg(username).arg(password));
}

bool SqlUsers::list(QStringList& usernames)
{
    QSqlQuery query(db_);
    auto const success = query.exec(QString(R"(
          select login from users
    )"));
    while (query.next())
    {
        auto const username = query.value(0).toString();
        if(!username.isEmpty())
            usernames.push_back(username);
    }
    return success;
}

bool SqlUsers::block(QString const& username)
{
    QSqlQuery query(db_);
    return query.exec(QString(R"(
        insert into blacklist(login)
        values ('%1')
    )").arg(username));
}

bool SqlUsers::isBlocked(QString const& username)
{
    return false;
}

QString SqlUsers::getLastError()const
{
    return db_.lastError().text();
}
