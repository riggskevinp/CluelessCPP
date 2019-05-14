#include "boardmodel.h"
#include "datastore.h"
#include "room.h"

BoardModel::BoardModel(DataStore* store, QObject *parent) : m_store(store), QAbstractTableModel(parent)
{
}

void BoardModel::setPlayerNumber(int n_int)
{
	playerNumber = n_int;
}

QVariant BoardModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// FIXME: Implement me!
	/*
	if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		switch (section) {
			case 0:
				return QString("Name"); //column name be in datastore or ijdparameter?
			case 1:
				return QString("Date");
			case 2:
				return QString("Gender");
			case 3:
				return QString("Salary");
		}
	}*/
	return QVariant();
}

Qt::ItemFlags BoardModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool BoardModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid() && role == Qt::EditRole) {
		/*
		switch (index.column()){
		    case 0:
				m_store->parameter(index.row())->setName(value.toString());
				emit dataChanged(index, index, {role});
				return true;
			case 1:
			    m_store->parameter(index.row())->setDate(value.toDate());
				emit dataChanged(index, index, {role});
				return true;
			case 2:
			    m_store->parameter(index.row())->setGender(value.toChar());
				emit dataChanged(index, index, {role});
				return true;
			case 3:
			    m_store->parameter(index.row())->setSalary(value.toDouble());
				emit dataChanged(index, index, {role});
				return true;
		}*/
        // clean this up
		emit dataChanged(index, index, {role});
        return true;
    }
    return false;

}

bool BoardModel::insertRows(int position, int rows, const QModelIndex &index)
{
	beginInsertRows(QModelIndex(), position, position+rows-1);
	for (int row = 0; row < rows; ++row){
		double j = 100.00;
		QChar m = 'm';
		//m_store->AddRoom(new IJDParameter(QString("Wally"), QDate::currentDate(), m, j));
	}
	endInsertRows();
	return true;
}

void BoardModel::onDoubleClick(QModelIndex* index)
{
	if(playerNumber == 0){
		for(int row = 0; row < 5; row++){
			for(int col = 0; col < 5; col++){
				m_store->GetRoom(row,col)->setPlayer0(false);
			}
		}
		m_store->GetRoom(index->row(), index->column())->setPlayer0(true);

	}
	if(playerNumber == 1){
		for(int row = 0; row < 5; row++){
			for(int col = 0; col < 5; col++){
				m_store->GetRoom(row,col)->setPlayer1(false);
			}
		}
		m_store->GetRoom(index->row(), index->column())->setPlayer1(true);

	}
	if(playerNumber == 2){
		for(int row = 0; row < 5; row++){
			for(int col = 0; col < 5; col++){
				m_store->GetRoom(row,col)->setPlayer2(false);
			}
		}
		m_store->GetRoom(index->row(), index->column())->setPlayer2(true);

	}
	if(playerNumber == 3){
		for(int row = 0; row < 5; row++){
			for(int col = 0; col < 5; col++){
				m_store->GetRoom(row,col)->setPlayer3(false);
			}
		}
		m_store->GetRoom(index->row(), index->column())->setPlayer3(true);

	}
	if(playerNumber == 4){
		for(int row = 0; row < 5; row++){
			for(int col = 0; col < 5; col++){
				m_store->GetRoom(row,col)->setPlayer4(false);
			}
		}
		m_store->GetRoom(index->row(), index->column())->setPlayer4(true);

	}
	if(playerNumber == 5){
		for(int row = 0; row < 5; row++){
			for(int col = 0; col < 5; col++){
				m_store->GetRoom(row,col)->setPlayer5(false);
			}
		}
		m_store->GetRoom(index->row(), index->column())->setPlayer5(true);

	}
	emit dataChanged(QModelIndex(), QModelIndex());
	return;
}

int BoardModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;

	// FIXME: Implement me!
	return 5;//m_store->rowCount(); //st
}

int BoardModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;

	// FIXME: Implement me!
	return 5;//m_store->columnCount();
}

QVariant BoardModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
//	if (index.row() >= m_store->rowCount())
//        return QVariant();

	Room* room = m_store->GetRoom(index.row(), index.column());

	// Build up String to display:
	QString t_status = " \n";
	if(room->getPlayer0() == true){
		t_status.append(" Player0Red \n");
	}
	if(room->getPlayer1() == true){
		t_status.append(" Player1Yellow \n");
	}
	if(room->getPlayer2() == true){
		t_status.append(" Player2White \n");
	}
	if(room->getPlayer3() == true){
		t_status.append(" Player3Green \n");
	}
	if(room->getPlayer4() == true){
		t_status.append(" Player4Blue \n");
	}
	if(room->getPlayer5() == true){
		t_status.append(" Player5Purple \n");
	}
	if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return room->name().append(t_status);
    }

    return QVariant();
}
