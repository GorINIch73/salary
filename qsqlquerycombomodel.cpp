#include "qsqlquerycombomodel.h"
#include <QSqlQuery>
#include <QtDebug>

namespace
{
    enum Columns // Depends with 'query.prepare( QString( "SELECT ... '
    {
        Id,
        Data,
    };
}

QSqlQueryComboModel::QSqlQueryComboModel(const QString &visualColumn, const QString &queryTail, QSqlDatabase db, QObject *parent) :
    QSqlQueryModel( parent )
{
    QSqlQuery query(db);
    query.prepare( QString( "SELECT %1.id, %2 FROM %3" ).arg( queryTail.split( ' ' ).first() ).arg( visualColumn ).arg( queryTail ) );
//    query.prepare( QString( "SELECT %1.id, %2 FROM %3" ).arg( queryTail).arg( visualColumn ).arg( queryTail ) );
    // I.e. query.prepare( "SELECT country.id, countryname || ' - ' || countrycode  FROM country" );
    query.exec();

//    qDebug() << "0 " << query.value(0).toString();
//    qDebug() << "1 " << query.value(1).toString();

    QSqlQueryModel::setQuery( query );
}


QVariant QSqlQueryComboModel::dataFromParent(QModelIndex index, int column) const
{
    return QSqlQueryModel::data( QSqlQueryModel::index( index.row() - 1 // "- 1" because make first row empty
                                                        , column ) );
}



QVariant QSqlQueryComboModel::data(const QModelIndex &item, int role) const
{
    QVariant result;

    if( item.row() == 0 ) // Make first row empty
    {
        switch( role )
        {
            case Qt::UserRole:
                result = 0;
                break;
            case Qt::DisplayRole:
                result = "(please select)";
                break;
            default:
                break;
        }
    }
    else
    {
        switch( role )
        {
            case Qt::UserRole:
                result = dataFromParent( item, Id );
                break;
            case Qt::DisplayRole:
                result = dataFromParent( item, Data );
                break;
        case Qt::EditRole:
            result = dataFromParent( item, Data );
            break;
            default:
                break;
        }
    }

    return result;
}

int QSqlQueryComboModel::rowCount(const QModelIndex &parent) const
{
   return QSqlQueryModel::rowCount( parent ) + 1; // Add info about first empty row
}
