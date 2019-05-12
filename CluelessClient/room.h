#ifndef ROOM_H
#define ROOM_H

#include <QString>
#include <QObject>

class Room
{
public:
	Room(const QString& name, const int& row, int& col, bool& player0, bool& player1
	     , bool& player2, bool& player3, bool& player4, bool& player5);
	~Room(){}
	inline QString name(){return m_name;}
	inline int getRow(){return m_row;}
	inline int getCol(){return m_col;}
	inline bool getPlayer0(){return m_player0;}
	inline bool getPlayer1(){return m_player1;}
	inline bool getPlayer2(){return m_player2;}
	inline bool getPlayer3(){return m_player3;}
	inline bool getPlayer4(){return m_player4;}
	inline bool getPlayer5(){return m_player5;}

	inline void setName(QString n){m_name = n;}
	inline void setRow(int r){m_row = r;}
	inline void setCol(int c){m_col = c;}
	inline void setPlayer0(bool p){m_player0 = p;}
	inline void setPlayer1(bool p){m_player1 = p;}
	inline void setPlayer2(bool p){m_player2 = p;}
	inline void setPlayer3(bool p){m_player3 = p;}
	inline void setPlayer4(bool p){m_player4 = p;}
	inline void setPlayer5(bool p){m_player5 = p;}
private:
	QString m_name;
	int m_row;
	int m_col;
	bool m_player0;
	bool m_player1;
	bool m_player2;
	bool m_player3;
	bool m_player4;
	bool m_player5;

};

#endif // ROOM_H
