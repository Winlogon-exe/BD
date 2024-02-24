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

    //после дисплея - кнопки
    setupButtons();
}

//создание и настройка таблицы
void View::setupTableView()
{
    tableView = new QTableView(this);
    tableView->setMinimumSize(500, 300);

    //после БД - слои
    setupLayouts();
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
    topLayout->addWidget(page);
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
    //показывем первую страницу при запуске
    showTable();
}

//создание и настройка кнопок и других элементов управления.
void View::setupButtons()
{
    page = new QLabel("0");

    //поля для поиска
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText(tr("Введите запрос..."));

    // Создание фильтра
    filterComboBox = new QComboBox(this);
    filterComboBox->addItem(tr("Фильтр 1"));
    filterComboBox->addItem(tr("Фильтр 2"));

    //возможно рефактор?
    // Создание и настройка кнопок
    nextButton = createButton(tr("Вперед"),Next);
    backButton = createButton(tr("Назад"),Back);
    searchButton = createButton(tr("Поиск"),Search);

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

void View::ButtonClicked()
{
    //свернуть
    QString searchText = searchLineEdit->text();
    delegate = new HighlightDelegate(searchText);
    tableView->setItemDelegate(delegate);

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
