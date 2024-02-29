#include "HighlightDelegate.h"

HighlightDelegate::HighlightDelegate(const QString& searchText, QObject *parent):
    QStyledItemDelegate(parent),
    m_searchText(searchText) {}

//возвращать строку надо?
void HighlightDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Вызываем базовую реализацию для отрисовки стандартного вида элемента.
    QStyledItemDelegate::paint(painter, option, index);

    // Если текст для поиска не пуст, начинаем поиск и подсветку совпадений.
    if (!m_searchText.isEmpty())
    {
        const QString text = index.data().toString();
        int startPos = text.indexOf(m_searchText, 0, Qt::CaseInsensitive);

        // Если нашли совпадение, подсвечиваем его.
        if (startPos >= 0)
        {
            painter->save(); // Сохраняем текущее состояние рисовальщика

            // Настройка стиля подсветки
            QColor highlightColor = QColor("red");
            highlightColor.setAlpha(75); // Делаем цвет полупрозрачным
            painter->setBrush(highlightColor);
            painter->setPen(Qt::NoPen); // Убираем контур

            // Вычисляем размеры текста для подсветки
            QFontMetrics metrics(painter->font());
            int textWidth = metrics.horizontalAdvance(m_searchText);
            int yOffset = (option.rect.height() - metrics.height()) / 2; // Выравнивание по вертикали

            // Рассчитываем положение и размер прямоугольника подсветки
            int textStartPos = metrics.horizontalAdvance(text.left(startPos))+5;
            QRect highlightRect(option.rect.left() + textStartPos, option.rect.top() + yOffset, textWidth, metrics.height());

            // Рисуем прямоугольник подсветки
            painter->fillRect(highlightRect, highlightColor);

            painter->restore(); // Восстанавливаем состояние рисовальщика
        }
    }
}
