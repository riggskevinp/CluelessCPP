#include <QString>

#include "datastore.h"
#include "room.h"

DataStore::DataStore()
{
	m_array.resize(5);
	for(int i = 0; i < 5; ++i){
		m_array[i].resize(5);
	}
	bool player0, player1,player2,player3,player4,player5 = false;
	QString name = "Study";
	int row = 0;
	int col = 0;
	Room* study = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(study);

	name = "Hall";
	col = 2;
	Room* hall = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(hall);

	name = "Lounge";
	col = 4;
	Room* lounge = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(lounge);

	name = "Library";
	row = 2;
	col = 0;
	Room* library = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(library);

	name = "Billiard Room";
	col = 2;
	Room* billiard = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(billiard);

	name = "Dining Room";
	col = 4;
	Room* dining = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(dining);

	name = "Conservatory";
	row = 4;
	col = 0;
	Room* conservatory = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(conservatory);

	name = "Ballroom";
	col = 2;
	Room* ball = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(ball);

	name = "Kitchen";
	col = 4;
	Room* kitchen = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(kitchen);

	name = "path01";
	row = 0;
	col = 1;
	Room* path01 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(path01);

	name = "path03";
	row = 0;
	col = 3;
	Room* path03 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(path03);

	name = "path10";
	row = 1;
	col = 0;
	Room* path10 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(path10);

	name = "path12";
	row = 1;
	col = 2;
	Room* path12 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(path12);

	name = "path14";
	row = 1;
	col = 4;
	Room* path14 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(path14);

	name = "path21";
	row = 2;
	col = 1;
	Room* path21 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(path21);

	name = "path23";
	row = 2;
	col = 3;
	Room* path23 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(path23);

	name = "path30";
	row = 3;
	col = 0;
	Room* path30 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(path30);

	name = "path32";
	row = 3;
	col = 2;
	Room* path32 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(path32);

	name = "path34";
	row = 3;
	col = 4;
	Room* path34 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(path34);

	name = "path41";
	row = 4;
	col = 1;
	Room* path41 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(path41);

	name = "path43";
	row = 4;
	col = 3;
	Room* path43 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(path43);

	name = "blank11";
	row = 1;
	col = 1;
	Room* blank11 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(blank11);

	name = "blank13";
	row = 1;
	col = 3;
	Room* blank13 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(blank13);

	name = "blank31";
	row = 3;
	col = 1;
	Room* blank31 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(blank31);

	name = "blank33";
	row = 3;
	col = 3;
	Room* blank33 = new Room(name,row,col,player0,player1,player2,player3,player4,player5);
	AddRoom(blank33);
}

void DataStore::AddRoom(Room* room)
{
	m_array[room->getRow()][room->getCol()] = room;
}

Room *DataStore::GetRoom(int row, int col)
{
	return m_array[row][col];
}






