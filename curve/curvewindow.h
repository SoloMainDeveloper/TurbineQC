#pragma once

namespace Ui {
    class CurveWindow;
};

class CurveWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit CurveWindow(QWidget *parent = nullptr);
    virtual ~CurveWindow();

private:
    Ui::CurveWindow *_ui;
};
