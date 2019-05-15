#include <QtWidgets>
#include <QtNetwork>
#include <QGroupBox>

#include "client.h"
#include "datastore.h"
#include "boardmodel.h"
#include "room.h"

Client::Client(QWidget *parent)
    : QDialog(parent)
    , hostCombo(new QComboBox)
    , portLineEdit(new QLineEdit)
    , joinGameButton(new QPushButton(tr("Join Game")))
    , sendMessageButton(new QPushButton(tr("Send Message")))
    , tcpSocket(new QTcpSocket(this))
	, store(new DataStore())
	, boardModel(new BoardModel(store))
	, tableView(new QTableView)
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

    // This is creating a groupbox with the character radio buttons
    auto charBox = new QGroupBox(tr("Characters"));
    QGridLayout *charLayout = new QGridLayout;
    // when we get a "card" emit a signal that disables the respective button
    QRadioButton *radioRed = new QRadioButton(tr("Red"));
    connect(radioRed, &QAbstractButton::clicked, this, &Client::redSelected);
    QRadioButton *radioYellow = new QRadioButton(tr("Yellow"));
    connect(radioYellow, &QAbstractButton::clicked, this, &Client::yellowSelected);
    QRadioButton *radioWhite = new QRadioButton(tr("White"));
    connect(radioWhite, &QAbstractButton::clicked, this, &Client::whiteSelected);
    QRadioButton *radioGreen = new QRadioButton(tr("Green"));
    connect(radioGreen, &QAbstractButton::clicked, this, &Client::greenSelected);
    QRadioButton *radioBlue = new QRadioButton(tr("Blue"));
    connect(radioBlue, &QAbstractButton::clicked, this, &Client::blueSelected);
    QRadioButton *radioPurple = new QRadioButton(tr("Purple"));
    connect(radioPurple, &QAbstractButton::clicked, this, &Client::purpleSelected);
    charLayout->addWidget(radioRed);
    charLayout->addWidget(radioYellow);
    charLayout->addWidget(radioWhite);
    charLayout->addWidget(radioGreen);
    charLayout->addWidget(radioBlue);
    charLayout->addWidget(radioPurple);
    charBox->setLayout(charLayout);

    // This is creating a groupbox with the weapon radio buttons
    auto weapBox = new QGroupBox(tr("Weapons"));
    QGridLayout *weapLayout = new QGridLayout;
    // when we get a "card" emit a signal that disables the respective button
    QRadioButton *radioRope = new QRadioButton(tr("Rope"));
    connect(radioRope, &QAbstractButton::clicked, this, &Client::ropeSelected);
    QRadioButton *radioPipe = new QRadioButton(tr("Pipe"));
    connect(radioPipe, &QAbstractButton::clicked, this, &Client::pipeSelected);
    QRadioButton *radioKnife = new QRadioButton(tr("Knife"));
    connect(radioKnife, &QAbstractButton::clicked, this, &Client::knifeSelected);
    QRadioButton *radioWrench = new QRadioButton(tr("Wrench"));
    connect(radioWrench, &QAbstractButton::clicked, this, &Client::wrenchSelected);
    QRadioButton *radioCandlestick = new QRadioButton(tr("Candlestick"));
    connect(radioCandlestick, &QAbstractButton::clicked, this, &Client::candlestickSelected);
    QRadioButton *radioRevolver = new QRadioButton(tr("Revolver"));
    connect(radioRevolver, &QAbstractButton::clicked, this, &Client::revolverSelected);
    weapLayout->addWidget(radioRope);
    weapLayout->addWidget(radioPipe);
    weapLayout->addWidget(radioKnife);
    weapLayout->addWidget(radioWrench);
    weapLayout->addWidget(radioCandlestick);
    weapLayout->addWidget(radioRevolver);
    weapBox->setLayout(weapLayout);

    // this sets up the gameboard
    QTableWidget *gameBoard = new QTableWidget;
    gameBoard->setRowCount(5);
    gameBoard->setColumnCount(5);
    gameBoard->verticalHeader()->setVisible(false);
    gameBoard->horizontalHeader()->setVisible(false);
    gameBoard->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    gameBoard->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameBoard->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //gameBoard->resizeColumnsToContents();

    // !!! BIG TODO !!!
    // We need to create roomobjects to populate the gameboard with
    // See https://github.com/riggskevinp/CluelessCPP/issues/1
    //DataStore* store = new DataStore();
    //QTableView* tableView = new QTableView;
	//BoardModel* boardModel = new BoardModel(store);
    tableView->setModel(boardModel);
	tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	tableView->horizontalHeader()->setVisible(false);
	tableView->setColumnWidth(0,110);
	tableView->setColumnWidth(1,110);
	tableView->setColumnWidth(2,110);
	tableView->setColumnWidth(3,110);
	tableView->setColumnWidth(4,110);
	tableView->setRowHeight(0,110);
	tableView->setRowHeight(1,110);
	tableView->setRowHeight(2,110);
	tableView->setRowHeight(3,110);
	tableView->setRowHeight(4,110);
	connect(tableView, &QAbstractItemView::doubleClicked, this, &Client::onDoubleClicked);

    // create a Guess button and connect it to the makeGuess slot
    auto makeSuggestButton = new QPushButton(tr("Suggest"));
    connect(makeSuggestButton, &QAbstractButton::clicked, this, &Client::suggest);

    // create a Accuse button and connect it to the accuse slot
    auto makeAccuseButton = new QPushButton(tr("Accuse"));
    connect(makeAccuseButton, &QAbstractButton::clicked, this, &Client::accuse);

    // A groupbox is created to hold both the character and weapon boxes
    // It also holds the makeGuessButton
    auto charWeapBox = new QGroupBox;
    QVBoxLayout *charWeapLayout = new QVBoxLayout;
    charWeapLayout->addWidget(charBox);
    charWeapLayout->addWidget(weapBox);
    charWeapLayout->addWidget(makeSuggestButton);
    charWeapLayout->addWidget(makeAccuseButton);

    charWeapBox->setLayout(charWeapLayout);

    auto guessBox = new QGroupBox(tr("Guess"));
    QHBoxLayout *guessLayout = new QHBoxLayout;
    // Character Weapon box with make guess button added with gameBoard
    guessLayout->addWidget(charWeapBox);
    //guessLayout->addWidget(gameBoard);
    guessLayout->addWidget(tableView);
    guessBox->setLayout(guessLayout);



    //we only connect to one server
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    // See the connection of some signals and slots
    connect(hostCombo, &QComboBox::editTextChanged, this, &Client::enableJoinGameButton);
    connect(portLineEdit, &QLineEdit::textChanged, this, &Client::enableJoinGameButton);
    connect(joinGameButton, &QAbstractButton::clicked, this, &Client::joinGame);
    connect(sendMessageButton, &QAbstractButton::clicked, this, &Client::sendMessage);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::receiveMessage);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::displayError);

    // This is where the main layout is put together
    //Commented out this if statement to see if it getting run, it isn't
    //I think this was checking to see if it was maximized or fullscreen
    //We may want to test this more, to see what happens when we maximize and resize with and without this statement
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

