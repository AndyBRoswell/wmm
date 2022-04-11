#include "QtTreeModel.h"

using namespace WritingMaterialsManager;

QtTreeModel::QtTreeModel(QObject* Parent)
    : QAbstractItemModel(Parent) {
}

QVariant QtTreeModel::headerData(int Section, Qt::Orientation Orientation, int Role) const {
    // FIXME: Implement me!
}

bool QtTreeModel::setHeaderData(int Section, Qt::Orientation Orientation, const QVariant& Value, int Role) {
    if (Value != headerData(Section, Orientation, Role)) {
        // FIXME: Implement me!
        emit headerDataChanged(Orientation, Section, Section);
        return true;
    }
    return false;
}

QModelIndex QtTreeModel::index(int Row, int Column, const QModelIndex& Parent) const {
    // FIXME: Implement me!
}

QModelIndex QtTreeModel::parent(const QModelIndex& Index) const {
    // FIXME: Implement me!
}

int QtTreeModel::rowCount(const QModelIndex& Parent) const {
    if (!Parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int QtTreeModel::columnCount(const QModelIndex& Parent) const {
    if (!Parent.isValid())
        return 0;

    // FIXME: Implement me!
}

bool QtTreeModel::hasChildren(const QModelIndex& Parent) const {
    // FIXME: Implement me!
}

bool QtTreeModel::canFetchMore(const QModelIndex& Parent) const {
    // FIXME: Implement me!
    return false;
}

void QtTreeModel::fetchMore(const QModelIndex& Parent) {
    // FIXME: Implement me!
}

QVariant QtTreeModel::data(const QModelIndex& Index, int Role) const {
    if (!Index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}

bool QtTreeModel::setData(const QModelIndex& Index, const QVariant& Value, int Role) {
    if (data(Index, Role) != Value) {
        // FIXME: Implement me!
        emit dataChanged(Index, Index, { Role });
        return true;
    }
    return false;
}

Qt::ItemFlags QtTreeModel::flags(const QModelIndex& Index) const {
    if (!Index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(Index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

bool QtTreeModel::insertRows(int Row, int Count, const QModelIndex& Parent) {
    beginInsertRows(Parent, Row, Row + Count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool QtTreeModel::insertColumns(int Column, int Count, const QModelIndex& Parent) {
    beginInsertColumns(Parent, Column, Column + Count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

bool QtTreeModel::removeRows(int Row, int Count, const QModelIndex& Parent) {
    beginRemoveRows(Parent, Row, Row + Count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

bool QtTreeModel::removeColumns(int Column, int Count, const QModelIndex& Parent) {
    beginRemoveColumns(Parent, Column, Column + Count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return true;
}
