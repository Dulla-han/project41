/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoadVideo;
    QAction *actionLoadFrames;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QWidget *resultsPanel;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *ButtonHorizontalLayout;
    QPushButton *previewButton;
    QPushButton *benchmarkButton;
    QStackedWidget *contentStack;
    QWidget *previewPage;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *previewScrollArea;
    QWidget *previewRowsContainer;
    QVBoxLayout *verticalLayout_4;
    QWidget *algorithmPreview1;
    QHBoxLayout *horizontalLayout;
    QLabel *rifeLabel;
    QLabel *startFrameLabel;
    QLabel *generatedFrameLabel;
    QLabel *endFrameLabel;
    QSpacerItem *verticalSpacer_2;
    QWidget *benchmarkPage;
    QWidget *controlsPanel;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QWidget *settingsPanel;
    QPushButton *runButton;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuLoad_Input;
    QMenu *menuSettings;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        MainWindow->setAutoFillBackground(false);
        actionLoadVideo = new QAction(MainWindow);
        actionLoadVideo->setObjectName("actionLoadVideo");
        actionLoadVideo->setEnabled(false);
        actionLoadFrames = new QAction(MainWindow);
        actionLoadFrames->setObjectName("actionLoadFrames");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        resultsPanel = new QWidget(centralwidget);
        resultsPanel->setObjectName("resultsPanel");
        resultsPanel->setMinimumSize(QSize(50, 50));
        resultsPanel->setMaximumSize(QSize(16777215, 16777215));
        resultsPanel->setStyleSheet(QString::fromUtf8("QWidget#resultsPanel {\n"
"    background-color: #e5efdf;\n"
"    border: 1px solid #779966;\n"
"}"));
        verticalLayout = new QVBoxLayout(resultsPanel);
        verticalLayout->setObjectName("verticalLayout");
        ButtonHorizontalLayout = new QHBoxLayout();
        ButtonHorizontalLayout->setObjectName("ButtonHorizontalLayout");
        previewButton = new QPushButton(resultsPanel);
        previewButton->setObjectName("previewButton");

        ButtonHorizontalLayout->addWidget(previewButton);

        benchmarkButton = new QPushButton(resultsPanel);
        benchmarkButton->setObjectName("benchmarkButton");

        ButtonHorizontalLayout->addWidget(benchmarkButton);


        verticalLayout->addLayout(ButtonHorizontalLayout);

        contentStack = new QStackedWidget(resultsPanel);
        contentStack->setObjectName("contentStack");
        contentStack->setStyleSheet(QString::fromUtf8("QStackedWidget#contentStack {\n"
"	background-color: rgb(255, 255, 255);\n"
"}"));
        previewPage = new QWidget();
        previewPage->setObjectName("previewPage");
        previewPage->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_3 = new QVBoxLayout(previewPage);
        verticalLayout_3->setObjectName("verticalLayout_3");
        previewScrollArea = new QScrollArea(previewPage);
        previewScrollArea->setObjectName("previewScrollArea");
        previewScrollArea->setStyleSheet(QString::fromUtf8(""));
        previewScrollArea->setWidgetResizable(true);
        previewRowsContainer = new QWidget();
        previewRowsContainer->setObjectName("previewRowsContainer");
        previewRowsContainer->setGeometry(QRect(0, 0, 588, 400));
        verticalLayout_4 = new QVBoxLayout(previewRowsContainer);
        verticalLayout_4->setObjectName("verticalLayout_4");
        algorithmPreview1 = new QWidget(previewRowsContainer);
        algorithmPreview1->setObjectName("algorithmPreview1");
        algorithmPreview1->setMinimumSize(QSize(0, 130));
        algorithmPreview1->setMaximumSize(QSize(16777215, 130));
        horizontalLayout = new QHBoxLayout(algorithmPreview1);
        horizontalLayout->setObjectName("horizontalLayout");
        rifeLabel = new QLabel(algorithmPreview1);
        rifeLabel->setObjectName("rifeLabel");
        rifeLabel->setMinimumSize(QSize(80, 0));
        rifeLabel->setMaximumSize(QSize(80, 16777215));
        rifeLabel->setFrameShape(QFrame::Shape::Box);

        horizontalLayout->addWidget(rifeLabel);

        startFrameLabel = new QLabel(algorithmPreview1);
        startFrameLabel->setObjectName("startFrameLabel");
        startFrameLabel->setMinimumSize(QSize(120, 90));
        startFrameLabel->setFrameShape(QFrame::Shape::Box);
        startFrameLabel->setAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignTop);

        horizontalLayout->addWidget(startFrameLabel);

        generatedFrameLabel = new QLabel(algorithmPreview1);
        generatedFrameLabel->setObjectName("generatedFrameLabel");
        generatedFrameLabel->setMinimumSize(QSize(120, 90));
        generatedFrameLabel->setFrameShape(QFrame::Shape::Box);
        generatedFrameLabel->setAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignTop);

        horizontalLayout->addWidget(generatedFrameLabel);

        endFrameLabel = new QLabel(algorithmPreview1);
        endFrameLabel->setObjectName("endFrameLabel");
        endFrameLabel->setMinimumSize(QSize(120, 90));
        endFrameLabel->setFrameShape(QFrame::Shape::Box);
        endFrameLabel->setAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignTop);

        horizontalLayout->addWidget(endFrameLabel);

        horizontalLayout->setStretch(1, 1);
        horizontalLayout->setStretch(2, 1);
        horizontalLayout->setStretch(3, 1);

        verticalLayout_4->addWidget(algorithmPreview1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        previewScrollArea->setWidget(previewRowsContainer);

        verticalLayout_3->addWidget(previewScrollArea);

        contentStack->addWidget(previewPage);
        benchmarkPage = new QWidget();
        benchmarkPage->setObjectName("benchmarkPage");
        contentStack->addWidget(benchmarkPage);

        verticalLayout->addWidget(contentStack);


        gridLayout->addWidget(resultsPanel, 0, 1, 1, 1);

        controlsPanel = new QWidget(centralwidget);
        controlsPanel->setObjectName("controlsPanel");
        controlsPanel->setMinimumSize(QSize(150, 50));
        controlsPanel->setMaximumSize(QSize(150, 16777215));
        controlsPanel->setStyleSheet(QString::fromUtf8("QWidget#controlsPanel {\n"
"    background-color: #dce8f5;\n"
"    border: 1px solid #6688aa;\n"
"}"));
        verticalLayout_2 = new QVBoxLayout(controlsPanel);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        gridLayout->addWidget(controlsPanel, 0, 0, 1, 1);

        settingsPanel = new QWidget(centralwidget);
        settingsPanel->setObjectName("settingsPanel");
        settingsPanel->setEnabled(true);
        settingsPanel->setMinimumSize(QSize(50, 50));
        settingsPanel->setStyleSheet(QString::fromUtf8("QWidget#settingsPanel {\n"
"	background-color: rgb(192, 28, 40);\n"
"}"));

        gridLayout->addWidget(settingsPanel, 1, 1, 1, 1);

        runButton = new QPushButton(centralwidget);
        runButton->setObjectName("runButton");
        runButton->setMinimumSize(QSize(50, 50));

        gridLayout->addWidget(runButton, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuLoad_Input = new QMenu(menuFile);
        menuLoad_Input->setObjectName("menuLoad_Input");
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName("menuSettings");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuSettings->menuAction());
        menuFile->addAction(menuLoad_Input->menuAction());
        menuLoad_Input->addAction(actionLoadVideo);
        menuLoad_Input->addAction(actionLoadFrames);

        retranslateUi(MainWindow);

        contentStack->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionLoadVideo->setText(QCoreApplication::translate("MainWindow", "Load Video (mp4, gif)", nullptr));
#if QT_CONFIG(shortcut)
        actionLoadVideo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionLoadFrames->setText(QCoreApplication::translate("MainWindow", "Load Frames (png, jpg)", nullptr));
#if QT_CONFIG(shortcut)
        actionLoadFrames->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+I", nullptr));
#endif // QT_CONFIG(shortcut)
        previewButton->setText(QCoreApplication::translate("MainWindow", "Preview", nullptr));
        benchmarkButton->setText(QCoreApplication::translate("MainWindow", "Benchmark", nullptr));
        rifeLabel->setText(QCoreApplication::translate("MainWindow", "RIFE", nullptr));
        startFrameLabel->setText(QCoreApplication::translate("MainWindow", "Start Frame", nullptr));
        generatedFrameLabel->setText(QCoreApplication::translate("MainWindow", "Generated Frame", nullptr));
        endFrameLabel->setText(QCoreApplication::translate("MainWindow", "End Frame", nullptr));
        runButton->setText(QCoreApplication::translate("MainWindow", "Run", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuLoad_Input->setTitle(QCoreApplication::translate("MainWindow", "Load Input", nullptr));
        menuSettings->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