void Client::decodeMessage(qint64 newMes)
{
    /*
     * Takes an interger, ideally a properly formatted message
     * and sets the global variables accordingly
     *
     * This is not the proper way to do this, but I don't think we have time to do it perfectly
     */
    i_col = newMes & 0xF;
    i_row = (newMes >> 4) & 0xF;
    i_weapon = (newMes >> 8) & 0xF;
    i_character = (newMes >> 12) & 0xF;
    i_GA = (newMes >> 16) & 0xF;
    i_playerNumber = (newMes >> 20) & 0xF;
}

qint64 Client::encodeMessage(qint64 m_player, qint64 m_ga, qint64 m_char, qint64 m_weap, qint64 m_row, qint64 m_col)
{
    /*
     * Takes integers and encodes them in the format to send to the server
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

void Client::enableJoinGameButton()
{
    // Should probably just stay as is, makes sure the joingamebutton is enabled
    // We can look closer at the logic to see if we can get rid of this, ie currently I think a client can connect to the server multiple times and the server will treat it as multiple players.
    joinGameButton->setEnabled((!networkSession || networkSession->isOpen()) &&
                                 !hostCombo->currentText().isEmpty() &&
                                 !portLineEdit->text().isEmpty());

}

void Client::enableSendMessageButton()
{
    //This can probably go away
    //However, notice that yout can enable and disable buttons, this slot was necessary because the button is private
    sendMessageButton->setEnabled(true);

}

void Client::sessionOpened()
{
    /*
     * From Qt client server example, we probably don't need to modify this
     * This gets run when you start the application
     */

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
    /*
     * This sends the current values of the global variables to the server as a message
     *
     * Can be useful for troubleshooting but we will probably want to take this away
     * It is connected to the sendMessage button
     *
     * Make Guess and Make Accusation buttons remove the need for this
     * Possibly refactor this into a Make Accusation - Also refactor send message button
     * Make accusation might come later, might not make it into final release
     */
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    qint64 message = encodeMessage(i_playerNumber,i_GA,i_character,i_weapon,i_row,i_col);
    out << message;

    tcpSocket->write(block);
    sendMessageButton->setEnabled(false);
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    /*
     * This comes from the Qt client server example
     *
     * This is error handling for when the client fails to connect to the host
     */
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
    /*
     * Connects the client to the host
     * Probably doesn't need changes
     */
    joinGameButton->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(hostCombo->currentText(), portLineEdit->text().toInt());
}

void Client::receiveMessage()
{
    /*
     * This message takes a message, decodes it and sets the status label to the results
     *
     * We can turn the status label into a scrolling text box in order to log messages.
     */
    in.startTransaction();

    qint64 newMessage;
    in >> newMessage;
    decodeMessage(newMessage);

	if(playerNumber == 6){
		playerNumber = i_playerNumber;
		boardModel->setPlayerNumber(i_playerNumber);
        }
    if(i_GA == 3){
        QMessageBox::information(this, tr("Game Over"), tr("Game has Ended. Player %1 has won.").arg(i_playerNumber));
    }

    statusLabel->setText(tr("Player%1 GA%2 Char%3 Weap%4 Row%5 Col%6").arg(i_playerNumber).arg(i_GA).arg(i_character).arg(i_weapon).arg(i_row).arg(i_col));
    sendMessageButton->setEnabled(true);
}

void Client::suggest()
{
    /*
     * This takes what the global variables are set to and sends that to the server
     * This is connected to the suggest button
     */
    i_GA = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    //Determine location

    qint64 message = encodeMessage(playerNumber,i_GA,i_character,i_weapon,i_row,i_col);
    out << message;

    tcpSocket->write(block);
}
void Client::accuse()
{
    /*
     * This takes what the global variables are set to and sends that to the server
     * This is connected to the accuse button
     */
    i_GA = 1;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    //Determine location

    qint64 message = encodeMessage(playerNumber,i_GA,i_character,i_weapon,i_row,i_col);
    out << message;

	tcpSocket->write(block);
}

void Client::onDoubleClicked()
{
	QModelIndex ind = tableView->currentIndex();
	i_row = ind.row();
	i_col = ind.column();
	boardModel->onDoubleClick(&ind);
}
