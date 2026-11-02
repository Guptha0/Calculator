#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QListWidget>
#include <QDockWidget>
#include <QComboBox>
#include <QKeyEvent>

#include "engine/Calculator.hpp"
#include "engine/MemoryRegister.hpp"
#include "common/Exceptions.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onModeSelected(int index);
    void onStandardButtonClicked();
    void onScientificButtonClicked();
    void onProgrammerButtonClicked();
    void onHistoryItemClicked(QListWidgetItem *item);
    void clearAll();
    void backspace();
    void evaluateCurrent();

private:
    void setupUI();
    void setupDualDisplay(QVBoxLayout *layout);
    void setupKeypads();
    void setupHistoryDock();
    void applyTheme();

    QWidget* createStandardKeypad();
    QWidget* createScientificKeypad();
    QWidget* createProgrammerKeypad();

    QLabel *m_expressionDisplay;
    QLabel *m_primaryDisplay;

    QComboBox *m_modeSelector;
    QStackedWidget *m_keypadStack;
    QListWidget *m_historyList;

    Calculator m_calculator;
    MemoryRegister m_memory;

    std::string m_currentInput;
    bool m_resultState = false;
};
