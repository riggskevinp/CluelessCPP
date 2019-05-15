#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <QTableView>

#include "datastore.h"
#include "boardmodel.h"

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    void decodeMessage(qint64 newMes);
    qint64 encodeMessage(qint64 m_player, qint64 m_ga, qint64 m_char, qint64 m_weap, qint64 m_row, qint64 m_col);
    qint64 playerNumber = 6; // This lets the client know which player it is, it can use this to determine if a message is directed specifically for it
    qint64 i_playerNumber = 0; // same data as in the server
    qint64 i_GA = 0;
    qint64 i_character = 0;
    qint64 i_weapon = 0;
    qint64 i_row = 0;
    qint64 i_col = 0;



private slots:
    void enableJoinGameButton();
    void enableSendMessageButton();
    void displayError(QAbstractSocket::SocketError socketError);
    void joinGame();
    void sessionOpened();
    void sendMessage(); // be able to click sendMessage after being notified by server
    void receiveMessage();
    void suggest();
    void accuse();
	void onDoubleClicked();
    void redSelected(){i_character = 0;}
    void yellowSelected(){i_character = 1;}
    void whiteSelected(){i_character = 2;}
    void greenSelected(){i_character = 3;}
    void blueSelected(){i_character = 4;}
    void purpleSelected(){i_character = 5;}
    void ropeSelected(){i_weapon = 0;}
    void pipeSelected(){i_weapon = 1;}
    void knifeSelected(){i_weapon = 2;}
    void wrenchSelected(){i_weapon = 3;}
    void candlestickSelected(){i_weapon = 4;}
    void revolverSelected(){i_weapon = 5;}

private:
    QComboBox *hostCombo = nullptr;
    QLineEdit *portLineEdit = nullptr;
    QLabel *statusLabel = nullptr;
    QPushButton *joinGameButton = nullptr;
    QPushButton *sendMessageButton = nullptr;

    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QString currentFortune;

    QNetworkSession *networkSession = nullptr;

	DataStore* store;
	QTableView* tableView;
	BoardModel* boardModel;

};

#endif // CLIENT_H
