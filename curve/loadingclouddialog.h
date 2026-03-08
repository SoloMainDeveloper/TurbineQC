#pragma once

namespace Ui {
    class LoadingCloudDialog;
};

class LoadingCloudDialog : public BaseDialog {
    Q_OBJECT

public:
    explicit LoadingCloudDialog();
    virtual ~LoadingCloudDialog();

    void initialize() override;

private slots:
    void selectFile();
    void selectClipboard();
    void findFile();
    void openFile();
    void pasteData();
    void loadCloud();
    void closeEvent(QCloseEvent *event);
    void closeWindow();

private:
    Ui::LoadingCloudDialog *_ui;

    QGroupBox *_groupOfBtnWidgets;
    QHBoxLayout *_btnWidgetsLayout;
    QPushButton *_pasteDataBtn;
    QMessageBox *_message;

    QStringList _fileData;

    bool isCorrectFileExtension(const QString &filePath);
};
