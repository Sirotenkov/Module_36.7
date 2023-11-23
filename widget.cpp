#include "widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent,
               QString const& login,
               QString const& host,
               QString const& database,
               QString const& username,
               QString const& password,
               bool isBlocked)
    : QMainWindow(parent),
      login_(login)
{
    QFont const font_but("Sans serif", 10, QFont::ExtraLight),
            font_text("Courrier New", 10, QFont::ExtraLight, QFont::Style::StyleItalic);

    if(!sqlUsers_.open(host, database, username, password) ||
            !sqlMessages_.open(host, database, username, password))
        throw std::exception();

    sendToAll_ = new QPushButton("Send to all", this);
    sendToAll_->setFont(font_but);
    sendToAll_->setDisabled(isBlocked);
    connect(sendToAll_, &QPushButton::clicked, this, &Widget::sendMessageAll);

    sendPrivate_ = new QPushButton("Send private", this);
    sendPrivate_->setFont(font_but);
    sendPrivate_->setDisabled(isBlocked);
    connect(sendPrivate_, &QPushButton::clicked, this, &Widget::sendMessagePrivate);

    usersCbs_ = new QComboBox(this);
    usersCbs_->setFont(font_text);
    QStringList usernames;
    if(!sqlUsers_.list(usernames))
        throw std::exception();
    usersCbs_->addItems(usernames);
    usersCbs_->setDisabled(isBlocked);

    usermenu_ = new QMenuBar(this);
    mainMenu_ = new QMenu("Главное меню");
    mainMenu_->setFont(font_text);
    mainMenu_->addAction("Открыть новый клиент");
    mainMenu_->setFont(font_text);

    usermenu_->addMenu(mainMenu_);

    labelReceivedPrivate_ = new QLabel("Принятые приватные сообщения", this);
    labelReceivedPrivate_->setFont(font_text);

    privateText_ = new QTextEdit(this);
    privateText_->setFont(font_but);
    privateText_->setTextInteractionFlags(Qt::NoTextInteraction);

    labelReceivedAll_ = new QLabel("Принятые общие сообщения", this);
    labelReceivedAll_->setFont(font_text);

    publicText_ = new QTextEdit(this);
    publicText_->setFont(font_but);
    publicText_->setTextInteractionFlags(Qt::NoTextInteraction);

    label_ = new QLabel("Your message: ", this);
    label_->setFont(font_text);

    textMessage_ = new QLineEdit(this);
    textMessage_->setFont(font_text);
    textMessage_->setPlaceholderText("Введите текст сообщения");
    textMessage_->setFont(font_text);
    textMessage_->setDisabled(isBlocked);

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

    auto const toolbar1 = new QWidget(this);
    auto const hBox1 = new QHBoxLayout(toolbar1);
    hBox1->addWidget(labelReceivedAll_);
    hBox1->addWidget(labelReceivedPrivate_);
    hBox1->setMargin(5);
    hBox1->setSpacing(0);


    auto const toolbar3 = new QWidget(this);
    auto const hBox3 = new QHBoxLayout(toolbar3);
    hBox3->setMargin(5);
    hBox3->setSpacing(5);
    hBox3->addWidget(publicText_);
    hBox3->addWidget(privateText_);

    auto const vBox = new QVBoxLayout(this);

    vBox->setMargin(0);
    vBox->setSpacing(5);
    vBox->addWidget(toolbar2, Qt::AlignRight);
    vBox->addWidget(toolbar1);
    vBox->addWidget(toolbar3);

    auto const centralWidget = new QWidget(this);
    centralWidget->setLayout(vBox);

    setMenuBar(usermenu_);
    setCentralWidget(centralWidget);
    setWindowTitle("chat - " + login_);

    update();
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
        sqlMessages_.send(login_, "all", textMessage_->text());
}

void Widget::update()
{
    QVector<Message> publicMessages, privateMessages;

    sqlMessages_.recv("all", publicMessages);
    sqlMessages_.recv(login_, privateMessages);

    QString privateText, publicText;

    for(auto const& message: publicMessages) {
        if(message.from != login_)
            publicText += QString("<i>&lt;%1&gt;</i>: %2<br>").arg(message.from).arg(message.text);
    }

    for(auto const& message: privateMessages) {
        if(message.from != login_)
            privateText += QString("<i>&lt;%1&gt;</i>: %2<br>").arg(message.from).arg(message.text);
    }

    publicText_->setHtml(publicText);
    privateText_->setHtml(privateText);
}
