#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QSqlQueryModel>
#include <QTableView>
#include <QSqlDatabase>
#include<QVBoxLayout>
#include <QtSql>
#include<QSqlTableModel>
#include <QSqlTableModel>
#include<QPushButton>
#include<QLineEdit>
#include"logic.h"

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


    void loadPage(int page);
    void showTable();
    QPushButton* createButton(const QString& text);

private slots:
    void ButtonClicked();

private:
    Logic logic;
    QTableView* tableView; // Виджет для отображения данных в виде таблицы

private:
    QPushButton *backButton;
    QPushButton *nextButton;
    QPushButton *searchButton;

    QLineEdit *searchLineEdit;
    QComboBox *filterComboBox;


    int page;
    int pageSize;
    int totalRecords;
};

#endif // VIEW_H
