#include "HighlightDelegate.h"
#include <QPainter>

HighlightDelegate::HighlightDelegate(const QString& searchText, QObject *parent)
    : QStyledItemDelegate(parent), m_searchText(searchText) {}

void HighlightDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index); // Отрисовка базового вида ячейки

    if (!m_searchText.isEmpty())
    {
        QString text = index.data().toString();
        int startPos = text.indexOf(m_searchText, 0, Qt::CaseInsensitive);
        if (startPos >= 0) {
            QRect rect = option.rect;
            painter->save();

            // Настройка внешнего вида подсветки с прозрачностью и смещением
            QColor highlightColor = QColor("yellow").lighter(160);
            highlightColor.setAlpha(128); // Установка прозрачности (0 - полностью прозрачный, 255 - полностью непрозрачный)
            QBrush brush(highlightColor);
            painter->setBrush(brush);
            painter->setPen(Qt::NoPen);

            // Вычисление размеров подсветки
            QFontMetrics metrics(painter->font());
            int width = metrics.horizontalAdvance(m_searchText);
            int height = metrics.height();
            int yOffset = 10; // Смещение вниз на 10 пикселя
            QRect highlightRect(rect.left() + metrics.horizontalAdvance(text.left(startPos)), rect.top() + yOffset, width, height);

            // Отрисовка подсветки
            painter->drawRect(highlightRect);

            painter->restore();
        }
    }
}
