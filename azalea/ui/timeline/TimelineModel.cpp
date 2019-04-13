//
// Created by cheesekun on 19/04/13.
//

#include "TimelineModel.hpp"


TimelineModel::TimelineModel(QObject *parent) :
        QAbstractListModel(parent),
        _statuses()
{

}

int TimelineModel::rowCount(const QModelIndex &parent) const
{
    return _statuses.count();
}

QVariant TimelineModel::data(const QModelIndex &index, int role) const
{
    qDebug() << "data() " << index.row();
    if (!index.isValid() || index.row() < 0 || index.row() > rowCount()) {
        qDebug() << "returning null";
        return QVariant();
    }
    
    qDebug() << "returning status";
    return QVariant::fromValue<StatusAdapter*>(_statuses.at(index.row()));
}

Qt::ItemFlags TimelineModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void TimelineModel::append(StatusAdapter *status)
{
    qDebug() << "inserting " << rowCount() << " ~ " << rowCount();
    this->beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _statuses.append(status);
    this->endInsertRows();
}
