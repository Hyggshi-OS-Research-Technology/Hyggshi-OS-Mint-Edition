#include "MainWindow.h"

#include <QApplication>
#include <QCheckBox>
#include <QDir>
#include <QFile>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <QWidget>

namespace {
QWidget *makePage(const QString &title, const QString &body, QWidget *extra = nullptr) {
    auto *page = new QWidget;
    auto *layout = new QVBoxLayout(page);
    layout->setContentsMargins(40, 36, 40, 28);
    layout->setSpacing(16);

    auto *heading = new QLabel(title);
    heading->setObjectName("PageTitle");
    heading->setWordWrap(true);
    auto *text = new QLabel(body);
    text->setWordWrap(true);
    text->setObjectName("PageBody");
    layout->addWidget(heading);
    layout->addWidget(text);
    if (extra) {
        layout->addWidget(extra);
    }
    layout->addStretch();
    return page;
}
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle(QStringLiteral("Hyggshi Welcome"));
    resize(760, 500);
    setMinimumSize(680, 440);
    buildUi();
}

void MainWindow::buildUi() {
    auto *central = new QWidget;
    auto *root = new QVBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    auto *header = new QWidget;
    auto *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(28, 22, 28, 18);

    auto *logo = new QLabel;
    logo->setPixmap(QPixmap(QStringLiteral(":/images/logo.png")).scaled(52, 52, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    headerLayout->addWidget(logo);

    auto *titleBox = new QVBoxLayout;
    auto *title = new QLabel(QStringLiteral("Welcome to Hyggshi OS"));
    title->setObjectName("HeaderTitle");
    auto *subtitle = new QLabel(QStringLiteral("Let's set up your desktop experience."));
    subtitle->setObjectName("HeaderSubtitle");
    titleBox->addWidget(title);
    titleBox->addWidget(subtitle);
    headerLayout->addLayout(titleBox, 1);
    root->addWidget(header);

    pages_ = new QStackedWidget;

    pages_->addWidget(makePage(
        QStringLiteral("Welcome"),
        QStringLiteral("Hyggshi OS is a Linux Mint based desktop system customized for the Hyggshi ecosystem.\n\nThis assistant will guide you through a few optional preferences. You can skip everything and configure the system later.")));

    auto *appearance = new QWidget;
    auto *appearanceLayout = new QVBoxLayout(appearance);
    appearanceLayout->setContentsMargins(40, 36, 40, 28);
    auto *aTitle = new QLabel(QStringLiteral("Appearance"));
    aTitle->setObjectName("PageTitle");
    auto *aBody = new QLabel(QStringLiteral("Choose whether Hyggshi OS should follow your desktop's automatic light/dark preference."));
    aBody->setWordWrap(true);
    appearanceLayout->addWidget(aTitle);
    appearanceLayout->addWidget(aBody);
    auto *autoTheme = new QCheckBox(QStringLiteral("Enable automatic Hyggshi theme switching"));
    autoTheme->setChecked(true);
    appearanceLayout->addWidget(autoTheme);
    appearanceLayout->addStretch();
    pages_->addWidget(appearance);

    auto *privacy = new QCheckBox(QStringLiteral("Do not show this wizard automatically again"));
    privacy->setChecked(false);
    pages_->addWidget(makePage(
        QStringLiteral("Ready"),
        QStringLiteral("Hyggshi Welcome has finished. Your choices can be changed later from the Hyggshi settings tools."),
        privacy));

    root->addWidget(pages_, 1);

    auto *footer = new QWidget;
    auto *footerLayout = new QHBoxLayout(footer);
    footerLayout->setContentsMargins(28, 14, 28, 18);
    backButton_ = new QPushButton(QStringLiteral("Back"));
    nextButton_ = new QPushButton(QStringLiteral("Next"));
    finishButton_ = new QPushButton(QStringLiteral("Finish"));
    finishButton_->setDefault(true);
    footerLayout->addWidget(backButton_);
    footerLayout->addStretch();
    footerLayout->addWidget(nextButton_);
    footerLayout->addWidget(finishButton_);
    root->addWidget(footer);

    setCentralWidget(central);
    setStyleSheet(R"CSS(
        QMainWindow { background: #f7f8fa; }
        QWidget { font-family: sans-serif; font-size: 14px; }
        #HeaderTitle { font-size: 24px; font-weight: 700; }
        #HeaderSubtitle { color: #6b7280; }
        #PageTitle { font-size: 28px; font-weight: 700; }
        #PageBody { color: #374151; font-size: 15px; }
        QPushButton { padding: 8px 18px; border-radius: 7px; }
    )CSS");

    connect(backButton_, &QPushButton::clicked, this, &MainWindow::previousPage);
    connect(nextButton_, &QPushButton::clicked, this, &MainWindow::nextPage);
    connect(finishButton_, &QPushButton::clicked, this, &MainWindow::finish);
    updateButtons();
}

void MainWindow::updateButtons() {
    const int index = pages_->currentIndex();
    const int last = pages_->count() - 1;
    backButton_->setEnabled(index > 0);
    nextButton_->setVisible(index < last);
    finishButton_->setVisible(index == last);
}

void MainWindow::nextPage() {
    if (pages_->currentIndex() < pages_->count() - 1) {
        pages_->setCurrentIndex(pages_->currentIndex() + 1);
        updateButtons();
    }
}

void MainWindow::previousPage() {
    if (pages_->currentIndex() > 0) {
        pages_->setCurrentIndex(pages_->currentIndex() - 1);
        updateButtons();
    }
}

void MainWindow::finish() {
    const QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(configDir);
    QFile marker(configDir + QStringLiteral("/welcome-shown"));
    marker.open(QIODevice::WriteOnly | QIODevice::Truncate);
    marker.write("hyggshi-welcome\n");
    marker.close();
    QApplication::quit();
}
