#include "curve/pch.h"
#include "macroswindow.h"

MacrosWindow::MacrosWindow(QWidget *parent, Project *project, Plot *plot) : QMainWindow(parent), _ui(new Ui::MacrosWindow()) {
    _ui->setupUi(this);
    _project = project;
    _plot = plot;
    _operationList = _ui->operationList;

    setFixedSize(320, 710);
    auto wid = parentWidget()->width();
    move(parentWidget()->width() - width(), 0);
    _operationList->setHeaderHidden(true);
    setWindowTitle("Macros");
    _ui->debugGroup->setVisible(false);

    removeItem = new QAction(QIcon("icons/delete.png"), "Remove");
    editItem = new QAction(QIcon("icons/circle.png"), "Edit");
    connect(removeItem, &QAction::triggered, this, &MacrosWindow::onRemoveItemTriggered);
    connect(editItem, &QAction::triggered, this, &MacrosWindow::onEditItemTriggered);
    connect(&MacrosManager::instance(), &MacrosManager::operationLogged, this, &MacrosWindow::addOperation);
    connect(&MacrosManager::instance(), &MacrosManager::recordingToggled, this, &MacrosWindow::updateRecordingButton);
    connect(&MacrosManager::instance(), &MacrosManager::recordIndexChanged, this, &MacrosWindow::onRecordIndexChanged);
    connect(&MacrosManager::instance(), &MacrosManager::operationExecuted, this, &MacrosWindow::onOperationExecuted);
    connect(_ui->toggleRecordButton, &QPushButton::clicked, this, &MacrosManager::toggleRecording);
    connect(_ui->clearButton, &QPushButton::clicked, this, &MacrosWindow::clear);
    connect(_ui->runButton, &QPushButton::clicked, this, &MacrosWindow::run);
    connect(_ui->saveButton, &QPushButton::clicked, this, &MacrosWindow::save); 
    connect(_ui->loadButton, &QPushButton::clicked, this, &MacrosManager::load);
    connect(_ui->moveRecordButton, &QPushButton::clicked, this, &MacrosWindow::onMoveRecordButtonClicked);
    connect(_ui->startDebugButton, &QPushButton::clicked, this, &MacrosWindow::startDebug);
    connect(_ui->stopDebugButton, &QPushButton::clicked, this, &MacrosWindow::stopDebug);
    connect(_ui->nextDebugButton, &QPushButton::clicked, this, &MacrosWindow::debugNext);
}

void MacrosWindow::initialization() {
    onRecordIndexChanged(MacrosManager::recordIndex());
    show();
    emit needShow(false);
}

void MacrosWindow::addOperation(QString operation, QString comment) {
    auto recordIndex = MacrosManager::recordIndex();
    auto item = createOperationItem(recordIndex, operation, comment);

    insert(_operationList, recordIndex, item);
    reindex(recordIndex);
}

void MacrosWindow::insert(QTreeWidget *tree, int index, QTreeWidgetItem* item) {
    auto items = QList<QTreeWidgetItem*>();
    while(index != tree->topLevelItemCount()) {
        auto itemi = tree->takeTopLevelItem(index - 1);
        auto text = itemi->text(0);
        items.append(itemi);
    }
    items.append(item);
    tree->insertTopLevelItems(index, items);
}

QTreeWidgetItem* MacrosWindow::createOperationItem(int index, QString operation, QString comment) {
    auto root = new QTreeWidgetItem(_operationList);
    root->setText(0, QString("%1. %2").arg(index).arg(operation));
    auto commentItem = new QTreeWidgetItem(root);
    auto commentList = comment.trimmed().split('\n');
    commentList.removeFirst();
    commentList.removeLast();
    commentItem->setText(0, commentList.join('\n'));
    return root;
}

void MacrosWindow::closeEvent(QCloseEvent *event) {
    emit needShow(true);
}

void MacrosWindow::clear() {
    _operationList->clear();
    MacrosManager::clear();
}

void MacrosWindow::save() {
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save macros", "", "Macros (*.crmm)");
    if(!fileName.isEmpty()) {
        MacrosManager::save(fileName);
    } else {
        QMessageBox::critical(nullptr, "Error", "Macros was not saved");
    }
}

void MacrosWindow::startDebug() {
    _ui->buttons->setVisible(false);
    _ui->debugGroup->setVisible(true);
}

void MacrosWindow::stopDebug() {
    MacrosManager::stopDebug();
    _ui->buttons->setVisible(true);
    _ui->debugGroup->setVisible(false);
    _ui->nextDebugButton->setEnabled(true);
    for(auto i = 0; i < _operationList->topLevelItemCount(); i++) {
        _operationList->topLevelItem(i)->setBackground(0, QBrush(QColor(255, 255, 255)));
    }
}

void MacrosWindow::debugNext() {
    MacrosManager::debugNext(_project, _plot);
}

