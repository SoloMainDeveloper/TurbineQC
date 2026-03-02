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
    _ui->buttonsList->setCurrentIndex(0);
    _isDebugging = false;

    editItem = new QAction(QIcon("icons/edit.ico"), "Edit");
    removeItem = new QAction(QIcon("icons/delete.ico"), "Remove");
    executeCurrentItem = new QAction(QIcon("icons/debug.ico"), "Execute current");
    moveOperationUp = new QAction(QIcon("icons/arrowUp.ico"), "Move up");
    moveOperationDown = new QAction(QIcon("icons/arrowDown.ico"), "Move down");

    connect(removeItem, &QAction::triggered, this, &MacrosWindow::onRemoveItemTriggered);
    connect(editItem, &QAction::triggered, this, &MacrosWindow::onEditItemTriggered);
    connect(executeCurrentItem, &QAction::triggered, this, &MacrosWindow::onExecuteCurrentItemTriggered);
    connect(moveOperationUp, &QAction::triggered, this, &MacrosWindow::onMoveOperationUpItemTriggered);
    connect(moveOperationDown, &QAction::triggered, this, &MacrosWindow::onMoveOperationDownItemTriggered);

    connect(&MacrosManager::instance(), &MacrosManager::operationLogged, this, &MacrosWindow::addOperation);
    connect(&MacrosManager::instance(), &MacrosManager::recordingToggled, this, &MacrosWindow::updateRecordingButton);
    connect(&MacrosManager::instance(), &MacrosManager::recordIndexChanged, this, &MacrosWindow::onRecordIndexChanged);
    connect(&MacrosManager::instance(), &MacrosManager::operationExecuted, this, &MacrosWindow::onOperationExecuted);
    connect(&MacrosManager::instance(), &MacrosManager::operationSkipped, this, &MacrosWindow::onOperationSkipped);

    connect(_ui->toggleRecordButton, &QPushButton::clicked, this, &MacrosManager::toggleRecording);
    connect(_ui->clearButton, &QPushButton::clicked, this, &MacrosWindow::clear);
    connect(_ui->runButton, &QPushButton::clicked, this, &MacrosWindow::run);
    connect(_ui->saveButton, &QPushButton::clicked, this, &MacrosWindow::save); 
    connect(_ui->loadButton, &QPushButton::clicked, this, &MacrosManager::load);
    connect(_ui->moveRecordButton, &QPushButton::clicked, this, &MacrosWindow::onMoveRecordButtonClicked);
    connect(_ui->startDebugButton, &QPushButton::clicked, this, &MacrosWindow::startDebug);
    connect(_ui->stopDebugButton, &QPushButton::clicked, this, &MacrosWindow::stopDebug);
    connect(_ui->nextDebugButton, &QPushButton::clicked, this, &MacrosWindow::debugNext);
    connect(_ui->skipButton, &QPushButton::clicked, this, &MacrosManager::skipOne);
}

void MacrosWindow::initialization() {
    onRecordIndexChanged(MacrosManager::recordIndex());
    show();
    emit needShow(false);
}

void MacrosWindow::addOperation(QString operation, QString comment) {
    auto recordIndex = MacrosManager::recordIndex();
    auto item = createOperationItem(recordIndex, operation, comment);
    insert(recordIndex, item);
    reindex(recordIndex);
}

void MacrosWindow::insert(int index, QTreeWidgetItem* item) {
    auto items = QList<QTreeWidgetItem*>();
    while(index != _operationList->topLevelItemCount()) {
        auto itemi = _operationList->takeTopLevelItem(index - 1);
        auto text = itemi->text(0);
        items.append(itemi);
    }
    items.append(item);
    _operationList->insertTopLevelItems(index, items);
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
    _ui->buttonsList->setCurrentIndex(1);
    _isDebugging = true;
}

void MacrosWindow::stopDebug() {
    MacrosManager::stopDebug();
    _ui->buttonsList->setCurrentIndex(0);
    _ui->nextDebugButton->setEnabled(true);
    _ui->skipButton->setEnabled(true);
    _isDebugging = false;
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
    auto index = current->text(0).split('.')[0].toInt() - 1;
    if(current && !current->parent()) {
        QMenu menu;
        menu.addAction(removeItem);
        menu.addAction(editItem);
        if(!_isDebugging) {
            moveOperationUp->setEnabled(index != 0);
            moveOperationDown->setEnabled(index != _operationList->topLevelItemCount() - 1);
            menu.addAction(executeCurrentItem);
            menu.addAction(moveOperationUp);
            menu.addAction(moveOperationDown);
        }
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
        {
        auto index = _operationList->currentItem()->text(0).split('.')[0].toInt() - 1;
            removeOperation(index);
            break;
        }
        case QMessageBox::Cancel:
            break;
    }
}

void MacrosWindow::onExecuteCurrentItemTriggered() {
     auto index = _operationList->currentItem()->text(0).split('.')[0].toInt() - 1;
     MacrosManager::executeOne(_project, _plot, index);
}

void MacrosWindow::onMoveOperationUpItemTriggered() {
     auto index = _operationList->currentItem()->text(0).split('.')[0].toInt() - 1;
     if(index == 0) return;
     _operationList->insertTopLevelItem(index - 1, _operationList->takeTopLevelItem(index));
     _operationList->setCurrentItem(_operationList->topLevelItem(index - 1));
     reindex(index - 1, index);
     MacrosManager::swapOperations(index - 1, index);
}

void MacrosWindow::onMoveOperationDownItemTriggered() {
    auto index = _operationList->currentItem()->text(0).split('.')[0].toInt() - 1;
    if(index == _operationList->topLevelItemCount() - 1) return;
    _operationList->insertTopLevelItem(index + 1, _operationList->takeTopLevelItem(index));
    _operationList->setCurrentItem(_operationList->topLevelItem(index + 1));
    reindex(index, index + 1);
    MacrosManager::swapOperations(index, index + 1);
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
        _ui->skipButton->setEnabled(false);
    }
}

void MacrosWindow::onOperationSkipped(int index) {
    _operationList->topLevelItem(index)->setBackground(0, QBrush(QColor(150, 150, 150)));
}

void MacrosWindow::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Delete && _ui->operationList->currentItem()) {
        QMessageBox mBox;
        QString name = _ui->operationList->currentItem()->text(0);

        mBox.setText(QString("Delete %1?").arg(name));
        mBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        int dialogResponse = mBox.exec();
        if(dialogResponse == QMessageBox::Ok) {
            auto index = _operationList->currentItem()->text(0).split('.')[0].toInt() - 1;
            removeOperation(index);
        }
    }
}

void MacrosWindow::removeOperation(int index) {
    _ui->operationList->takeTopLevelItem(index);
    MacrosManager::remove(index);
    reindex(index);
}

void MacrosWindow::reindex(int indexFrom, int indexTo) {
    for(auto i = indexFrom; i <= indexTo; i++) { //check
        auto text = _operationList->topLevelItem(i)->text(0).split(". ");
        _operationList->topLevelItem(i)->setText(0, QString("%1. %2").arg(i + 1).arg(text[1]));
    }
}

void MacrosWindow::reindex(int indexFrom) {
    reindex(indexFrom, _operationList->topLevelItemCount() - 1); // -1
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

MacrosWindow::~MacrosWindow() {
    delete _ui;
}
