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
