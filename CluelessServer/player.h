#ifndef PLAYER_H
#define PLAYER_H


#include <QtNetwork>

class Player
{
public:
    Player(QTcpSocket* socket);
    QTcpSocket* getSocket(void);

private:
    QTcpSocket *tcpClient = nullptr;

};

#endif // PLAYER_H
