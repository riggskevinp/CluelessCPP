#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QtNetwork>

#include "player.h"

class QLabel;
class QTcpServer;
class QNetworkSession;
class QPushButton;

class Server : public QDialog
{
    Q_OBJECT
public:
    explicit Server(QWidget *parent = nullptr);
    qint64 encodeMessage(qint64 m_player, qint64 m_gA, qint64 m_char, qint64 m_loc, qint64 m_weap, qint64 m_col);
    void decodeMessage(qint64 newMes);
    void answer(qint64 m_message);
    qint64 playerTurn = 0;
    qint64 t_playerNumber;
    qint64 t_GA;
    qint64 t_character;
    qint64 t_weapon;
    qint64 t_row;
    qint64 t_col;
    qint64 solCharacter = 2;
    qint64 solWeapon = 3;
    qint64 solRow = 4;
    qint64 solCol = 2;

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
    void notifyAll();

};

#endif // SERVER_H
