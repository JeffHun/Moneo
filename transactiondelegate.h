#ifndef TRANSACTIONDELEGATE_H
#define TRANSACTIONDELEGATE_H

#include "transaction.h"

#include <QStyledItemDelegate>
#include <QPushButton>
#include <QComboBox>

class TransactionDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TransactionDelegate(QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget* editor, const QModelIndex &index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void deleteClicked(int row);
};

#endif // TRANSACTIONDELEGATE_H
