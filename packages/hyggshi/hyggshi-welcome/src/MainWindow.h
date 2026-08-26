#pragma once
#include <QMainWindow>
#include <QStackedWidget>

class QLabel;
class QPushButton;

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void nextPage();
    void previousPage();
    void finish();

private:
    void buildUi();
    void updateButtons();

    QStackedWidget *pages_ = nullptr;
    QPushButton *backButton_ = nullptr;
    QPushButton *nextButton_ = nullptr;
    QPushButton *finishButton_ = nullptr;
};
