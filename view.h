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
#include<QLabel>
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
    QPushButton* createButton(const QString& text,State state);

private slots:
    void ButtonClicked();
    void showTable();

private:
    Logic logic;
    QTableView* tableView; // Виджет для отображения данных в виде таблицы

private:
    QPushButton* backButton;
    QPushButton* nextButton;
    QPushButton* searchButton;

    QLineEdit*   searchLineEdit;
    QComboBox*   filterComboBox;
    QLabel*      page;

};

#endif // VIEW_H
