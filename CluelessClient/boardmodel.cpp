#include "boardmodel.h"
#include "datastore.h"
#include "room.h"

BoardModel::BoardModel(DataStore* store, QObject *parent) : m_store(store), QAbstractTableModel(parent)
{
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

	// FIXME: Implement me!
	/*
	if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
			case 0:
				return room->name();
			case 1:
				return room->date();
			case 2:
				return room->gender();
			case 3:
				return room->salary();
		}
    }*/
	if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return room->name();
    }

    return QVariant();
}
