#include "ui/MainWindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_currentInput("") {
    setupUI();
    applyTheme();
}

void MainWindow::setupUI() {
    setWindowTitle("Calculator Pro - Production Suite");
    resize(440, 620);

    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(12, 12, 12, 12);

    // Mode Selector
    m_modeSelector = new QComboBox(this);
    m_modeSelector->addItems({"Standard Mode", "Scientific Mode", "Programmer Mode"});
    connect(m_modeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onModeSelected);
    mainLayout->addWidget(m_modeSelector);

    // Dual display
    setupDualDisplay(mainLayout);

    // Keypads in a stacked layout
    setupKeypads();
    mainLayout->addWidget(m_keypadStack);

    setCentralWidget(centralWidget);

    // History Side Dock (loads from history.txt automatically)
    setupHistoryDock();
}

void MainWindow::setupDualDisplay(QVBoxLayout *layout) {
    m_expressionDisplay = new QLabel("", this);
    m_expressionDisplay->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_expressionDisplay->setObjectName("expressionDisplay");

    m_primaryDisplay = new QLabel("0", this);
    m_primaryDisplay->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_primaryDisplay->setObjectName("primaryDisplay");

    layout->addWidget(m_expressionDisplay);
    layout->addWidget(m_primaryDisplay);
}

