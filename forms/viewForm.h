#ifndef VIEWFORM_H
#define VIEWFORM_H

#include <QWidget>
#include <QTableWidget>
#include <QSqlQueryModel>
#include <QTableView>
#include <QSqlDatabase>
#include <QVBoxLayout>
#include <QSqlTableModel>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHeaderView>
#include <memory>
#include <QToolBar>

#include "../delegate/highlightdelegate.h"
#include "../logic/logicView.h"

class ViewForm : public QWidget
{
    Q_OBJECT

public:
    explicit ViewForm(const QString& db,const QString& table,QWidget *parent = nullptr);
     ~ViewForm();

    void iniThread();
    void createUI();
    void setupDisplay();
    void setupButtons();
    void setupLayouts();
    void setupConnect();
    void setupLineEdit();
    void setupLabel();
    void setupTableView();
    void paintSearch(const QString& text);

    QPushButton* createButton(const QString& text,StateButtonView state);

signals:
    void requestProcessState(QObject *sender, const QString &searchText,const QString filter);
    void setState(QObject* sender, StateButtonView state);

public slots:
    void s_showTable(QSharedPointer<QSqlQueryModel>model);
    void s_onFieldsRetrieved(const QStringList &fields);
    void s_showLabel(int currentPage, int totalPages);
    void s_buttonClicked();

private:
    QScopedPointer<HighlightDelegate> delegate;
    QPointer<QThread>   logicThread;
    QVBoxLayout         *mainLayout;
    QTableView          *tableView;
    QString             searchText;
    QLabel              *page;

    QLineEdit*   searchLineEdit;
    QComboBox*   filterComboBox;
    std::unique_ptr<LogicView> logic;

private:
    QString pageInfo;
    QString nameBD;
    QString nameTable;

private:
    QPushButton* backButton;
    QPushButton* nextButton;
    QPushButton* searchButton;  
};

#endif // VIEWFORM_H
