#pragma once

#include <QDialog>
#include "ui_widthedgewindow.h"
#include "project.h"
#include "algorithms.h"
#include "curvegraphics.h"

struct EdgeWidth {
public:
    explicit EdgeWidth(double widthLeadingEdge = 0, double distLE = 1,  double widthTrailingEdge = 0, double distTE = 1);
    virtual ~EdgeWidth();

    double widthLE;
    double distanceLE;
    double widthTE;
    double distanceTE;
};

namespace Ui {
    class WidthEdgeWindow;
}

class WidthEdgeWindow : public QDialog
{
    Q_OBJECT

public:
    WidthEdgeWindow(Project *project, QWidget *parent = nullptr);
    ~WidthEdgeWindow();

public slots:
    void initialization();

private:
    void setupWindow();
    void closeWindow();
    void calculateWidthEdge();
    void checkBoxLEStateChange();
    void checkBoxTEStateChange();
    void updateAnswerView();
    void closeEvent(QCloseEvent *event);

    Ui::WidthEdgeWindow *_ui;
    Project *_project;
    CurveGraphics *_curveGraphics;
    QMap<QString, EdgeWidth*> _widths;
    QDoubleValidator *_doubleValidator;

    QListWidget *curves;
    QComboBox *directionLE;
    QCheckBox *checkBoxLE;
    QCheckBox *checkBoxTE;
    QLineEdit *distanceLE;
    QLineEdit *distanceTE;
    QGroupBox *groupBoxLE;
    QGroupBox *groupBoxTE;
    QLineEdit *answerLE;
    QLineEdit *answerTE;
};
