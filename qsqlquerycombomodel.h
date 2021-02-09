#ifndef QSQLQUERYCOMBOMODEL_H
#define QSQLQUERYCOMBOMODEL_H

#include <QSqlQueryModel>
#include <QSqlDatabase>


class QSqlQueryComboModel : public QSqlQueryModel
{
    Q_OBJECT
    QVariant dataFromParent(QModelIndex index, int column) const;

public:
    explicit QSqlQueryComboModel(const QString &visualColumn, const QString &queryTail, QSqlDatabase db, QObject *parent = 0 );

    virtual QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent) const;



};

#endif // QSQLQUERYCOMBOMODEL_H
