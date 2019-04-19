#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>

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
    qint64 playerNumber;
    qint64 t_playerNumber;
    qint64 t_GA;
    qint64 t_character;
    qint64 t_weapon;
    qint64 t_row;
    qint64 t_col;


private slots:
    void enableJoinGameButton();
    void enableSendMessageButton();
    void displayError(QAbstractSocket::SocketError socketError);
    void joinGame();
    void sessionOpened();
    void sendMessage(); // be able to click sendMessage after being notified by server
    void receiveMessage();

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

};

#endif // CLIENT_H
