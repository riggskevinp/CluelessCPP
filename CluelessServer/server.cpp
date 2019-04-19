#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>

#include "server.h"

Server::Server(QWidget *parent)
    : QDialog (parent)
    , statusLabel(new QLabel)
    , gameStatusLabel(new QLabel)
    , sendMessageButton(new QPushButton(tr("Send Message")))
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    gameStatusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Server::sessionOpened);

        //connect(tcpServer, &QTcpServer::newConnection, this, &Server::addPlayers); //modify to create a new player instance and add to list

        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpened();
    }


    auto quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::addPlayer);
    //connect(playerList[0]->getSocket(), &QIODevice::readyRead, this, &Server::receiveMessage);

    sendMessageButton->setDefault(true);
    sendMessageButton->setEnabled(false);
    connect(sendMessageButton, &QAbstractButton::clicked, this, &Server::sendMessage);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(sendMessageButton);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = nullptr;
    if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
        auto outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        auto outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QVBoxLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    } else {
        mainLayout = new QVBoxLayout(this);
    }


    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(gameStatusLabel);
    mainLayout->addLayout(buttonLayout);

    setWindowTitle(QGuiApplication::applicationDisplayName());
}

void Server::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    tcpServer = new QTcpServer(this);
    tcpServer->setMaxPendingConnections(6);

    if (!tcpServer->listen()) {
        QMessageBox::critical(this, tr("Game Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Game Client now.")
                         .arg(ipAddress).arg(tcpServer->serverPort()));
}

void Server::addPlayer()
{
    if(playerList.length() < 3){
        QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
        playerList.append(new Player(clientConnection));

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_10);

        gameStatusLabel->setText(tr("%1 players have joined.\n").arg(playerList.length()));

        QString gameState = QString("Welcome Player%1").arg(playerList.length());
        qint64 message = 2000;
        //out << gameState;
        out << message;
        clientConnection->write(block);
        sendMessageButton->setEnabled(true);
        connect(clientConnection, &QIODevice::readyRead, this, &Server::receiveMessage);
    }

}

void Server::sendMessage()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    qint64 message = 2555;
    out << message;

    //gameState << QString("Game Starting");
    //out << QString("Game Started");



    playerList[0]->getSocket()->write(block);
    sendMessageButton->setEnabled(false);

    //while(i < playerList.length()){
    //    playerList[i]->write(block);
    //    i++;
    //}
}

void Server::receiveMessage()
{
    in.setDevice(playerList[0]->getSocket());
    in.setVersion(QDataStream::Qt_5_10);
    in.startTransaction();

    QString newMessage;
    in >> newMessage;


    //qint64 newMessage;
    //in >> newMessage;

    //if (!in.commitTransaction()){ // potential for error checking
    //    gameStatusLabel-setText("Empty message");
    //    return;
    //}
    gameStatusLabel->setText(newMessage);
    //gameStatusLabel->setText(tr("%i").arg(newMessge));
    sendMessageButton->setEnabled(true);
}


