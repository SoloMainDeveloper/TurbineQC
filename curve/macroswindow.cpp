#include "curve/pch.h"
#include "macroswindow.h"

MacrosWindow::MacrosWindow(QWidget *parent, Project *project) : QMainWindow(parent), _ui(new Ui::MacrosWindow()) {
    _ui->setupUi(this);
    _project = project;
    _operationList = _ui->operationList;

    setFixedSize(320, 660);
    auto wid = parentWidget()->width();
    move(parentWidget()->width() - width(), 0);
    _operationList->setHeaderHidden(true);
    setWindowTitle("Macros");

    removeItem = new QAction(QIcon("icons/delete.png"), "Remove");
    connect(removeItem, &QAction::triggered, this, &MacrosWindow::onRemoveItemTriggered);
    connect(&MacrosManager::instance(), &MacrosManager::operationLogged, this, &MacrosWindow::addOperation);
    connect(_ui->toggleRecordButton, &QPushButton::clicked, this, &MacrosWindow::toggleRecording);
    connect(_ui->clearButton, &QPushButton::clicked, this, &MacrosWindow::clear);
    connect(_ui->runButton, &QPushButton::clicked, this, &MacrosWindow::runMacros);
    connect(_ui->saveButton, &QPushButton::clicked, this, &MacrosWindow::save); 
    connect(_ui->loadButton, &QPushButton::clicked, this, &MacrosWindow::loadMacros);
}

void MacrosWindow::initialization() {
    show();
    emit needShow(false);
}

void MacrosWindow::addOperation(QString operation, QString comment) {
    auto root = new QTreeWidgetItem(_operationList);
    auto operationStr = QString("%1. %2").arg(_operationList->topLevelItemCount()).arg(operation);
    root->setText(0, operationStr);
    _operationList->addTopLevelItem(root);
    auto commentItem = new QTreeWidgetItem(root);
    auto commentList = comment.trimmed().split('\n');
    commentList.removeFirst();
    commentList.removeLast();
    commentItem->setText(0, commentList.join('\n'));
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
        MacrosManager::saveRecord(fileName);
    } else {
        QMessageBox::critical(nullptr, "Error", "Macros was not saved");
        MacrosManager::clear();
    }
}

void MacrosWindow::loadMacros() {
    auto path = QFileDialog::getOpenFileName(nullptr, "Open file", "", "(*.crmm)");
    if(!path.isEmpty()) {
        auto operations = FileSystem::readFile(path).split("$END_OPERATION");
        if(!operations.isEmpty()) {
            MacrosManager::toggleRecording();
            for(auto i = 0; i < operations.size(); i++) {
                if(operations.at(i).isEmpty()) continue;
                auto comment = operations.at(i).trimmed();
                auto type = comment.split('\n')[0].trimmed();
                MacrosManager::log(MacrosManager::fromString(type), {}, comment);
            }
            MacrosManager::toggleRecording();
        }
        initialization();
    }
}

void MacrosWindow::toggleRecording() {
    MacrosManager::toggleRecording();
    auto isRecording = MacrosManager::isRecording();
    if(isRecording) {
        _ui->toggleRecordButton->setText("Stop");
    } else {
        _ui->toggleRecordButton->setText("Start record");
    }
}

void MacrosWindow::runMacros() {
    MacrosManager::runMacros(_project);
}

void MacrosWindow::contextMenuEvent(QContextMenuEvent *event) {
    auto current = _operationList->currentItem();
    if(current && !current->parent()) {
        QMenu menu;
        menu.addAction(removeItem);
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

void MacrosWindow::removeOperation(QTreeWidgetItem *item) {
    auto text = item->text(0).split('.');
    auto number = text[0].toInt();
    _ui->operationList->takeTopLevelItem(number - 1);
    MacrosManager::remove(number - 1);
    reindex(number - 1);
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
