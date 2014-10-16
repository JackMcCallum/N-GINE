/********************************************************************************
** Form generated from reading UI file 'CubeMap.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUBEMAP_H
#define UI_CUBEMAP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ViewPort.h"

QT_BEGIN_NAMESPACE

class Ui_CubeMapClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    ViewPort *viewPort;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *selectedLabel2;
    QLabel *selectedLabel1;
    QLabel *selectedLabel3;
    QLabel *selectedLabel4;
    QLabel *selectedLabel5;
    QPushButton *pushButton_2;
    QPushButton *pushButton_6;
    QPushButton *pushButton_5;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QComboBox *comboBox;
    QLabel *selectedLabel0;
    QPushButton *pushButton;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QLabel *label_3;
    QSlider *horizontalSlider_2;
    QLabel *label_2;
    QSlider *horizontalSlider;
    QPushButton *pushButton_9;
    QLabel *label_4;
    QSlider *horizontalSlider_5;
    QLabel *label_7;
    QComboBox *comboBox_2;
    QLabel *label_5;
    QSlider *horizontalSlider_3;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_8;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_7;

    void setupUi(QMainWindow *CubeMapClass)
    {
        if (CubeMapClass->objectName().isEmpty())
            CubeMapClass->setObjectName(QStringLiteral("CubeMapClass"));
        CubeMapClass->resize(1073, 867);
        centralWidget = new QWidget(CubeMapClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        viewPort = new ViewPort(centralWidget);
        viewPort->setObjectName(QStringLiteral("viewPort"));
        viewPort->setMinimumSize(QSize(640, 480));

        horizontalLayout->addWidget(viewPort);

        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMaximumSize(QSize(300, 16777215));
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(widget_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label, 14, 0, 1, 1);

        selectedLabel2 = new QLabel(groupBox);
        selectedLabel2->setObjectName(QStringLiteral("selectedLabel2"));
        sizePolicy.setHeightForWidth(selectedLabel2->sizePolicy().hasHeightForWidth());
        selectedLabel2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(selectedLabel2, 6, 0, 1, 1);

        selectedLabel1 = new QLabel(groupBox);
        selectedLabel1->setObjectName(QStringLiteral("selectedLabel1"));
        sizePolicy.setHeightForWidth(selectedLabel1->sizePolicy().hasHeightForWidth());
        selectedLabel1->setSizePolicy(sizePolicy);

        gridLayout->addWidget(selectedLabel1, 4, 0, 1, 1);

        selectedLabel3 = new QLabel(groupBox);
        selectedLabel3->setObjectName(QStringLiteral("selectedLabel3"));
        sizePolicy.setHeightForWidth(selectedLabel3->sizePolicy().hasHeightForWidth());
        selectedLabel3->setSizePolicy(sizePolicy);

        gridLayout->addWidget(selectedLabel3, 9, 0, 1, 1);

        selectedLabel4 = new QLabel(groupBox);
        selectedLabel4->setObjectName(QStringLiteral("selectedLabel4"));
        sizePolicy.setHeightForWidth(selectedLabel4->sizePolicy().hasHeightForWidth());
        selectedLabel4->setSizePolicy(sizePolicy);

        gridLayout->addWidget(selectedLabel4, 11, 0, 1, 1);

        selectedLabel5 = new QLabel(groupBox);
        selectedLabel5->setObjectName(QStringLiteral("selectedLabel5"));
        sizePolicy.setHeightForWidth(selectedLabel5->sizePolicy().hasHeightForWidth());
        selectedLabel5->setSizePolicy(sizePolicy);

        gridLayout->addWidget(selectedLabel5, 13, 0, 1, 1);

        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 4, 1, 1, 1);

        pushButton_6 = new QPushButton(groupBox);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));

        gridLayout->addWidget(pushButton_6, 13, 1, 1, 1);

        pushButton_5 = new QPushButton(groupBox);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        gridLayout->addWidget(pushButton_5, 11, 1, 1, 1);

        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 9, 1, 1, 1);

        pushButton_4 = new QPushButton(groupBox);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        gridLayout->addWidget(pushButton_4, 6, 1, 1, 1);

        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setMaxVisibleItems(12);
        comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout->addWidget(comboBox, 14, 1, 1, 1);

        selectedLabel0 = new QLabel(groupBox);
        selectedLabel0->setObjectName(QStringLiteral("selectedLabel0"));
        sizePolicy.setHeightForWidth(selectedLabel0->sizePolicy().hasHeightForWidth());
        selectedLabel0->setSizePolicy(sizePolicy);

        gridLayout->addWidget(selectedLabel0, 2, 0, 1, 1);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_3 = new QGroupBox(widget_2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_3->addWidget(label_3, 7, 1, 1, 1);

        horizontalSlider_2 = new QSlider(groupBox_3);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setMinimum(1);
        horizontalSlider_2->setMaximum(10000);
        horizontalSlider_2->setValue(2000);
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        gridLayout_3->addWidget(horizontalSlider_2, 7, 2, 1, 1);

        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_3->addWidget(label_2, 3, 1, 1, 1);

        horizontalSlider = new QSlider(groupBox_3);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setMaximum(10000);
        horizontalSlider->setValue(600);
        horizontalSlider->setOrientation(Qt::Horizontal);
        horizontalSlider->setInvertedAppearance(false);
        horizontalSlider->setInvertedControls(false);
        horizontalSlider->setTickPosition(QSlider::NoTicks);

        gridLayout_3->addWidget(horizontalSlider, 3, 2, 1, 1);

        pushButton_9 = new QPushButton(groupBox_3);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));

        gridLayout_3->addWidget(pushButton_9, 2, 2, 1, 1);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_3->addWidget(label_4, 5, 1, 1, 1);

        horizontalSlider_5 = new QSlider(groupBox_3);
        horizontalSlider_5->setObjectName(QStringLiteral("horizontalSlider_5"));
        horizontalSlider_5->setMaximum(10000);
        horizontalSlider_5->setValue(10000);
        horizontalSlider_5->setOrientation(Qt::Horizontal);
        horizontalSlider_5->setTickPosition(QSlider::NoTicks);

        gridLayout_3->addWidget(horizontalSlider_5, 5, 2, 1, 1);

        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_3->addWidget(label_7, 4, 1, 1, 1);

        comboBox_2 = new QComboBox(groupBox_3);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));

        gridLayout_3->addWidget(comboBox_2, 0, 2, 1, 1);

        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);

        gridLayout_3->addWidget(label_5, 0, 1, 1, 1);

        horizontalSlider_3 = new QSlider(groupBox_3);
        horizontalSlider_3->setObjectName(QStringLiteral("horizontalSlider_3"));
        horizontalSlider_3->setMaximum(10000);
        horizontalSlider_3->setValue(10000);
        horizontalSlider_3->setOrientation(Qt::Horizontal);
        horizontalSlider_3->setTickPosition(QSlider::NoTicks);

        gridLayout_3->addWidget(horizontalSlider_3, 4, 2, 1, 1);


        verticalLayout->addWidget(groupBox_3);

        groupBox_5 = new QGroupBox(widget_2);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        gridLayout_4 = new QGridLayout(groupBox_5);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));

        verticalLayout->addWidget(groupBox_5);

        groupBox_4 = new QGroupBox(widget_2);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        gridLayout_2 = new QGridLayout(groupBox_4);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        pushButton_8 = new QPushButton(groupBox_4);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));

        gridLayout_2->addWidget(pushButton_8, 0, 0, 1, 2);


        verticalLayout->addWidget(groupBox_4);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        groupBox_2 = new QGroupBox(widget_2);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setEnabled(false);
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        pushButton_7 = new QPushButton(groupBox_2);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));

        verticalLayout_2->addWidget(pushButton_7);


        verticalLayout->addWidget(groupBox_2);


        horizontalLayout->addWidget(widget_2);

        CubeMapClass->setCentralWidget(centralWidget);

        retranslateUi(CubeMapClass);
        QObject::connect(pushButton_8, SIGNAL(clicked()), viewPort, SLOT(openDisplayModel()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), viewPort, SLOT(choosePosY()));
        QObject::connect(pushButton_5, SIGNAL(clicked()), viewPort, SLOT(choosePosZ()));
        QObject::connect(pushButton, SIGNAL(clicked()), viewPort, SLOT(choosePosX()));
        QObject::connect(pushButton_6, SIGNAL(clicked()), viewPort, SLOT(chooseNegZ()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), viewPort, SLOT(chooseNegY()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), viewPort, SLOT(chooseNegX()));
        QObject::connect(horizontalSlider, SIGNAL(valueChanged(int)), viewPort, SLOT(updateMicrosurface(int)));
        QObject::connect(horizontalSlider_3, SIGNAL(valueChanged(int)), viewPort, SLOT(updateReflectivity(int)));
        QObject::connect(horizontalSlider_5, SIGNAL(valueChanged(int)), viewPort, SLOT(updateFresnel(int)));
        QObject::connect(horizontalSlider_2, SIGNAL(valueChanged(int)), viewPort, SLOT(updateSampleCount(int)));
        QObject::connect(pushButton_9, SIGNAL(clicked()), viewPort, SLOT(chooseColor()));
        QObject::connect(comboBox, SIGNAL(currentIndexChanged(QString)), viewPort, SLOT(texSizeChanged(QString)));

        comboBox->setCurrentIndex(9);


        QMetaObject::connectSlotsByName(CubeMapClass);
    } // setupUi

    void retranslateUi(QMainWindow *CubeMapClass)
    {
        CubeMapClass->setWindowTitle(QApplication::translate("CubeMapClass", "CubeMap", 0));
        groupBox->setTitle(QApplication::translate("CubeMapClass", "Images", 0));
        label->setText(QApplication::translate("CubeMapClass", "Top level mip map size:", 0));
        selectedLabel2->setText(QApplication::translate("CubeMapClass", "None Selected....", 0));
        selectedLabel1->setText(QApplication::translate("CubeMapClass", "None Selected....", 0));
        selectedLabel3->setText(QApplication::translate("CubeMapClass", "None Selected....", 0));
        selectedLabel4->setText(QApplication::translate("CubeMapClass", "None Selected....", 0));
        selectedLabel5->setText(QApplication::translate("CubeMapClass", "None Selected....", 0));
        pushButton_2->setText(QApplication::translate("CubeMapClass", "Right (-X)", 0));
        pushButton_6->setText(QApplication::translate("CubeMapClass", "Back (-Z)", 0));
        pushButton_5->setText(QApplication::translate("CubeMapClass", "Front (+Z)", 0));
        pushButton_3->setText(QApplication::translate("CubeMapClass", "Down (-Y)", 0));
        pushButton_4->setText(QApplication::translate("CubeMapClass", "Up (+Y)", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("CubeMapClass", "1x1", 0)
         << QApplication::translate("CubeMapClass", "2x2", 0)
         << QApplication::translate("CubeMapClass", "4x4", 0)
         << QApplication::translate("CubeMapClass", "8x8", 0)
         << QApplication::translate("CubeMapClass", "16x16", 0)
         << QApplication::translate("CubeMapClass", "32x32", 0)
         << QApplication::translate("CubeMapClass", "64x64", 0)
         << QApplication::translate("CubeMapClass", "128x128", 0)
         << QApplication::translate("CubeMapClass", "256x256", 0)
         << QApplication::translate("CubeMapClass", "512x512", 0)
         << QApplication::translate("CubeMapClass", "1024x1024", 0)
         << QApplication::translate("CubeMapClass", "2048x2048", 0)
        );
        selectedLabel0->setText(QApplication::translate("CubeMapClass", "None Selected....", 0));
        pushButton->setText(QApplication::translate("CubeMapClass", "Left (+X)", 0));
        groupBox_3->setTitle(QApplication::translate("CubeMapClass", "Shader", 0));
        label_3->setText(QApplication::translate("CubeMapClass", "Sample Count:", 0));
        label_2->setText(QApplication::translate("CubeMapClass", "Microsurface:", 0));
        pushButton_9->setText(QApplication::translate("CubeMapClass", "Pick Color", 0));
        label_4->setText(QApplication::translate("CubeMapClass", "Fresnel:", 0));
        label_7->setText(QApplication::translate("CubeMapClass", "Reflectivity:", 0));
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("CubeMapClass", "Original", 0)
         << QApplication::translate("CubeMapClass", "Filtered", 0)
        );
        label_5->setText(QApplication::translate("CubeMapClass", "Shader:", 0));
        groupBox_5->setTitle(QApplication::translate("CubeMapClass", "Filter", 0));
        groupBox_4->setTitle(QApplication::translate("CubeMapClass", "Model", 0));
        pushButton_8->setText(QApplication::translate("CubeMapClass", "Open Display Model (.obj)", 0));
        groupBox_2->setTitle(QApplication::translate("CubeMapClass", "Export", 0));
        pushButton_7->setText(QApplication::translate("CubeMapClass", "Export", 0));
    } // retranslateUi

};

namespace Ui {
    class CubeMapClass: public Ui_CubeMapClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUBEMAP_H
