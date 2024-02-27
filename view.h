#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QSqlQueryModel>
#include <QTableView>
#include <QSqlDatabase>
#include <QVBoxLayout>
#include <QtSql>
#include <QSqlTableModel>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHeaderView>

#include "highlightdelegate.h"
#include "logic.h"


class View : public QWidget
{
    Q_OBJECT

public:
   explicit View(QWidget *parent = nullptr);

    void createUI();
    void setupDisplay();
    void setupButtons();
    void setupTableView();
    void setupLayouts();
    void setupConnect();
    void paintSearch();

    QPushButton* createButton(const QString& text,State state);

private slots:
    void ButtonClicked();
    void showTable();
    void showLabel(int currentPage);

private:
    QString             searchText;
    QTableView*         tableView;
    HighlightDelegate*  delegate;
    QLabel*             page;
    Logic               logic;

private:
    QPushButton* backButton;
    QPushButton* nextButton;
    QPushButton* searchButton;

    QLineEdit*   searchLineEdit;
    QComboBox*   filterComboBox;
};

#endif // VIEW_H
