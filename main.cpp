#include "logwidget.h"
#include "widget.h"
#include "sqlusers.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString const host = "localhost",
            database = "users",
            username = "root",
            password = "root";

    LogWidget l;

    while(true)
    {
        if (l.exec() == QDialog::Accepted)
        {
            SqlUsers sqlUsers;
            auto const state = l.getState();

            if(!sqlUsers.open(host, database, username, password))
            {
                QString error = QString("%1\n%2")
                        .arg("Не удалось подключиться к БД!")
                        .arg(sqlUsers.getLastError());
                l.setInfoLog(error);
                continue;
            }

            if(state == LogWidget::State::StateLogin)
            {
                if (sqlUsers.login(l.getUsername(), l.getPassword()))
                {
                    break;
                }
                else
                {
                    l.setInfoLog("Не верные имя пользователя или пароль.");
                    continue;
                }
            }
            else if (state == LogWidget::State::StateRegister)
            {
                if(sqlUsers.find(l.getUsername()))
                {
                    l.setInfoLog(QString(R"(Пользователь '%1' уже сущеcтвует.)").arg(l.getUsername()));
                    continue;
                }

                if(sqlUsers.signup(l.getUsername(), l.getPassword()))
                {
                    break;
                }
                else
                {
                    l.setInfoLog("Не удалось зарегистрировать пользователя.");
                    continue;
                }
            }
        }
        else
        {
            std::exit(0);
        }
    }

    Widget w(nullptr, l.getUsername(), host, database, username, password);
    w.show();

    return a.exec();
}