void MacrosWindow::updateRecordingButton() {
    if(MacrosManager::isRecording()) {
        _ui->toggleRecordButton->setText("Stop");
    } else {
        _ui->toggleRecordButton->setText("Start record");
    }
}

void MacrosWindow::run() {
    MacrosManager::run(_project, _plot);
}

void MacrosWindow::contextMenuEvent(QContextMenuEvent *event) {
    auto current = _operationList->currentItem();
    if(current && !current->parent()) {
        QMenu menu;
        menu.addAction(removeItem);
        menu.addAction(editItem);
        menu.exec(event->globalPos());
    }
}

void MacrosWindow::onRemoveItemTriggered() {
    QMessageBox mBox;
    mBox.setText("Delete current item?");
    mBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    auto dialogWindow = mBox.exec();
    switch(dialogWindow) {
        case QMessageBox::Ok:
            removeOperation(_operationList->currentItem());
            break;
        case QMessageBox::Cancel:
            break;
    }
}

void MacrosWindow::onEditItemTriggered() {
    auto *dialog = new QDialog(this, Qt::WindowStaysOnTopHint);
    dialog->setWindowTitle("Edit current item");
    auto currentOperation = _operationList->currentItem();
    auto currentOperationText = currentOperation->child(0)->text(0);
    auto *edit = new QTextEdit(dialog);
    edit->setText(currentOperationText);
    edit->setGeometry(10, 10, 400, 400);
    edit->show();

    auto *cancelBtn = new QPushButton("Cancel", dialog);
    cancelBtn->setGeometry(10, 420, 195, 40);
    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::close);
    cancelBtn->show();

    auto *acceptBtn = new QPushButton("Accept", dialog);
    acceptBtn->setGeometry(215, 420, 195, 40);
    connect(acceptBtn, &QPushButton::clicked, this, [&]() {
        auto operationInfo = currentOperation->text(0).split('.');
        auto operationIndex = operationInfo[0].toInt() - 1;
        auto operationType = operationInfo[1].trimmed();
        auto newOperationText = operationType + '\n' + edit->toPlainText() + "\n$END_OPERATION\n";
        MacrosManager::editOperation(operationIndex, newOperationText);
        currentOperation->child(0)->setText(0, edit->toPlainText());
        dialog->close();
    });
    acceptBtn->show();

    dialog->setFixedSize(420, 470);
    dialog->exec();
}

void MacrosWindow::onMoveRecordButtonClicked() {
    auto *dialog = new QDialog(this, Qt::WindowStaysOnTopHint);
    dialog->setWindowTitle("Move record");
    auto *label = new QLabel(dialog);
    label->setText("Set record after which operation?");
    label->setGeometry(10, 10, 180, 30);

    auto *edit = new QLineEdit(dialog);
    edit->setValidator(new QIntValidator(0, _operationList->topLevelItemCount()));
    edit->setText(0);
    edit->setGeometry(10, 45, 180, 30);
    edit->show();

    auto *cancelBtn = new QPushButton("Cancel", dialog);
    cancelBtn->setGeometry(10, 80, 85, 30);
    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::close);
    cancelBtn->show();

    auto *acceptBtn = new QPushButton("Accept", dialog);
    acceptBtn->setGeometry(105, 80, 85, 30);
    connect(acceptBtn, &QPushButton::clicked, this, [&]() {
        auto index = edit->text().toInt();
        auto count = _operationList->topLevelItemCount();
        if(index >= 0 && index <= count) {
            MacrosManager::setRecordIndex(index);
            dialog->close();
        } else {
            edit->clear();
            edit->setPlaceholderText("Incorrect index. Try again");
        }
    });
    acceptBtn->show();

    dialog->setFixedSize(200, 120);
    dialog->exec();
}

void MacrosWindow::onRecordIndexChanged(int index) {
    _ui->indexLineEdit->setText(QString::number(index));
}

void MacrosWindow::onOperationExecuted(int index, bool isSuccessful) {
    if(isSuccessful) {
        _operationList->topLevelItem(index)->setBackground(0, QBrush(QColor(0, 255, 0)));
    } else {
        _operationList->topLevelItem(index)->setBackground(0, QBrush(QColor(255, 0, 0)));
        _ui->nextDebugButton->setEnabled(false);
    }
}

void MacrosWindow::removeOperation(QTreeWidgetItem *item) {
    auto text = item->text(0).split('.');
    auto index = text[0].toInt() - 1;
    _ui->operationList->takeTopLevelItem(index);
    MacrosManager::remove(index);
    reindex(index);
}

void MacrosWindow::reindex(int indexFrom) {
    for(auto i = indexFrom; i < _operationList->topLevelItemCount(); i++) {
        auto text = _operationList->topLevelItem(i)->text(0).split('.');
        _operationList->topLevelItem(i)->setText(0, QString("%1. %2").arg(i + 1).arg(text[1]));
    }
}

MacrosWindow::~MacrosWindow() {
    delete _ui;
}
