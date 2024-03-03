#include "view.h"

View::View(QWidget *parent)
    : QWidget(parent)
{
    createUI();
}

void View::createUI()
{
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
    // кол-во страниц
    page = new QLabel("0");

    // поля для поиска
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("Введите запрос...");

    // Создание фильтра(поиск по фильтру)
    filterComboBox = new QComboBox(this);
    filterComboBox->addItem("default");
    filterComboBox->addItem("ID");
    filterComboBox->addItem("Name");

    // Создание и настройка кнопок
    //контейнер?
    nextButton = createButton("Вперед",Next);
    backButton = createButton("Назад",Back);
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
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(filterComboBox);
    topLayout->addWidget(searchLineEdit);
    topLayout->addWidget(searchButton);
    topLayout->addWidget(page);

    mainLayout->addLayout(topLayout);

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(tableView, 0, 0, 1, 1);
    mainLayout->addLayout(gridLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(nextButton);
    buttonLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(buttonLayout);
    this->setLayout(mainLayout);

    //показываем 0 страницу
    s_showTable();
}

QPushButton* View::createButton(const QString& text,State state)
{
    QPushButton* button = new QPushButton(text, this);
    logic.setButtonState(button,state);
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
    connect(&logic,&Logic::updateDB,this,&View::s_showTable);
    connect(&logic,&Logic::updateLabel,this,&View::s_showLabel);
}

//передача в логику
void View::s_ButtonClicked()
{
    paintSearch();
    logic.processState(sender(),searchText);
}

//вывод бд
void View::s_showTable()
{
    tableView->setModel(logic.getModel());
    tableView->show();
}

//вывод страниц
void View::s_showLabel(int currentPage)
{
    page->setText(QString::number(currentPage));
}
