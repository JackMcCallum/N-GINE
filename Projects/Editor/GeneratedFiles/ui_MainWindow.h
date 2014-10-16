/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "console.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionItem;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QMenuBar *menuBar;
    QMenu *menuMenu;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame_2;
    QDockWidget *dockWidget_2;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_4;
    Console *consoleView;
    QDockWidget *consoleDock;
    QWidget *dockWidgetContents_3;
    QVBoxLayout *verticalLayout_5;
    QFrame *frame_4;
    QDockWidget *dockWidget_4;
    QWidget *dockWidgetContents_4;
    QVBoxLayout *verticalLayout_2;
    QWidget *sceneView;
    QDockWidget *dockWidget_5;
    QWidget *dockWidgetContents_5;
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(875, 556);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon_cube_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setAnimated(true);
        MainWindow->setTabShape(QTabWidget::Rounded);
        MainWindow->setDockNestingEnabled(true);
        MainWindow->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);
        MainWindow->setUnifiedTitleAndToolBarOnMac(false);
        actionItem = new QAction(MainWindow);
        actionItem->setObjectName(QStringLiteral("actionItem"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setEnabled(true);
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 6, 0, 0);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 875, 21));
        menuMenu = new QMenu(menuBar);
        menuMenu->setObjectName(QStringLiteral("menuMenu"));
        MainWindow->setMenuBar(menuBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icon_cube_purple.png"), QSize(), QIcon::Normal, QIcon::Off);
        dockWidget->setWindowIcon(icon1);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        frame_2 = new QFrame(dockWidgetContents);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);

        verticalLayout_3->addWidget(frame_2);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
        dockWidget_2 = new QDockWidget(MainWindow);
        dockWidget_2->setObjectName(QStringLiteral("dockWidget_2"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icon_cube_green.png"), QSize(), QIcon::Normal, QIcon::Off);
        dockWidget_2->setWindowIcon(icon2);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        verticalLayout_4 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        consoleView = new Console(dockWidgetContents_2);
        consoleView->setObjectName(QStringLiteral("consoleView"));
        consoleView->setFrameShape(QFrame::NoFrame);
        consoleView->setLineWidth(0);
        consoleView->setAlternatingRowColors(true);
        consoleView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        consoleView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        consoleView->setWordWrap(true);

        verticalLayout_4->addWidget(consoleView);

        dockWidget_2->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget_2);
        consoleDock = new QDockWidget(MainWindow);
        consoleDock->setObjectName(QStringLiteral("consoleDock"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icon_cube_red.png"), QSize(), QIcon::Normal, QIcon::Off);
        consoleDock->setWindowIcon(icon3);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QStringLiteral("dockWidgetContents_3"));
        verticalLayout_5 = new QVBoxLayout(dockWidgetContents_3);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        frame_4 = new QFrame(dockWidgetContents_3);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);

        verticalLayout_5->addWidget(frame_4);

        consoleDock->setWidget(dockWidgetContents_3);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), consoleDock);
        dockWidget_4 = new QDockWidget(MainWindow);
        dockWidget_4->setObjectName(QStringLiteral("dockWidget_4"));
        dockWidget_4->setWindowIcon(icon);
        dockWidget_4->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents_4);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        sceneView = new QWidget(dockWidgetContents_4);
        sceneView->setObjectName(QStringLiteral("sceneView"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sceneView->sizePolicy().hasHeightForWidth());
        sceneView->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(sceneView);

        dockWidget_4->setWidget(dockWidgetContents_4);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(4), dockWidget_4);
        dockWidget_5 = new QDockWidget(MainWindow);
        dockWidget_5->setObjectName(QStringLiteral("dockWidget_5"));
        dockWidget_5->setWindowIcon(icon);
        dockWidget_5->setFloating(false);
        dockWidget_5->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QStringLiteral("dockWidgetContents_5"));
        horizontalLayout = new QHBoxLayout(dockWidgetContents_5);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(dockWidgetContents_5);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        horizontalLayout->addWidget(listWidget);

        dockWidget_5->setWidget(dockWidgetContents_5);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget_5);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuMenu->menuAction());
        menuMenu->addAction(actionItem);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionItem->setText(QApplication::translate("MainWindow", "Item", 0));
        menuMenu->setTitle(QApplication::translate("MainWindow", "Menu", 0));
        dockWidget->setWindowTitle(QApplication::translate("MainWindow", "Scene Outline", 0));
        dockWidget_2->setWindowTitle(QApplication::translate("MainWindow", "Console", 0));
        consoleDock->setWindowTitle(QApplication::translate("MainWindow", "Inspector", 0));
        dockWidget_4->setWindowTitle(QApplication::translate("MainWindow", "Scene View", 0));
        dockWidget_5->setWindowTitle(QApplication::translate("MainWindow", "Project", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
