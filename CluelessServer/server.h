#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QtNetwork>

#include "player.h"
#include "CardDictionary.h"

class QLabel;
class QTcpServer;
class QNetworkSession;
class QPushButton;

class Server : public QDialog
{
    Q_OBJECT
public:
    explicit Server(QWidget *parent = nullptr);
    CardDictionary *cardDict = new CardDictionary();

private slots:
    void sessionOpened();
    void addPlayer();
    void sendMessage();
    void receiveMessage();

private:
    QList<Player*> playerList;
    QLabel *statusLabel = nullptr;
    QLabel *gameStatusLabel = nullptr;
    QTcpServer *tcpServer = nullptr;
    QNetworkSession *networkSession = nullptr;
    QDataStream in;
    QPushButton *sendMessageButton = nullptr;

};

#endif // SERVER_H
