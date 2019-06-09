//
// Created by cheesekun on 19/04/13.
//

#ifndef AZALEA_TIMELINEMODEL_HPP
#define AZALEA_TIMELINEMODEL_HPP

#include <QAbstractListModel>
#include "StatusAdapter.hpp"

/**
 * Timeline Model
 */
class TimelineModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    explicit TimelineModel(QObject *parent = nullptr);
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    
    void prepend(StatusAdapter *status);
    void append(StatusAdapter *status);
    void clear();
    
    int count();
    
    StatusAdapter *first();
    StatusAdapter *last();
    
    
private:
    QVector<StatusAdapter*> _statuses;
};


#endif //AZALEA_TIMELINEMODEL_HPP
