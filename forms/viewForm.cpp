#include "viewForm.h"

ViewForm::ViewForm(const QString& db,QWidget *parent)
    : QWidget(parent),
      nameBD(db)
{
    logic = std::make_unique<LogicView>(nameBD);
    createUI();
}

void ViewForm::createUI()
{
    qDebug() << "Текущий поток View :" << QThread::currentThreadId();

    iniThread();
    setupConnect();
    setupDisplay();
    setupButtons();
    setupLineEdit();
    setupLabel();
    setupTableView();
    setupLayouts();
}

void ViewForm::iniThread()
{
    logicThread = new QThread();
    logic->moveToThread(logicThread);

    connect(logicThread, &QThread::started, logic.get(), &LogicView::s_initDB);
    connect(logicThread, &QThread::finished, logicThread, &QThread::deleteLater);
    logicThread->start();
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
    connect(this, &ViewForm::requestProcessState, logic.get(), &LogicView::s_processState);
    connect(this, &ViewForm::setState, logic.get(), &LogicView::s_setButtonState);

    connect(logic.get(),&LogicView::updateFilter,this,&ViewForm::s_onFieldsRetrieved);
    connect(logic.get(),&LogicView::updateLabel,this,&ViewForm::s_showLabel);
    connect(logic.get(),&LogicView::updateTable,this,&ViewForm::s_showTable);
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
    qDebug()<<"деструктор ViewForm";
    logicThread->quit();
    logicThread->wait();
}
