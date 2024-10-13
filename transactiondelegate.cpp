#include "transactiondelegate.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QComboBox>
#include <QDateEdit>
#include <QStyleOptionButton>
#include <QPainterPath>
#include <QFont>
#include <iostream>

TransactionDelegate::TransactionDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

QWidget* TransactionDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    if (index.column() == 0) // Date
    {
        QDateEdit* editor = new QDateEdit(parent);
        editor->setCalendarPopup(false);
        editor->setDisplayFormat("dd-MM-yyyy");
        return editor;
    }
    else if (index.column() == 3) // Category
    {
        QComboBox* editor = new QComboBox(parent);
        // Autorized category
        editor->addItems(Transaction::getAllowedCategories());

        return editor;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void TransactionDelegate::setEditorData(QWidget* editor, const QModelIndex &index) const
{
    if (index.column() == 0) // Date
    {
        QDateEdit* dateEdit = static_cast<QDateEdit*>(editor);
        QDate date = index.model()->data(index, Qt::EditRole).toDate();
        dateEdit->setDate(date);
    }
    else if (index.column() == 3) // Category
    {
        QComboBox* comboBox = static_cast<QComboBox*>(editor);
        QString currentText = index.model()->data(index, Qt::EditRole).toString();
        int idx = comboBox->findText(currentText);
        if (idx >= 0)
            comboBox->setCurrentIndex(idx);
    }
    else
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void TransactionDelegate::setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == 0) // Date
    {
        QDateEdit* dateEdit = static_cast<QDateEdit*>(editor);
        model->setData(index, dateEdit->date(), Qt::EditRole);
    }
    else if (index.column() == 3) // Category
    {
        QComboBox* comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::EditRole);
    }
    else
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void TransactionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 4) // Delete btn
    {
        QStyleOptionButton deleteBtn;
        deleteBtn.rect = option.rect.adjusted(4, 4, -4, -4);
        deleteBtn.text = "Delete";
        deleteBtn.palette.setColor(QPalette::Button, QColor("#6954D7"));
        deleteBtn.palette.setColor(QPalette::ButtonText, QColor("#D9D9D9"));

        // Style
        QPainterPath path;
        path.addRoundedRect(deleteBtn.rect, 10, 10);
        painter->fillPath(path, deleteBtn.palette.button());
        QFont font("Roboto", 10, QFont::Bold);
        painter->setFont(font);
        painter->setPen(deleteBtn.palette.buttonText().color());

        painter->drawText(deleteBtn.rect, Qt::AlignCenter, deleteBtn.text);
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

bool TransactionDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (index.column() == 4 && event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QRect rect = option.rect.adjusted(4, 4, -4, -4);
        if (rect.contains(mouseEvent->pos()))
        {
            emit deleteClicked(index.row());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
