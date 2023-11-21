#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <exception>
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
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(host);
    db.setDatabaseName(database);
    db.setUserName(username);
    db.setPassword(password);
    auto ok = db.open();
    if(!ok)
        qDebug() << db.lastError().text();
    return ok;
}

void SqlUsers::close()
{
    if(db.isOpen())
        db.close();
}

bool SqlUsers::find(QString const& username)
{
    QSqlQuery query(QString(R"(
        select username from users
        where username = '%1'
    )").arg(username), db);
    return query.size() == 1;
}

bool SqlUsers::login(QString const& username, QString const& password)
{
    QSqlQuery query(QString(R"(
        select username, password from users
        where username = '%1' and password = '%2'
    )").arg(username).arg(password), db);
    return query.size() == 1;
}

bool SqlUsers::signup(QString const& username, QString const& password)
{
    QSqlQuery query(db);
    return query.exec(QString(R"(
        insert into users(username, password)
        values ('%1', '%2')
    )").arg(username).arg(password));
}
