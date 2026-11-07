/********************************************************************************
** Form generated from reading UI file 'algorithmpreviewrow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALGORITHMPREVIEWROW_H
#define UI_ALGORITHMPREVIEWROW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "frameview.h"

QT_BEGIN_NAMESPACE

class Ui_AlgorithmPreviewRow
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *algorithmNameLabel;
    QVBoxLayout *verticalLayout;
    QLabel *frame1Label;
    FrameView *frameView1;
    QVBoxLayout *verticalLayout_2;
    QLabel *frame2Label;
    FrameView *frameView2;
    QVBoxLayout *verticalLayout_3;
    QLabel *frame3Label;
    FrameView *frameView3;

    void setupUi(QWidget *AlgorithmPreviewRow)
    {
        if (AlgorithmPreviewRow->objectName().isEmpty())
            AlgorithmPreviewRow->setObjectName("AlgorithmPreviewRow");
        AlgorithmPreviewRow->resize(756, 539);
        horizontalLayout = new QHBoxLayout(AlgorithmPreviewRow);
        horizontalLayout->setObjectName("horizontalLayout");
        algorithmNameLabel = new QLabel(AlgorithmPreviewRow);
        algorithmNameLabel->setObjectName("algorithmNameLabel");
        algorithmNameLabel->setMinimumSize(QSize(80, 0));
        algorithmNameLabel->setMaximumSize(QSize(80, 16777215));

        horizontalLayout->addWidget(algorithmNameLabel);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        frame1Label = new QLabel(AlgorithmPreviewRow);
        frame1Label->setObjectName("frame1Label");
        frame1Label->setMinimumSize(QSize(0, 24));
        frame1Label->setMaximumSize(QSize(16777215, 24));
        frame1Label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(frame1Label);

        frameView1 = new FrameView(AlgorithmPreviewRow);
        frameView1->setObjectName("frameView1");
        frameView1->setMinimumSize(QSize(120, 90));

        verticalLayout->addWidget(frameView1);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        frame2Label = new QLabel(AlgorithmPreviewRow);
        frame2Label->setObjectName("frame2Label");
        frame2Label->setMinimumSize(QSize(0, 24));
        frame2Label->setMaximumSize(QSize(16777215, 24));
        frame2Label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(frame2Label);

        frameView2 = new FrameView(AlgorithmPreviewRow);
        frameView2->setObjectName("frameView2");
        frameView2->setMinimumSize(QSize(120, 90));

        verticalLayout_2->addWidget(frameView2);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        frame3Label = new QLabel(AlgorithmPreviewRow);
        frame3Label->setObjectName("frame3Label");
        frame3Label->setMinimumSize(QSize(0, 24));
        frame3Label->setMaximumSize(QSize(16777215, 24));
        frame3Label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_3->addWidget(frame3Label);

        frameView3 = new FrameView(AlgorithmPreviewRow);
        frameView3->setObjectName("frameView3");
        frameView3->setMinimumSize(QSize(120, 90));

        verticalLayout_3->addWidget(frameView3);


        horizontalLayout->addLayout(verticalLayout_3);

        horizontalLayout->setStretch(1, 1);
        horizontalLayout->setStretch(2, 1);
        horizontalLayout->setStretch(3, 1);

        retranslateUi(AlgorithmPreviewRow);

        QMetaObject::connectSlotsByName(AlgorithmPreviewRow);
    } // setupUi

    void retranslateUi(QWidget *AlgorithmPreviewRow)
    {
        AlgorithmPreviewRow->setWindowTitle(QCoreApplication::translate("AlgorithmPreviewRow", "Form", nullptr));
        algorithmNameLabel->setText(QCoreApplication::translate("AlgorithmPreviewRow", "Algorithm", nullptr));
        frame1Label->setText(QCoreApplication::translate("AlgorithmPreviewRow", "Start Frame", nullptr));
        frameView1->setText(QCoreApplication::translate("AlgorithmPreviewRow", "Frame 1", nullptr));
        frame2Label->setText(QCoreApplication::translate("AlgorithmPreviewRow", "Intepolated Frame", nullptr));
        frameView2->setText(QCoreApplication::translate("AlgorithmPreviewRow", "Frame 2", nullptr));
        frame3Label->setText(QCoreApplication::translate("AlgorithmPreviewRow", "End Frame", nullptr));
        frameView3->setText(QCoreApplication::translate("AlgorithmPreviewRow", "Frame 3", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AlgorithmPreviewRow: public Ui_AlgorithmPreviewRow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALGORITHMPREVIEWROW_H
