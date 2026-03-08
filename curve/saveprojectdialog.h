#pragma once

namespace Ui {
    class SaveProjectDialog;
}

class SaveProjectDialog : public BaseDialog {
    Q_OBJECT

public:
    explicit SaveProjectDialog();
    virtual ~SaveProjectDialog();

public slots:
    void initialize() override;

private slots:
    void closeWindow();
    void closeEvent(QCloseEvent *event);
    void showBoxCRV();
    void saveProject();
    void openExplorer();
    void projectNameChanged();

private:
    Ui::SaveProjectDialog *_ui;
};
