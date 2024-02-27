#include "view.h"
#include<QHeaderView>
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

    //после дисплея - кнопки
    setupButtons();
}

//создание и настройка таблицы
void View::setupTableView()
{
    tableView = new QTableView(this);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //после БД - слои
    setupLayouts();
}

//настройка компоновки виджетов в окне.
void View::setupLayouts()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
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
    showTable();
}

//создание и настройка кнопок и других элементов управления.
void View::setupButtons()
{
    page = new QLabel("0");

    //поля для поиска
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("Введите запрос...");

    // Создание фильтра
    filterComboBox = new QComboBox(this);
    filterComboBox->addItem("Фильтр 1");
    filterComboBox->addItem("Фильтр 2");

    //возможно рефактор?
    // Создание и настройка кнопок
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
    //после кнопок - БД
    setupTableView();
}

QPushButton* View::createButton(const QString& text,State state)
{
    QPushButton* button = new QPushButton(text, this);
    button->setObjectName(text);
    logic.setButtonState(button,state);
    connect(button, &QPushButton::clicked, this, &View::ButtonClicked);
    return button;
}

void View::paintSearch()
{
    searchText = searchLineEdit->text();
    delegate = new HighlightDelegate(searchText);
    tableView->setItemDelegate(delegate);
}

void View::ButtonClicked()
{
    paintSearch();
    logic.processState(sender(),searchText);
}

//подключение сигналов и слотов для обработки событий.
void View::setupConnect()
{
    connect(&logic,&Logic::updateDB,this,&View::showTable);
    connect(&logic,&Logic::updateLabel,this,&View::showLabel);
}

//получать модель через геттер или параметр сиганала (как лучше?)
void View::showTable()
{
    QSqlQueryModel *model = logic.getModel();
    tableView->setModel(model);
    tableView->show();
}

void View::showLabel(int currentPage)
{
    page->setText(QString::number(currentPage));
}
