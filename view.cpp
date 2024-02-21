#include "view.h"

View::View(QWidget *parent)
    : QWidget(parent)
{
    logic  = new Logic;
    createUI();
}

void View::createUI()
{
    setupDisplay();
    setupButtons();
    setupTableView();
    setupLayouts();
    setupConnect();
}

// настройка отображения основного окна.
void View::setupDisplay()
{
    setWindowTitle("DB");
    this->resize(800, 500);
}

//создание и настройка таблицы
void View::setupTableView()
{
    tableView = new QTableView(this);
    tableView->setMinimumSize(500, 300);
}

//настройка компоновки виджетов в окне.
void View::setupLayouts()
{
    // Компоновка для верхнего поля ввода
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(filterComboBox);
    topLayout->addWidget(searchLineEdit);
    topLayout->addWidget(searchButton);
    mainLayout->addLayout(topLayout);

    // Компоновка с сеткой для таблицы
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(tableView, 0, 0, 1, 1);
    mainLayout->addLayout(gridLayout);

    // Компоновка для кнопок внизу
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(nextButton);
    buttonLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(buttonLayout);

    this->setLayout(mainLayout);
}

//создание и настройка кнопок и других элементов управления.
void View::setupButtons()
{
    //поля для поиска
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText(tr("Введите запрос..."));

    // Создание фильтра
    filterComboBox = new QComboBox(this);
    filterComboBox->addItem(tr("Фильтр 1"));
    filterComboBox->addItem(tr("Фильтр 2"));

    // Создание и настройка кнопок
    backButton = createButton(tr("Назад"));
    nextButton = createButton(tr("Вперед"));
    searchButton = createButton(tr("Поиск"));

    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    nextButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    searchButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

}

QPushButton* View::createButton(const QString& text)
{
    QPushButton* button = new QPushButton(text, this);
    button->setObjectName(text);
    connect(button, &QPushButton::clicked, this, &View::ButtonClicked);
    return button;
}

void View::ButtonClicked()
{

}

//подключение сигналов и слотов для обработки событий.
void View::setupConnect()
{
    connect(logic, &Logic::dataChanged, this, &View::showTable);
}

void View::showTable(QSqlQueryModel *model)
{
     qDebug() << "слот вызван";
    tableView->setModel(model);
    tableView->update();
    tableView->show();
}
