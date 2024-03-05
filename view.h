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
#include <QToolBar>
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
    void s_ButtonClicked();
    void s_showTable();
    void s_showLabel(int currentPage, int totalPages);

private:
    std::vector<QString> filters;
    QString             searchText;
    QTableView          *tableView;
    HighlightDelegate   *delegate;
    QLabel              *page;
    QVBoxLayout         *mainLayout;
    Logic               logic;

private:
    QPushButton* backButton;
    QPushButton* nextButton;
    QPushButton* searchButton;

    QLineEdit*   searchLineEdit;
    QComboBox*   filterComboBox;
};

#endif // VIEW_H
