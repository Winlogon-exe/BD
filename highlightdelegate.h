#ifndef HIGHLIGHTDELEGATE_H
#define HIGHLIGHTDELEGATE_H

#include <QStyledItemDelegate>

class HighlightDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit HighlightDelegate(const QString& searchText, QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QString m_searchText;
};

#endif // HIGHLIGHTDELEGATE_H
