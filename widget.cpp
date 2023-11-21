#include "widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent,
               QString const& login,
               QString const& host,
               QString const& database,
               QString const& username,
               QString const& password)
    : QMainWindow(parent),
      login_(login)
{
    QFont const font_but("Courrier New", 10, QFont::ExtraLight),
            font_text("Courrier New", 10, QFont::ExtraLight, QFont::Style::StyleItalic);

    if(!sqlUsers_.open(host, database, username, password) ||
            !sqlMessages_.open(host, database, username, password))
        throw std::exception();

    sendToAll_ = new QPushButton("Send to all", this);
    sendToAll_->setFont(font_but);
    connect(sendToAll_, &QPushButton::clicked, this, &Widget::sendMessageAll);

    sendPrivate_ = new QPushButton("Send private", this);
    sendPrivate_->setFont(font_but);
    connect(sendPrivate_, &QPushButton::clicked, this, &Widget::sendMessagePrivate);

    usersCbs_ = new QComboBox(this);
    QStringList usernames;
    if(!sqlUsers_.list(usernames))
        throw std::exception();
    usersCbs_->addItems(usernames);

    usermenu_ = new QMenuBar(this);
    mainMenu_ = new QMenu("Главное меню");
    mainMenu_->setFont(font_text);
    mainMenu_->addAction("Открыть новый клиент");
    mainMenu_->setFont(font_text);

    usermenu_->addMenu(mainMenu_);

    fromMe_ = new QTextEdit(this);
    fromMe_->setFont(font_but);
    toMe_ = new QTextEdit(this);
    toMe_->setFont(font_but);
    label_ = new QLabel("Your message: ", this);
    label_->setFont(font_text);
    textMessage_ = new QLineEdit(this);
    textMessage_->setFont(font_text);
    textMessage_->setPlaceholderText(" Здесь текст сообщения");
    textMessage_->setFont(font_text);

    auto const toolbar2 = new QWidget(this);
    toolbar2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    auto const hBox2 = new QHBoxLayout(toolbar2);
    hBox2->setMargin(5);
    hBox2->setSpacing(5);
    hBox2->addWidget(label_);
    hBox2->addWidget(textMessage_);
    hBox2->addWidget(sendToAll_);
    hBox2->addWidget(sendPrivate_);
    hBox2->addWidget(usersCbs_);

    auto const toolbar3 = new QWidget(this);
    auto const hBox3 = new QHBoxLayout(toolbar3);
    hBox3->setMargin(5);
    hBox3->setSpacing(5);
    hBox3->addWidget(toMe_);
    hBox3->addWidget(fromMe_);

    auto const vBox = new QVBoxLayout(this);

    vBox->setMargin(0);
    vBox->setSpacing(5);
    vBox->addWidget(toolbar2, Qt::AlignRight);
    vBox->addWidget(toolbar3);

    auto const centralWidget = new QWidget(this);
    centralWidget->setLayout(vBox);

    setMenuBar(usermenu_);
    setCentralWidget(centralWidget);
}

Widget::~Widget()
{
    sqlUsers_.close();
    sqlMessages_.close();
}

void Widget::sendMessagePrivate()
{
    if(!login_.isEmpty() && !usersCbs_->currentText().isEmpty() && !textMessage_->text().isEmpty())
        sqlMessages_.send(login_, usersCbs_->currentText(), textMessage_->text());
}

void Widget::sendMessageAll()
{
    if(!login_.isEmpty() && !textMessage_->text().isEmpty())
    {
        QStringList users;
        sqlUsers_.list(users);

        for(auto const& user : users)
            if(!user.isEmpty() && user != login_)
                sqlMessages_.send(login_, user, textMessage_->text());
    }
}
