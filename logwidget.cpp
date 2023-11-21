#include "logwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

LogWidget::LogWidget(QDialog *parent) : QDialog(parent)
{
    QFont const font_but("Courrier New", 10, QFont::ExtraLight),
            font_text("Courrier New", 10, QFont::ExtraLight, QFont::Style::StyleItalic);

    infoLog_ = new QLabel(this);
    infoLog_->setVisible(false);

    login_ = new QLabel("Login:", this);
    login_->setFont(font_but);
    password_ = new QLabel("Password:", this);
    password_->setFont(font_but);
    confPassword_ = new QLabel("Confirm password:", this);
    confPassword_->setFont(font_but);

    textLogin_ = new QLineEdit(this);
    textLogin_->setFont(font_text);

    textPassword_ = new QLineEdit(this);
    textPassword_->setFont(font_text);
    textPassword_->setEchoMode(QLineEdit::Password);

    textConfPassword_ = new QLineEdit(this);
    textConfPassword_->setFont(font_text);
    textConfPassword_->setEchoMode(QLineEdit::Password);

    ok_ = new QPushButton("Ok", this);
    ok_->setFont(font_but);
    connect(ok_, &QPushButton::pressed, this, &QDialog::accept);

    cancel_ = new QPushButton("Cancel", this);
    cancel_->setFont(font_but);
    connect(cancel_, &QPushButton::pressed, this, &QDialog::reject);

    register_ = new QPushButton(this);
    register_->setFont(font_but);
    connect(register_, &QPushButton::pressed, this, [this]{setState(currentState_ == StateLogin ? StateRegister : StateLogin);});

    auto const toolbar4 = new QWidget(this);
    auto const hBox4 = new QHBoxLayout(toolbar4);
    hBox4->addWidget(ok_);
    hBox4->addWidget(cancel_);
    hBox4->addWidget(register_);

    auto const layout = new QGridLayout(this);
    setLayout(layout);

        layout->setSizeConstraint(QLayout::SetFixedSize);

    //layout->setMargin(0);
    layout->setSpacing(5);
    layout->addWidget(infoLog_, 0, 0, 1, 2, Qt::AlignHCenter);
    layout->addWidget(login_, 1, 0, Qt::AlignRight);
    layout->addWidget(textLogin_, 1, 1);
    layout->addWidget(password_, 2, 0, Qt::AlignRight);
    layout->addWidget(textPassword_, 2, 1);
    layout->addWidget(confPassword_, 3, 0, Qt::AlignRight);
    layout->addWidget(textConfPassword_, 3, 1);
    layout->addWidget(toolbar4, 4, 0, 1, 2, Qt::AlignRight | Qt::AlignTop);

    setState(StateLogin);
}

LogWidget::~LogWidget()
{

}

QString LogWidget::getUsername()const
{
    return textLogin_->text();
}

QString LogWidget::getPassword()const
{
    return textPassword_->text();
}

LogWidget::State LogWidget::getState()const {
    return currentState_;
}

void LogWidget::setInfoLog(const QString &log)
{
    infoLog_->setText(log);
    infoLog_->setVisible(!log.isEmpty());
}

void LogWidget::setState(State state)
{
    textLogin_->clear();
    textPassword_->clear();
    textConfPassword_->clear();

    if(state == StateLogin)
    {
        confPassword_->setVisible(false);
        textConfPassword_->setVisible(false);
        register_->setText("Register");
    }

    if(state == StateRegister)
    {
        confPassword_->setVisible(true);
        textConfPassword_->setVisible(true);
        register_->setText("Login");
    }

    currentState_ = state;
}
