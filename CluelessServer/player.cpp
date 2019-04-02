#include "player.h"

Player::Player(QTcpSocket* socket)
{
    this->tcpClient = socket;
}

QTcpSocket* Player::getSocket(void)
{
    return this->tcpClient;

}
