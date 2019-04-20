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

qint64 Server::encodeMessage(qint64 m_player, qint64 m_ga, qint64 m_char, qint64 m_weap, qint64 m_row, qint64 m_col)
{
    qint64 m_mes = 0;
    m_mes = m_mes | m_player;
    m_mes = (m_mes << 4) | m_ga;
    m_mes = (m_mes << 4) | m_char;
    m_mes = (m_mes << 4) | m_weap;
    m_mes = (m_mes << 4) | m_row;
    m_mes = (m_mes << 4) | m_col;
    return m_mes;
}

void Server::decodeMessage(qint64 newMes)
{
    t_col = newMes & 0xF;
    t_row = (newMes >> 4) & 0xF;
    t_weapon = (newMes >> 8) & 0xF;
    t_character = (newMes >> 12) & 0xF;
    t_GA = (newMes >> 16) & 0xF;
    t_playerNumber = (newMes >> 20) & 0xF;
}

void Server::answer(qint64 m_message)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    out << m_message;

    //gameState << QString("Game Starting");
    //out << QString("Game Started");



    playerList[t_playerNumber]->getSocket()->write(block);
    sendMessageButton->setEnabled(false);
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
    if(playerList.length() < 6){
        QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
        playerList.append(new Player(clientConnection));

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_10);

        qint64 playerListLength = playerList.length();

        gameStatusLabel->setText(tr("%1 players have joined.\n").arg(playerListLength));

        QString gameState = QString("Welcome Player%1").arg(playerListLength);
        qint64 message = 0;
        if(playerListLength == 1){
            message = encodeMessage(playerListLength - 1,2,6,6,5,5);
        } else if (playerListLength == 2) {
            message = encodeMessage(playerListLength - 1,2,6,6,5,5);
        } else if (playerListLength == 3) {
            message = encodeMessage(playerListLength - 1,2,6,6,5,5);
        } else if (playerListLength == 4) {
            message = encodeMessage(playerListLength - 1,2,6,6,5,5);
        } else if (playerListLength == 5) {
            message = encodeMessage(playerListLength - 1,2,6,6,5,5);
        } else if (playerListLength == 6) {
            message = encodeMessage(playerListLength - 1,2,6,6,5,5);
        }
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

    qint64 message = encodeMessage(playerTurn,2,5,5,4,2);
    out << message;

    //gameState << QString("Game Starting");
    //out << QString("Game Started");



    playerList[playerTurn]->getSocket()->write(block);
    sendMessageButton->setEnabled(false);

    //while(i < playerList.length()){
    //    playerList[i]->write(block);
    //    i++;
    //}
}

void Server::notifyAll()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    qint64 message = encodeMessage(t_playerNumber,t_GA,t_character,t_weapon,t_row,t_col);
    out << message;

    //gameState << QString("Game Starting");
    //out << QString("Game Started");
    for(int i = 0; i < playerList.length(); i++){
        if(t_playerNumber == i){

        } else {
            playerList[i]->getSocket()->write(block);
        }
    }
    //playerList[player]->getSocket()->write(block);
    //sendMessageButton->setEnabled(false);

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

    qint64 newMessage;
    in >> newMessage;
    decodeMessage(newMessage);

    if(t_character != solCharacter){
        // tell playerTurn the character was wrong
        // notify all other players of guess
        // set playerTurn to next player
        //notifyAll();
        answer(encodeMessage(t_playerNumber,2,t_character,6,5,5));
    }

    //qint64 newMessage;
    //in >> newMessage;

    //if (!in.commitTransaction()){ // potential for error checking
    //    gameStatusLabel-setText("Empty message");
    //    return;
    //}

    // Todo turn gameStatusLabel into a scrolling log of all updates
    //gameStatusLabel->setText(newMessage);
    gameStatusLabel->setText(tr("Player%1  GA%2  Character%3  Weapon%4  Row%5 Col%6").arg(t_playerNumber).arg(t_GA).arg(t_character).arg(t_weapon).arg(t_row).arg(t_col));
    sendMessageButton->setEnabled(true);
}


