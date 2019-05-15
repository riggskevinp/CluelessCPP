#include "room.h"

Room::Room(const QString& name, const int& row, int& col, bool& player0, bool& player1, bool& player2, bool& player3, bool& player4, bool& player5)
	: m_name(name), m_row(row), m_col(col), m_player0(player0), m_player1(player1)
           , m_player2(player2)
           , m_player3(player3)
           , m_player4(player4)
           , m_player5(player5)
{

}
