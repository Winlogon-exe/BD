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
#include <QToolBar>

#include "../delegate/highlightdelegate.h"
#include "../logic/logicView.h"

class ViewForm : public QWidget
{
    Q_OBJECT

public:
    explicit ViewForm(QWidget *parent = nullptr);
      ~ViewForm();

    void iniThread();
    void setupTableView();
    void  createUI();
    void  setupDisplay();
    void  setupButtons();
    void  setupLayouts();
    void  setupConnect();
    void paintSearch(const QString& text);
    QPushButton* createButton(const QString& text,State state);

signals:
    void  requestProcessState(QObject *sender, const QString &searchText,const QString filter);
    void  setState(QObject* button, State state);

public slots:
    void s_onFieldsRetrieved(const QStringList &fields);
    void s_showLabel(int currentPage, int totalPages);
    void s_showTable(QSharedPointer<QSqlQueryModel>model);
    void s_ButtonClicked();

private:
    QScopedPointer<HighlightDelegate> delegate;

    QThread*            logicThread;
    QVBoxLayout         *mainLayout;
    QTableView          *tableView;
    QString             searchText;
    QLabel              *page;
    LogicView            logic;

private:
    QString pageInfo;
    QPushButton* backButton;
    QPushButton* nextButton;
    QPushButton* searchButton;

    QLineEdit*   searchLineEdit;
    QComboBox*   filterComboBox;
};

#endif // VIEWFORM_H
