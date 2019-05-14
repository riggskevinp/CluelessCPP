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
    /*
     * Constructor for the server
     * This has the UI elements and sets up the actual 'server' portion
     *
     * Tough to document how the UI is set up, I recommend reading carefully
     * This is creating the UI entirely through code instead of using a UI form
     */

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    // Potentiall replace gameStatusLabel with a scrolling textbox in order to handl logging
    // We can dump the contents when the application closes
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
    // Signal and slot example, we connect the signal newConnection that is emitted by tcpServer and connect it to the slot addPlayer
    // Thus for each new connection, addPlayer gets run.
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
    /*
     * Takes as inputs the integers that you want to send and encodes them into an integer to send
     */
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
    /*
     * This decodes incoming messages and sets the global variables to those values
     *
     * Not a very good solution but time is short
     */
    t_col = newMes & 0xF;
    t_row = (newMes >> 4) & 0xF;
    t_weapon = (newMes >> 8) & 0xF;
    t_character = (newMes >> 12) & 0xF;
    t_GA = (newMes >> 16) & 0xF;
    t_playerNumber = (newMes >> 20) & 0xF;
}

void Server::answer(qint64 m_message)
{
    /*
     * Function whose intent is to respond to a client's guess with an answer
     *
     * Tell the client what it got wrong
     *
     * Need to decide how this message encoding will work
     */

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    out << m_message;

    // need to determine who and how to access who to send the message to
    playerList[t_playerNumber]->getSocket()->write(block);
    sendMessageButton->setEnabled(false);
}


void Server::sessionOpened()
{
    /*
     * This is taken from the Qt client server example
     * When you start the server, it goes through possible ways to set itself up and then presents you with the result
     * It has some error handling
     *
     * No modifications needed as far as I'm aware
     */

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
    /*
     * This is a slot that when a client connects for the first time, it is added as a new player, and told which player it is
     *
     * This could probably also initialize the clients with their 'cards'
     *
     * Current logic limits players at 6, any new connections will be 'ignored' really just not handled
     */
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

        out << message;
        clientConnection->write(block);
        sendMessageButton->setEnabled(false);
        connect(clientConnection, &QIODevice::readyRead, this, &Server::receiveMessage);
    }

}

void Server::sendMessage()
{
    /*
     * Slot that has been used for testing sending messages
     * This slot is connected to the SendMessage button
     *
     * Good for troublshooting
     *
     */
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    qint64 message = encodeMessage(playerTurn,2,5,5,4,2);
    out << message;

    playerList[playerTurn]->getSocket()->write(block);
    sendMessageButton->setEnabled(false);
}

void Server::notifyAll()
{
    /*
     * Function for notifying the clients of the current clients move
     * */
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    qint64 message = encodeMessage(t_playerNumber,t_GA,t_character,t_weapon,t_row,t_col);
    out << message;

    for(int i = 0; i < playerList.length(); i++){
        if(t_playerNumber == i){

        } else {
            playerList[i]->getSocket()->write(block);
        }
    }
}

void Server::receiveMessage()
{
    /*
     * !!!Problem with this!!!!!  Also See Server::answer() for continuation of the problem
     *
     * This is a slot that should be used for handling messages coming from clients
     * We only want to listen to the client whose turn it is.  ie client == playerturn
     *
     * All other clients we want to ignore
     *
     * This slot is connected to each of the client connections readyread signals
     *
     * Currently this will never work as we are trying to ascertain who the message is coming from by using the message
     * But we will only read the message if we know it is coming from the correct client
     *
     * Possible solution:
     * We set in.setDevice(playerList[playerTurn]->getSocket()); This set the correct client that we listen to
     * However, everytime a different client sends a message, we are going to read an empty message and send that to everyone
     * I don't know what problems this might cause
     *
     * Possible solution: check to see if you can access the connection object that emitted the signal.
     * This way you can test whether the clientconnection == playerList[playerturn]->getSocket()
     *
     * Doesn't solve this, but receive message might belong inside of the player class? Maybe?
     */
    //sender() can be used to get the object who emitted the signal
    for(int i = 0; i < playerList.length(); i++){
        in.setDevice(playerList[i]->getSocket());
        in.setVersion(QDataStream::Qt_5_10);
        in.startTransaction();

        qint64 newMessage;
        in >> newMessage;
        if(in.commitTransaction()){
            decodeMessage(newMessage);
            notifyAll();

            //create answer to guessing player
            //if a guess is correct, send back an invalid int ie 6
            if(t_character == solCharacter){
                t_character = 6;
            }
            if(t_weapon == solWeapon){
                t_weapon = 6;
            }
            if(t_row == solRow && t_col == solCol){
                t_row = 5;
                t_col = 5;
            }
            //what to do if all are correct? If it is a suggetion, nothing different,
            //if it's an accusation, we need to do a notify all to tell the other players who won.
            //we can use t_GA to say if they won, lost, etc.

            answer(encodeMessage(t_playerNumber,2,t_character,t_weapon,t_row,t_col));
            // Todo turn gameStatusLabel into a scrolling log of all updates
            //gameStatusLabel->setText(newMessage);
            gameStatusLabel->setText(tr("Player%1  GA%2  Character%3  Weapon%4  Row%5 Col%6").arg(t_playerNumber).arg(t_GA).arg(t_character).arg(t_weapon).arg(t_row).arg(t_col));
        }
    }
}



