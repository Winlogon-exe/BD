#include "viewForm.h"

ViewForm::ViewForm(QWidget *parent)
    : QWidget(parent)
{
    setupConnect();
    iniThread();
    createUI();
}

void ViewForm::iniThread()
{
    QThread* logicThread = new QThread(this);
    logic.moveToThread(logicThread);

    connect(logicThread, &QThread::started, &logic, &LogicView::s_initDB);
    connect(logicThread, &QThread::finished, logicThread, &QThread::deleteLater);
    logicThread->start();
}

void ViewForm::createUI()
{
    qDebug() << "Текущий поток View :" << QThread::currentThreadId();        
    setupDisplay();
    setupButtons();
    setupLineEdit();
    setupLabel();
    setupTableView();
    setupLayouts();
}

// настройка отображения основного окна.
void ViewForm::setupDisplay()
{
    setWindowTitle("DB");
    this->resize(800, 500);  
}

void ViewForm::setupLineEdit()
{
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("Поиск...");
}

void ViewForm::setupLabel()
{
    page = new QLabel("0");
}

//создание и настройка кнопок и других элементов управления.
void ViewForm::setupButtons()
{    
    filterComboBox = new QComboBox(this);

    nextButton = createButton(">",Next);
    backButton = createButton("<",Back);
    searchButton = createButton("Поиск",Search);

    QList<QPushButton*> allButton =
    {
        nextButton,backButton,searchButton
    };

    for(const auto& button:allButton)
    {
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }
}

void ViewForm::setupTableView()
{
    tableView = new QTableView(this);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

//настройка компоновки виджетов в окне.
void ViewForm::setupLayouts()
{
    mainLayout = new QVBoxLayout(this);

    // Layout для верхней части интерфейса, включая фильтр, поиск
    QHBoxLayout *topLayout = new QHBoxLayout();

    topLayout->addWidget(backButton);
    topLayout->addWidget(page);
    topLayout->addWidget(nextButton);

    topLayout->addWidget(searchLineEdit);
    topLayout->addWidget(searchButton);
    topLayout->addWidget(filterComboBox);

    mainLayout->addLayout(topLayout);

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(tableView, 0, 0, 1, 1);
    mainLayout->addLayout(gridLayout);
    this->setLayout(mainLayout);
}

QPushButton* ViewForm::createButton(const QString& text,StateButtonView state)
{
    QPushButton* button = new QPushButton(text, this);
    emit setState(button,state);  
    connect(button, &QPushButton::clicked, this, &ViewForm::s_buttonClicked);
    return button;
}

void ViewForm::paintSearch(const QString& text)
{
    delegate.reset(new HighlightDelegate(text));
    tableView->setItemDelegate(delegate.data());
}

void ViewForm::setupConnect()
{
    connect(this, &ViewForm::requestProcessState, &logic, &LogicView::s_processState);
    connect(this, &ViewForm::setState, &logic, &LogicView::s_setButtonState);

    connect(&logic,&LogicView::updateFilter,this,&ViewForm::s_onFieldsRetrieved);
    connect(&logic,&LogicView::updateLabel,this,&ViewForm::s_showLabel);
    connect(&logic,&LogicView::updateTable,this,&ViewForm::s_showTable);
}

//передача в логику
void ViewForm::s_buttonClicked()
{
    QString selectedFilter = filterComboBox->currentText();
    searchText = searchLineEdit->text();
   // paintSearch(searchText);
    emit requestProcessState(sender(),searchText,selectedFilter);
}

void ViewForm::s_showTable(QSharedPointer<QSqlQueryModel>model)
{
    tableView->setModel(model.data());
    tableView->show();
}

void ViewForm::s_onFieldsRetrieved(const QStringList &fields)
{
    filterComboBox->addItem("All");
    for (const QString &field : fields)
    {
        filterComboBox->addItem(field);
    }
}

void ViewForm::s_showLabel(int currentPage, int totalPages)
{
    pageInfo = QString("%1/%2").arg(currentPage).arg(totalPages);
    page->setText(pageInfo);
}

ViewForm::~ViewForm()
{
    logicThread->quit();
    logicThread->wait();
}
