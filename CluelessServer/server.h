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
    qint64 playerTurn = 0; // use this to verify whose turn it is, logic starts at 0 goes up to 5 and returns to 0
    qint64 t_playerNumber; // The clients use this field to sign who the message is coming from. If a client receives a message and this doesn't match, it knows it is just being notified about that clients guess
    qint64 t_GA; // Is this a guess: 0, or an accusation 1, can be set to 2 for other meanings
    qint64 t_character; // This is the character that is guessed or being sent 0-5
    qint64 t_weapon; // 0-5
    qint64 t_row; // 0-4  see board outline for this to make sense, board location (row,col) top left is (0,0)  bottom right is (4,4)
    qint64 t_col; // 0-4
    // todo, randomly generate the solutions upon construction of the server, do this in the server's constructor,
    // leaving this as known makes it easier to test
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
    QList<Player*> playerList;  // list of player objects
    QLabel *statusLabel = nullptr;
    QLabel *gameStatusLabel = nullptr;
    QTcpServer *tcpServer = nullptr;
    QNetworkSession *networkSession = nullptr;
    QDataStream in;
    QPushButton *sendMessageButton = nullptr;
    void notifyAll();

};

#endif // SERVER_H
