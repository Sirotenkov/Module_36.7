#include "logwidget.h"
#include "widget.h"
#include "sqlusers.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    LogWidget l;

    while(true)
    {
        if (l.exec() == QDialog::Accepted)
        {
            SqlUsers sqlUsers;
            auto const state = l.getState();

            if(!sqlUsers.open("localhost", "users", "root", "root"))
            {
                l.setInfoLog("Не удалось подключиться к БД!");
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

    w.show();
    return a.exec();
}