void MainWindow::setupHistoryDock() {
    auto *dock = new QDockWidget("Calculation History (history.txt)", this);
    dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    m_historyList = new QListWidget(dock);
    dock->setWidget(m_historyList);

    // Populate past persistent records from history.txt
    for (const auto& rec : m_calculator.getHistory().getCachedRecords()) {
        m_historyList->addItem(QString::fromStdString(rec.expression + " = " + rec.result));
    }

    connect(m_historyList, &QListWidget::itemClicked, this, &MainWindow::onHistoryItemClicked);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

void MainWindow::setupKeypads() {
    m_keypadStack = new QStackedWidget(this);
    m_keypadStack->addWidget(createStandardKeypad());
    m_keypadStack->addWidget(createScientificKeypad());
    m_keypadStack->addWidget(createProgrammerKeypad());
}

QWidget* MainWindow::createStandardKeypad() {
    auto *widget = new QWidget();
    auto *grid = new QGridLayout(widget);
    grid->setSpacing(6);

    const std::vector<std::vector<QString>> buttons = {
        {"MC", "MR", "M+", "M-", "MS"},
        {"%",  "CE", "C",  "DEL", "/"},
        {"7",  "8",  "9",  "*",   "^"},
        {"4",  "5",  "6",  "-",   "("},
        {"1",  "2",  "3",  "+",   ")"},
        {"+/-","0",  ".",  "=",   ""}
    };

    for (int r = 0; r < static_cast<int>(buttons.size()); ++r) {
        for (int c = 0; c < static_cast<int>(buttons[r].size()); ++c) {
            QString text = buttons[r][c];
            if (text.isEmpty()) continue;

            auto *btn = new QPushButton(text);
            if (text == "=") {
                btn->setObjectName("equalsButton");
                grid->addWidget(btn, r, c, 1, 2);
            } else {
                grid->addWidget(btn, r, c);
            }
            connect(btn, &QPushButton::clicked, this, &MainWindow::onStandardButtonClicked);
        }
    }
    return widget;
}

QWidget* MainWindow::createScientificKeypad() {
    auto *widget = new QWidget();
    auto *grid = new QGridLayout(widget);
    grid->setSpacing(6);

    const std::vector<std::vector<QString>> buttons = {
        {"sin", "cos",  "tan",  "sqrt", "log"},
        {"ln",  "pi",   "e",    "^",    "%"},
        {"7",   "8",    "9",    "/",    "DEL"},
        {"4",   "5",    "6",    "*",    "C"},
        {"1",   "2",    "3",    "-",    "("},
        {"0",   ".",    "=",    "+",    ")"}
    };

    for (int r = 0; r < static_cast<int>(buttons.size()); ++r) {
        for (int c = 0; c < static_cast<int>(buttons[r].size()); ++c) {
            auto *btn = new QPushButton(buttons[r][c]);
            grid->addWidget(btn, r, c);
            connect(btn, &QPushButton::clicked, this, &MainWindow::onScientificButtonClicked);
        }
    }
    return widget;
}

QWidget* MainWindow::createProgrammerKeypad() {
    auto *widget = new QWidget();
    auto *grid = new QGridLayout(widget);
    grid->setSpacing(6);

    const std::vector<std::vector<QString>> buttons = {
        {"HEX", "DEC", "OCT", "BIN"},
        {"AND", "OR",  "XOR", "NOT"},
        {"LSH", "RSH", "C",   "DEL"},
        {"A",   "B",   "C_HEX","D"},
        {"E",   "F",   "+",   "-"},
        {"7",   "8",   "9",   "="},
        {"4",   "5",   "6",   "0"},
        {"1",   "2",   "3",   ""}
    };

    for (int r = 0; r < static_cast<int>(buttons.size()); ++r) {
        for (int c = 0; c < static_cast<int>(buttons[r].size()); ++c) {
            QString text = buttons[r][c];
            if (text.isEmpty()) continue;
            auto *btn = new QPushButton(text);
            grid->addWidget(btn, r, c);
            connect(btn, &QPushButton::clicked, this, &MainWindow::onProgrammerButtonClicked);
        }
    }
    return widget;
}

void MainWindow::onModeSelected(int index) {
    m_keypadStack->setCurrentIndex(index);
    if (index == 0) m_calculator.setMode(Calculator::ModeType::Standard);
    else if (index == 1) m_calculator.setMode(Calculator::ModeType::Scientific);
    else if (index == 2) m_calculator.setMode(Calculator::ModeType::Programmer);
    clearAll();
}

void MainWindow::clearAll() {
    m_currentInput.clear();
    m_expressionDisplay->setText("");
    m_primaryDisplay->setText("0");
    m_resultState = false;
}

void MainWindow::backspace() {
    if (!m_currentInput.empty()) {
        m_currentInput.pop_back();
        m_primaryDisplay->setText(m_currentInput.empty() ? "0" : QString::fromStdString(m_currentInput));
    }
}

void MainWindow::evaluateCurrent() {
    if (m_currentInput.empty()) return;

    try {
        double result = m_calculator.calculate(m_currentInput);
        std::string resultStr = std::to_string(result);
        
        // Strip trailing zeros
        if (resultStr.find('.') != std::string::npos) {
            resultStr.erase(resultStr.find_last_not_of('0') + 1, std::string::npos);
            if (resultStr.back() == '.') resultStr.pop_back();
        }

        m_expressionDisplay->setText(QString::fromStdString(m_currentInput + " ="));
        m_primaryDisplay->setText(QString::fromStdString(resultStr));

        // Add to history list widget
        m_historyList->addItem(QString::fromStdString(m_currentInput + " = " + resultStr));

        m_currentInput = resultStr;
        m_resultState = true;
    } catch (const CalculatorException& e) {
        m_primaryDisplay->setText("[Error]");
        m_expressionDisplay->setText(e.what());
        m_resultState = true;
    } catch (const std::exception& e) {
        m_primaryDisplay->setText("[Error]");
        m_expressionDisplay->setText(e.what());
        m_resultState = true;
    }
}

void MainWindow::onStandardButtonClicked() {
    auto *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    QString text = btn->text();

    if (text == "C" || text == "CE") {
        clearAll();
    } else if (text == "DEL") {
        backspace();
    } else if (text == "=") {
        evaluateCurrent();
    } else if (text == "MC") {
        m_memory.clear();
    } else if (text == "MR") {
        m_currentInput = std::to_string(m_memory.recall());
        m_primaryDisplay->setText(QString::fromStdString(m_currentInput));
    } else if (text == "M+") {
        try { m_memory.add(m_calculator.calculate(m_currentInput)); } catch (...) {}
    } else if (text == "M-") {
        try { m_memory.subtract(m_calculator.calculate(m_currentInput)); } catch (...) {}
    } else if (text == "MS") {
        try { m_memory.store(m_calculator.calculate(m_currentInput)); } catch (...) {}
    } else {
        if (m_resultState && (text[0].isDigit() || text == ".")) {
            m_currentInput.clear();
        }
        m_resultState = false;
        m_currentInput += text.toStdString();
        m_primaryDisplay->setText(QString::fromStdString(m_currentInput));
    }
}

void MainWindow::onScientificButtonClicked() {
    auto *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    QString text = btn->text();

    if (text == "C") {
        clearAll();
    } else if (text == "DEL") {
        backspace();
    } else if (text == "=") {
        evaluateCurrent();
    } else if (text == "sin" || text == "cos" || text == "tan" || text == "sqrt" || text == "log" || text == "ln") {
        m_currentInput += text.toStdString() + "(";
        m_primaryDisplay->setText(QString::fromStdString(m_currentInput));
    } else {
        m_currentInput += text.toStdString();
        m_primaryDisplay->setText(QString::fromStdString(m_currentInput));
    }
}

void MainWindow::onProgrammerButtonClicked() {
    auto *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    QString text = btn->text();
    auto& prog = m_calculator.getProgrammerMode();

    if (text == "C") {
        clearAll();
    } else if (text == "DEL") {
        backspace();
    } else if (text == "HEX") {
        m_primaryDisplay->setText(QString::fromStdString(prog.toHex()));
    } else if (text == "DEC") {
        m_primaryDisplay->setText(QString::fromStdString(prog.toDec()));
    } else if (text == "OCT") {
        m_primaryDisplay->setText(QString::fromStdString(prog.toOct()));
    } else if (text == "BIN") {
        m_primaryDisplay->setText(QString::fromStdString(prog.toBin()));
    } else if (text == "NOT") {
        prog.bitwiseNot();
        m_primaryDisplay->setText(QString::fromStdString(prog.toDec()));
    } else {
        m_currentInput += text.toStdString();
        m_primaryDisplay->setText(QString::fromStdString(m_currentInput));
    }
}

void MainWindow::onHistoryItemClicked(QListWidgetItem *item) {
    if (!item) return;
    QString text = item->text();
    QStringList parts = text.split(" = ");
    if (!parts.isEmpty()) {
        m_currentInput = parts.first().toStdString();
        m_primaryDisplay->setText(parts.first());
        m_expressionDisplay->setText("");
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    int key = event->key();

    if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        if (m_resultState) {
            m_currentInput.clear();
            m_resultState = false;
        }
        m_currentInput += std::to_string(key - Qt::Key_0);
        m_primaryDisplay->setText(QString::fromStdString(m_currentInput));
    } else if (key == Qt::Key_Period) {
        m_currentInput += ".";
        m_primaryDisplay->setText(QString::fromStdString(m_currentInput));
    } else if (key == Qt::Key_Plus) {
        m_resultState = false;
        m_currentInput += "+";
        m_primaryDisplay->setText(QString::fromStdString(m_currentInput));
    } else if (key == Qt::Key_Minus) {
        m_resultState = false;
        m_currentInput += "-";
        m_primaryDisplay->setText(QString::fromStdString(m_currentInput));
    } else if (key == Qt::Key_Asterisk) {
        m_resultState = false;
        m_currentInput += "*";
        m_primaryDisplay->setText(QString::fromStdString(m_currentInput));
    } else if (key == Qt::Key_Slash) {
        m_resultState = false;
        m_currentInput += "/";
        m_primaryDisplay->setText(QString::fromStdString(m_currentInput));
    } else if (key == Qt::Key_ParenLeft) {
        m_currentInput += "(";
        m_primaryDisplay->setText(QString::fromStdString(m_currentInput));
    } else if (key == Qt::Key_ParenRight) {
        m_currentInput += ")";
        m_primaryDisplay->setText(QString::fromStdString(m_currentInput));
    } else if (key == Qt::Key_Return || key == Qt::Key_Enter || key == Qt::Key_Equal) {
        evaluateCurrent();
    } else if (key == Qt::Key_Backspace) {
        backspace();
    } else if (key == Qt::Key_Escape) {
        clearAll();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::applyTheme() {
    QFile file(":/styles/dark_theme.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        setStyleSheet(styleSheet);
        file.close();
    }
}
