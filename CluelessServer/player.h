#ifndef PLAYER_H
#define PLAYER_H


#include <QtNetwork>

class Player
{
public:
    // Simple player object that holds the client connections and allows the server to use access those connections for sending messages
    Player(QTcpSocket* socket);
    QTcpSocket* getSocket(void);

private:
    QTcpSocket *tcpClient = nullptr;

};

#endif // PLAYER_H
