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
    if (!index.isValid() || index.row() < 0 || index.row() > rowCount()) {
        return QVariant();
    }
    
    return QVariant::fromValue<StatusAdapter*>(_statuses.at(index.row()));
}

Qt::ItemFlags TimelineModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void TimelineModel::append(StatusAdapter *status)
{
    this->beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _statuses.append(status);
    this->endInsertRows();
}

void TimelineModel::clear()
{
    this->beginResetModel();
    _statuses.clear();
    this->endResetModel();
}
