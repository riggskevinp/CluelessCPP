#ifndef DATASTORE_H
#define DATASTORE_H

#include <QVector>
#include "room.h"

class DataStore
{
public:
	DataStore();
	~DataStore() // empty m_array
	{}
	void AddRoom(Room* room); // only in contructor?
	Room* GetRoom(int row, int col);
	//int rowCount(); //might not need
	//int columnCount(){return 4;} //might not need
private:
	QVector<QVector<Room*>> m_array;
};


#endif // DATASTORE_H
