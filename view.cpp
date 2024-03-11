#include "view.h"

View::View(QWidget *parent)
    : QWidget(parent)
{
    iniThread();
    createUI();
}

void View::iniThread()
{
    QThread* logicThread = new QThread(this);
    logic.moveToThread(logicThread);

    connect(logicThread, &QThread::started, &logic, &Logic::initDB);
    connect(logicThread, &QThread::finished, logicThread, &QThread::deleteLater);
    logicThread->start();
}

void View::createUI()
{
    qDebug() << "Текущий поток View :" << QThread::currentThreadId();
    setupConnect();
    setupDisplay();
}

// настройка отображения основного окна.
void View::setupDisplay()
{
    setWindowTitle("DB");
    this->resize(800, 500);
    setupButtons();
}

//создание и настройка кнопок и других элементов управления.
void View::setupButtons()
{
    page = new QLabel("0");

    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("Поиск...");

    // Создание фильтра(поиск по фильтру)
    filterComboBox = new QComboBox(this);

    //!
    //onFieldsRetrieved(logic.getFields());

    // Создание и настройка кнопок
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

    setupTableView();
}

//создание и настройка таблицы
void View::setupTableView()
{
    tableView = new QTableView(this);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setupLayouts();
}

//настройка компоновки виджетов в окне.
void View::setupLayouts()
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

    //показываем 0 страницу
    s_showTable();
    s_showLabel(logic.currentPage,logic.totalPages);
}

QPushButton* View::createButton(const QString& text,State state)
{
    QPushButton* button = new QPushButton(text, this);
    emit setState(button,state);
    //logic.setButtonState(button,state);

    connect(button, &QPushButton::clicked, this, &View::s_ButtonClicked);
    return button;
}

//выделение текста
void View::paintSearch()
{
    searchText = searchLineEdit->text();
    delegate = new HighlightDelegate(searchText);
    tableView->setItemDelegate(delegate);
}

//подключение сигналов и слотов.
void View::setupConnect()
{
    connect(this, &View::requestProcessState, &logic, &Logic::processState);
    connect(this, &View::setState, &logic, &Logic::setButtonState);
    connect(&logic,&Logic::updateLabel,this,&View::s_showLabel);
    connect(&logic,&Logic::updateFilter,this,&View::onFieldsRetrieved);

}

//передача в логику
void View::s_ButtonClicked()
{
    QString selectedFilter = filterComboBox->currentText();
    paintSearch();
    emit requestProcessState(sender(),searchText,selectedFilter);
}

//вывод бд
void View::s_showTable()
{
    tableView->setModel(logic.getsqlModel());
    tableView->show();
}

void View::onFieldsRetrieved(const QStringList &fields)
{
    filterComboBox->addItem("All");
    for (const QString &field : fields)
    {
        filterComboBox->addItem(field);
    }
}

//вывод страниц
void View::s_showLabel(int currentPage, int totalPages)
{
    pageInfo = QString("%1/%2").arg(currentPage).arg(totalPages);
    page->setText(pageInfo);
}

View::~View()
{
    logicThread->quit();
    logicThread->wait();
}
