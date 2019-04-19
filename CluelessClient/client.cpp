#include <QtWidgets>
#include <QtNetwork>
#include <QGroupBox>

#include "client.h"

Client::Client(QWidget *parent)
    : QDialog(parent)
    , hostCombo(new QComboBox)
    , portLineEdit(new QLineEdit)
    , joinGameButton(new QPushButton(tr("Join Game")))
    , sendMessageButton(new QPushButton(tr("Send Message")))
    , tcpSocket(new QTcpSocket(this))
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    hostCombo->setEditable(true);

    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            hostCombo->addItem(name + QChar('.') + domain);
    }
    if (name != QLatin1String("localhost"))
        hostCombo->addItem(QString("localhost"));

    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    auto hostLabel = new QLabel(tr("&Server name:"));
    hostLabel->setBuddy(hostCombo);
    auto portLabel = new QLabel(tr("S&erver port:"));
    portLabel->setBuddy(portLineEdit);

    statusLabel = new QLabel(tr("This examples requires that you run the "
                                "Client Server as well."));

    joinGameButton->setDefault(true);
    joinGameButton->setEnabled(false);

    sendMessageButton->setDefault(true);
    sendMessageButton->setEnabled(false);

    auto quitButton = new QPushButton(tr("Quit"));

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(joinGameButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(sendMessageButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    // Need to track what radio buttons are enabled and where the player's icon is
    // When make guess is selected, that information is sent to the server in a coded form
    // Need to decide how to encode data packets

    auto charBox = new QGroupBox(tr("Characters"));
    QGridLayout *charLayout = new QGridLayout;
    // when we get a "card" emit a signal that disables the respective button
    QRadioButton *radioA = new QRadioButton(tr("A"));
    QRadioButton *radioB = new QRadioButton(tr("B"));
    QRadioButton *radioC = new QRadioButton(tr("C"));
    QRadioButton *radioD = new QRadioButton(tr("D"));
    QRadioButton *radioE = new QRadioButton(tr("E"));
    QRadioButton *radioF = new QRadioButton(tr("F"));
    charLayout->addWidget(radioA);
    charLayout->addWidget(radioB);
    charLayout->addWidget(radioC);
    charLayout->addWidget(radioD);
    charLayout->addWidget(radioE);
    charLayout->addWidget(radioF);
    charBox->setLayout(charLayout);

    auto weapBox = new QGroupBox(tr("Weapons"));
    QGridLayout *weapLayout = new QGridLayout;
    // when we get a "card" emit a signal that disables the respective button
    QRadioButton *radioU = new QRadioButton(tr("U"));
    QRadioButton *radioV = new QRadioButton(tr("V"));
    QRadioButton *radioW = new QRadioButton(tr("W"));
    QRadioButton *radioX = new QRadioButton(tr("X"));
    QRadioButton *radioY = new QRadioButton(tr("Y"));
    QRadioButton *radioZ = new QRadioButton(tr("Z"));
    weapLayout->addWidget(radioU);
    weapLayout->addWidget(radioV);
    weapLayout->addWidget(radioW);
    weapLayout->addWidget(radioX);
    weapLayout->addWidget(radioY);
    weapLayout->addWidget(radioZ);
    weapBox->setLayout(weapLayout);

    QTableWidget *gameBoard = new QTableWidget;
    gameBoard->setRowCount(5);
    gameBoard->setColumnCount(5);
    gameBoard->verticalHeader()->setVisible(false);
    gameBoard->horizontalHeader()->setVisible(false);
    gameBoard->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    gameBoard->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameBoard->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //gameBoard->resizeColumnsToContents();



    auto makeGuessButton = new QPushButton(tr("Make Guess"));

    auto charWeapBox = new QGroupBox;
    QVBoxLayout *charWeapLayout = new QVBoxLayout;
    charWeapLayout->addWidget(charBox);
    charWeapLayout->addWidget(weapBox);
    charWeapLayout->addWidget(makeGuessButton);

    charWeapBox->setLayout(charWeapLayout);

    auto guessBox = new QGroupBox(tr("Guess"));
    QHBoxLayout *guessLayout = new QHBoxLayout;
    // add make guess button to layout; later move to larger layout
    guessLayout->addWidget(charWeapBox);
    guessLayout->addWidget(gameBoard);
    guessBox->setLayout(guessLayout);




    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(hostCombo, &QComboBox::editTextChanged, this, &Client::enableJoinGameButton);
    connect(portLineEdit, &QLineEdit::textChanged, this, &Client::enableJoinGameButton);
    connect(joinGameButton, &QAbstractButton::clicked, this, &Client::joinGame);
    connect(sendMessageButton, &QAbstractButton::clicked, this, &Client::sendMessage);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::receiveMessage);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::displayError);

    QGridLayout *mainLayout = nullptr;
    //if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
    //    auto outerVerticalLayout = new QVBoxLayout(this);
    //    outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    //    auto outerHorizontalLayout = new QHBoxLayout;
    //    outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
    //    auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
    //    mainLayout = new QGridLayout(groupBox);
    //    outerHorizontalLayout->addWidget(groupBox);
    //    outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
    //    outerVerticalLayout->addLayout(outerHorizontalLayout);
    //    outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    //} else {
        mainLayout = new QGridLayout(this);
    //}
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostCombo, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(statusLabel, 2, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);
    mainLayout->addWidget(guessBox, 4, 0, 20, 100);

    setWindowTitle(QGuiApplication::applicationDisplayName());
    portLineEdit->setFocus();

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
        connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);

        joinGameButton->setEnabled(false);
        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    }
}

void Client::enableJoinGameButton()
{
    joinGameButton->setEnabled((!networkSession || networkSession->isOpen()) &&
                                 !hostCombo->currentText().isEmpty() &&
                                 !portLineEdit->text().isEmpty());

}

void Client::enableSendMessageButton()
{
    sendMessageButton->setEnabled(true);

}

void Client::sessionOpened()
{
    // Save the used configuration
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

    statusLabel->setText(tr("This examples requires that you run the "
                            "Game Server example as well."));

    enableJoinGameButton();
}

void Client::sendMessage()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    QString gameState = QString("Message from Player");
    out << gameState;
    tcpSocket->write(block);
    sendMessageButton->setEnabled(false);
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Game Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Game Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the game server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Game Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }

    joinGameButton->setEnabled(true);
}

void Client::joinGame()
{
    joinGameButton->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(hostCombo->currentText(), portLineEdit->text().toInt());
}

void Client::receiveMessage()
{
    in.startTransaction();

    //QString newMessage;
    //in >> newMessage;
    //newMessage.append("Message received");

    qint64 newMessage;
    in >> newMessage;

    //if (!in.commitTransaction()) // potential for error checking
    //    return;

    statusLabel->setText(tr("%1").arg(newMessage));
    sendMessageButton->setEnabled(true);
}
