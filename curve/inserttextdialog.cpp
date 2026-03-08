#include "curve/pch.h"

#include "inserttextdialog.h"
#include "ui_inserttextdialog.h"

InsertTextDialog::InsertTextDialog() : _ui(new Ui::InsertTextDialog()) {
    _ui->setupUi(this);

    _ui->xLE->setValidator(new QDoubleValidator);
    _ui->yLE->setValidator(new QDoubleValidator);
    _ui->sizeSB->setMinimum(0);
    _ui->zoomSB->setMinimum(0);

    connect(_ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &InsertTextDialog::insertText);
    connect(_ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &InsertTextDialog::reject);
    connect(_ui->imagePB, &QPushButton::clicked, this, &InsertTextDialog::openImage);
    connect(_ui->selectPB, &QPushButton::clicked, this, &InsertTextDialog::selectCoords);
}

InsertTextDialog::~InsertTextDialog() {
    delete _ui;
}

void InsertTextDialog::initialize() {
    _ui->refsLW->clear();
    _ui->nameLE->setText("TXT_" + QString::number(findFreeIndex()));
    _ui->textLE->clear();
    _ui->sizeSB->setValue(0);
    _ui->zoomSB->setValue(1);
    _ui->le_width->setPlaceholderText("6");
    _ui->le_height->setPlaceholderText("4.5");
    _ui->xLE->setText("");
    _ui->yLE->setText("");
    auto &refFigures = Project::instance().figures();
    for(auto &figure : refFigures) {
        if(dynamic_cast<TextFigure*>(figure) || dynamic_cast<DimFigure*>(figure)) {
            continue;
        } else {
            _ui->refsLW->addItem(figure->name());
        }
    }

    show();
}

void InsertTextDialog::onPlotClick(const QPointF &point) {
    if(_pointSelecting == false) return;
    _pointSelecting = false;
    const auto &precision = Project::instance().precision();
    _ui->xLE->setText(QString::number(point.x(), 'f', precision));
    _ui->yLE->setText(QString::number(point.y(), 'f', precision));
    raise();
}

int InsertTextDialog::findFreeIndex() {
    auto &textFigures = Project::instance().textFigures();
    auto index = 1;
    auto flag = true;
    while(flag) {
        flag = false;
        for(auto figure : textFigures) {
            if(figure->name() == "TXT_" + QString::number(index)) {
                index += 1;
                flag = true;
                break;
            }
        }
        if(!flag) {
            return index;
        }
    }
    return 0;
}

void InsertTextDialog::insertText() {
    auto text = _ui->textLE->text();
    if(text.isEmpty()) {
        auto warning = QMessageBox::warning(this, "No text entered", "Enter text", "Ok");
        return;
    }
    auto name = _ui->nameLE->text();
    if(name.isEmpty()) {
        name = "TXT_" + QString::number(findFreeIndex());
    }
    auto x = 0.0;
    auto y = 0.0;
    if(_ui->xLE->text().isEmpty() == false) x = _ui->xLE->text().toDouble();
    if(_ui->yLE->text().isEmpty() == false) y = _ui->yLE->text().toDouble();

    QString referenceName;
    if(auto ref = _ui->refsLW->currentItem()) {
        referenceName = ref->text();
    }
    auto size = _ui->sizeSB->value();
    auto zoom = _ui->zoomSB->value();

    double width = 6;
    double height = 4.5;
    if(_ui->le_width->text().isEmpty() == false) width = _ui->le_width->text().toDouble();
    if(_ui->le_height->text().isEmpty() == false) height = _ui->le_height->text().toDouble();

    Project::instance().constructText(name, text, x, y, size, referenceName, width, height, zoom);
    accept();
}

void InsertTextDialog::openImage() {
    auto filePath = QFileDialog::getOpenFileName(nullptr, "Open file", "", "Images (*.png *.jpg *.jpeg *.bmp);; All files (*)");
    if(filePath.isEmpty()) {
        return;
    }
    auto pixmap = QPixmap(filePath);
    if(pixmap.isNull()) {
        QMessageBox::warning(this, "Error", "Image not found");
        return;
    }
    _ui->textLE->setText("IMG:" + filePath);
}

void InsertTextDialog::selectCoords() {
    _pointSelecting = true;
    lower();
}