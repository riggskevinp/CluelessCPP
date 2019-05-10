#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QAbstractTableModel>
#include "datastore.h"
#include "room.h"

class BoardModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit BoardModel(DataStore* store, QObject *parent = nullptr);
	~BoardModel(){} // empty m_store

	// Header:

	DataStore* store(){return m_store;}
	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
	bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

private:
	DataStore* m_store;
};

#endif // BOARDMODEL_H
