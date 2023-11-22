#include "logwidget.h"
#include "widget.h"
#include "adminwidget.h"
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
    bool adminLogin = false;

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
                // Для простоты имя админа и его пароль жёстко заданы
                if(l.getUsername() == "admin" && l.getPassword() == "1234")
                {
                    adminLogin = true;
                    break;
                }
                else if (sqlUsers.login(l.getUsername(), l.getPassword()))
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

    QWidget* w = nullptr;

    if(adminLogin)
        w = new AdminWidget(nullptr, host, database, username, password);
    else
        w = new Widget(nullptr, l.getUsername(), host, database, username, password);

    w->show();

    return a.exec();
}
