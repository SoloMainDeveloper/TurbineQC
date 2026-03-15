#include "curve/pch.h"

#include "printpreviewdialog.h"

PrintPreviewDialog::PrintPreviewDialog() : _ui(new Ui::PrintPreviewDialog) {
    _ui->setupUi(this);

    this->setWindowTitle("Print Viewer");
    _printer = &Printer::instance();

    _ui->pageList->setHeaderHidden(true);
    _printItem = new QAction(QIcon("icons/printer.ico"), "Print Current");
    _deleteItem = new QAction(QIcon("icons/delete.ico"), "Delete");

    connect(_ui->printBtn, &QPushButton::clicked, this, &PrintPreviewDialog::printHandler);
    connect(_ui->clearBtn, &QPushButton::clicked, this, &PrintPreviewDialog::clearHandler);
    connect(_printItem, &QAction::triggered, this, &PrintPreviewDialog::printItemTriggered);
    connect(_deleteItem, &QAction::triggered, this, &PrintPreviewDialog::deleteItemTriggered);
    connect(_printer, &Printer::pageAdded, this, &PrintPreviewDialog::addPage);
    connect(_printer, &Printer::pageRemoved, this, &PrintPreviewDialog::removePage);
    connect(_printer, &Printer::printerPagesCleared, this->_ui->pageList, &QTreeWidget::clear);
}

void PrintPreviewDialog::initialize() {
    auto pageNumber = _ui->pageList->topLevelItemCount();
    this->show();
    emit needShow(false);
}

void PrintPreviewDialog::printHandler() {
    QList<int> pagesToTake;
    for(auto i = 0; i < _ui->pageList->topLevelItemCount(); i++) {
        auto item = _ui->pageList->topLevelItem(i);
        auto textItem = item->text(0);
        if(item->checkState(0) == Qt::Checked) {
            auto index = textItem.split("Page ")[1].toInt() - 1;
            pagesToTake.append(index);
        }
    }
    _printer->print(pagesToTake);
}

void PrintPreviewDialog::clearHandler() {
    _printer->clear();
}

void PrintPreviewDialog::printItemTriggered() {
    auto currentItem = _ui->pageList->currentItem();
    auto index = currentItem->text(0).split("Page ")[1].toInt() - 1;
    _printer->print({ index });
}

void PrintPreviewDialog::deleteItemTriggered() {
    auto currentIndex = _ui->pageList->currentIndex().row();
    _printer->removePage(currentIndex);
}

void PrintPreviewDialog::addPage(const QMap<QString, QString> &information) {
    auto pageNumber = _ui->pageList->topLevelItemCount() + 1;
    auto item = new QTreeWidgetItem(_ui->pageList, { QString("Page %1").arg(pageNumber) });
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(0, Qt::Checked);

    auto nominalCurve = QString("Nominal curve: %1\n").arg(information["nominalCurve"]);
    auto measuredCurve = QString("Measured curve: %1\n").arg(information["measuredCurve"]);
    auto creationTime = QString("Creation report time: %1").arg(information["time"]);
    auto child = new QTreeWidgetItem({ nominalCurve + measuredCurve + creationTime });
    item->addChild(child);

    _ui->pageList->addTopLevelItem(item);
}

void PrintPreviewDialog::removePage(int index) {
    auto currentItem = _ui->pageList->topLevelItem(index);
    delete currentItem;
    updateReportPages(index);
}

void PrintPreviewDialog::updateReportPages(int updateIndex) {
    for(auto i = updateIndex; i < _ui->pageList->topLevelItemCount(); i++) {
        auto currentItem = _ui->pageList->topLevelItem(i);
        currentItem->setText(0, QString("Page %1").arg(i + 1));
    }
}

void PrintPreviewDialog::contextMenuEvent(QContextMenuEvent * event) {
    if(_ui->pageList->currentItem()) {
        QMenu menu(this);
        menu.addAction(_printItem);
        menu.addAction(_deleteItem);
        menu.exec(event->globalPos());
    }
}

void PrintPreviewDialog::closeEvent(QCloseEvent * event) {
    emit needShow(true);
}

PrintPreviewDialog::~PrintPreviewDialog() {
    delete _ui;
}
