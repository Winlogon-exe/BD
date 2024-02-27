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

            QColor highlightColor = QColor("red").lighter(160);
            highlightColor.setAlpha(128);
            QBrush brush(highlightColor);
            painter->setBrush(brush);
            painter->setPen(Qt::NoPen);

            QFontMetrics metrics(painter->font());
            int textWidth = metrics.horizontalAdvance(m_searchText);
            int textHeight = metrics.ascent();

            // Выравнивание подсветки по горизонтали
            int textStartPos = metrics.horizontalAdvance(text.left(startPos)) + 3; // Добавляем отступ 5 пикселей к начальной позиции
            // Корректировка вертикальной позиции подсветки
            int yOffset = (rect.height() - textHeight) / 2; // Вычисление смещения по Y для выравнивания по центру

            QRect highlightRect(rect.left() + textStartPos, rect.top() + yOffset, textWidth, textHeight);

            painter->drawRect(highlightRect);

            painter->restore();
        }
    }
}
