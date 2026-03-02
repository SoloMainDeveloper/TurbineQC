#include "curve/pch.h"
#include "printpreviewwindow.h"

PrintPreviewWindow::PrintPreviewWindow(QWidget *parent) : QMainWindow(parent), _ui(new Ui::PrintPreviewWindow) {
    _ui->setupUi(this);

    this->setWindowTitle("Print Viewer");

    _ui->pageList->setHeaderHidden(true);
    _printItem = new QAction(QIcon("icons/printer.ico"), "Print Current");
    _deleteItem = new QAction(QIcon("icons/delete.ico"), "Delete");

    connect(_ui->printBtn, &QPushButton::clicked, this, &PrintPreviewWindow::printHandler);
    connect(_ui->clearBtn, &QPushButton::clicked, this, &PrintPreviewWindow::clearHandler);
    connect(_printItem, &QAction::triggered, this, &PrintPreviewWindow::printItemTriggered);
    connect(_deleteItem, &QAction::triggered, this, &PrintPreviewWindow::deleteItemTriggered);
    connect(&Printer::instance(), &Printer::pageAdded, this, &PrintPreviewWindow::addPage);
    connect(&Printer::instance(), &Printer::pageRemoved, this, &PrintPreviewWindow::removePage);
    connect(&Printer::instance(), &Printer::printerPagesCleared, this->_ui->pageList, &QTreeWidget::clear);
}

void PrintPreviewWindow::initialization() {
    auto pageNumber = _ui->pageList->topLevelItemCount();
    this->show();
    emit needShow(false);
}

void PrintPreviewWindow::printHandler() {
    QStringList pagesToTake;
    for(auto i = 0; i < _ui->pageList->topLevelItemCount(); i++) {
        auto item = _ui->pageList->topLevelItem(i);
        auto textItem = item->text(0);
        if(item->checkState(0) == Qt::Checked) {
            auto index = textItem.split("Page ")[1].toInt() - 1;
            pagesToTake.append(QString::number(index));
        }
    }
    Printer::print(pagesToTake);
}

void PrintPreviewWindow::clearHandler() {
    Printer::clear();
}

void PrintPreviewWindow::printItemTriggered() {
    auto currentItem = _ui->pageList->currentItem();
    auto index = currentItem->text(0).split("Page ")[1].toInt() - 1;
    Printer::print({ QString::number(index)});
}

void PrintPreviewWindow::deleteItemTriggered() {
    auto currentIndex = _ui->pageList->currentIndex().row();
    Printer::removePage(currentIndex);
}

void PrintPreviewWindow::addPage(const QMap<QString, QString> &information) {
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

void PrintPreviewWindow::removePage(int index) {
    auto currentItem = _ui->pageList->topLevelItem(index);
    delete currentItem;
    updateReportPages(index);
}

void PrintPreviewWindow::updateReportPages(int updateIndex) {
    for(auto i = updateIndex; i < _ui->pageList->topLevelItemCount(); i++) {
        auto currentItem = _ui->pageList->topLevelItem(i);
        currentItem->setText(0, QString("Page %1").arg(i + 1));
    }
}

void PrintPreviewWindow::contextMenuEvent(QContextMenuEvent * event) {
    if(_ui->pageList->currentItem()) {
        QMenu menu(this);
        menu.addAction(_printItem);
        menu.addAction(_deleteItem);
        menu.exec(event->globalPos());
    }
}

void PrintPreviewWindow::closeEvent(QCloseEvent * event) {
    emit needShow(true);
}

PrintPreviewWindow::~PrintPreviewWindow() {
    delete _ui;
}
