#include "curve/pch.h"

#include "figureeditdialog.h"
#include "ui_figureeditdialog.h"

FigureEditDialog::FigureEditDialog() : _ui(new Ui::FigureEditDialog()) {
    _ui->setupUi(this);

    _project = &Project::instance();
    _doubleValidator = new QDoubleValidator();

    connect(_ui->applyChangesButton, &QPushButton::clicked, this, &FigureEditDialog::applyChanges);

    connect(_ui->colorPB, &QPushButton::clicked, this, &FigureEditDialog::chooseColor);
    connect(_ui->nameLE, &QLineEdit::editingFinished, this, &FigureEditDialog::figureNameChanged);
    connect(_ui->curveTable, &QTableWidget::cellChanged, this, &FigureEditDialog::tableCellChanged);
    connect(_ui->dimTable, &QTableWidget::cellChanged, this, &FigureEditDialog::tableCellChanged);
    //dim
    //connect(this, &FigureEditDialog::dimensionValueChanged, _project, &Project::changeDimensionValue);
    //connect(_project, &Project::dimensionValueChanged, this, &FigureEditDialog::changeDimensionValue);
}

void FigureEditDialog::initialize() {
    _ui->nameLE->blockSignals(true);
    _ui->curveTable->blockSignals(true);
    _ui->dimTable->blockSignals(true);

    _figureName = _project->currentFigureName();
    _figure = _project->findFigure(_figureName);
    setWindowTitle("Edit " + _figureName);
    _ui->nameLE->setText(_figureName);
    auto precision = _project->precision();
    auto &color = _figure->color();
    _ui->colorPB->setStyleSheet("background-color: " + color.name() + ";");

    if(const CurveFigure* curve = dynamic_cast<const CurveFigure*>(_figure)) {
        _ui->container->setCurrentIndex(0);
        resize(605, 470);

        auto points = curve->points();
        auto pointsCount = points.length();
        _ui->curveTable->setRowCount(pointsCount);
        _ui->curveTable->setColumnCount(9);
        _ui->curveTable->setHorizontalHeaderLabels({ "X", "Y", "Z", "I", "J", "K", "Dev.", "UT", "LT" });
        _ui->curveTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        for(auto i = 0; i < pointsCount; i++) {
            auto currentPoint = points[i];
            auto parameters = QVector<double> { currentPoint.x, currentPoint.y, currentPoint.z,
                currentPoint.i, currentPoint.j, currentPoint.k, currentPoint.dev, currentPoint.ut, currentPoint.lt };
            for(auto j = 0; j < _ui->curveTable->columnCount(); j++) {
                auto text = QString::number(parameters[j], 'f', precision);
                QTableWidgetItem* item = new QTableWidgetItem;
                item->setText(text);
                _ui->curveTable->setItem(i, j, item);
            }
        }
    } else if(const PointFigure* point = dynamic_cast<const PointFigure*>(_figure)) { //param A?
        _ui->container->setCurrentIndex(3);
        resize(600, 200);

        auto coordinates = point->point();
        _ui->pointX->setText(QString::number(coordinates.x, 'f', precision));
        _ui->pointY->setText(QString::number(coordinates.y, 'f', precision));
        _ui->pointZ->setText(QString::number(coordinates.z, 'f', precision));
        _ui->pointI->setText(QString::number(coordinates.i, 'f', precision));
        _ui->pointJ->setText(QString::number(coordinates.j, 'f', precision));
        _ui->pointK->setText(QString::number(coordinates.k, 'f', precision));

        validateFields({ _ui->pointX, _ui->pointY, _ui->pointZ, _ui->pointI, _ui->pointJ, _ui->pointK }, _doubleValidator);
    } else if(const CircleFigure* circle = dynamic_cast<const CircleFigure*>(_figure)) {
        _ui->container->setCurrentIndex(1);
        resize(600, 250);

        auto center = circle->center();
        auto normal = circle->normal();

        _ui->circleX->setText(QString::number(center.x, 'f', precision));
        _ui->circleY->setText(QString::number(center.y, 'f', precision));
        _ui->circleZ->setText(QString::number(center.z, 'f', precision));
        _ui->circleI->setText(QString::number(normal.x, 'f', precision));
        _ui->circleJ->setText(QString::number(normal.y, 'f', precision));
        _ui->circleK->setText(QString::number(normal.z, 'f', precision));
        _ui->circleRadius->setText(QString::number(circle->radius(), 'f', precision));

        validateFields({ _ui->circleX, _ui->circleY, _ui->circleZ, _ui->circleI,
            _ui->circleJ, _ui->circleK, _ui->circleRadius }, _doubleValidator);
    } else if(const LineFigure* line = dynamic_cast<const LineFigure*>(_figure)) {
        _ui->container->setCurrentIndex(2);
        resize(600, 250);

        auto origin = line->origin();
        auto direction = line->direction();

        _ui->lineX->setText(QString::number(origin.x, 'f', precision));
        _ui->lineY->setText(QString::number(origin.y, 'f', precision));
        _ui->lineZ->setText(QString::number(origin.z, 'f', precision));
        _ui->lineI->setText(QString::number(direction.x, 'f', precision));
        _ui->lineJ->setText(QString::number(direction.y, 'f', precision));
        _ui->lineK->setText(QString::number(direction.z, 'f', precision));
        _ui->lineLength->setText(QString::number(line->length(), 'f', precision));

        validateFields({ _ui->lineX, _ui->lineY, _ui->lineZ, _ui->lineI, _ui->lineJ, _ui->lineK, _ui->lineLength }, _doubleValidator);
    } else if(const DimFigure* dimension = dynamic_cast<const DimFigure*>(_figure)) {
        _ui->container->setCurrentIndex(4);
        resize(600, 460);

        auto &labelPoint = dimension->labelPoint();
        auto &values = dimension->values();

        _ui->dimX->setText(QString::number(labelPoint.x, 'f', precision));
        _ui->dimY->setText(QString::number(labelPoint.y, 'f', precision));
        _ui->dimZ->setText(QString::number(labelPoint.z, 'f', precision));
        _ui->dimRef1->setText(dimension->firstReference());
        _ui->dimRef2->setText(dimension->secondReference());

        _ui->dimTable->setRowCount(values.length());
        _ui->dimTable->setColumnCount(6);
        _ui->dimTable->setHorizontalHeaderLabels({ "Show", "Type", "Meas", "Nom", "UT", "LT" });
        _ui->dimTable->horizontalHeader()->setStretchLastSection(true);
        fillDimTable(values);

        validateFields({ _ui->dimX, _ui->dimY, _ui->dimZ }, _doubleValidator);
    } else if(const TextFigure* txt = dynamic_cast<const TextFigure*>(_figure)) {
        _ui->container->setCurrentIndex(5);
        resize(510, 230);

        auto position = txt->position();

        _ui->txtContent->setText(txt->text());
        _ui->txtX->setText(QString::number(position.x, 'f', precision));
        _ui->txtY->setText(QString::number(position.y, 'f', precision));
        _ui->zoomSB_2->setValue(txt->textSize());
        _ui->zoomSB->setValue(txt->imageZoom());
        _ui->txtWidth->setText(QString::number(txt->imageWidth(), 'f', precision));
        _ui->txtHeight->setText(QString::number(txt->imageHeight(), 'f', precision));
        _ui->txtRef->setText(txt->reference());

        _ui->txtX->setValidator(_doubleValidator);
        _ui->txtY->setValidator(_doubleValidator);
        _ui->txtWidth->setValidator(_doubleValidator);
        _ui->txtHeight->setValidator(_doubleValidator);
    }
    _tableCellChanged.clear();
    _paramsChanged.clear();

    _ui->nameLE->blockSignals(false);
    _ui->curveTable->blockSignals(false);
    _ui->dimTable->blockSignals(false);

    exec();
}

void FigureEditDialog::validateFields(const QList<QLineEdit*> &fields, const QValidator *validator) {
    for(auto field : fields) {
        field->setValidator(validator);
    }
}

void FigureEditDialog::chooseColor() {
    auto color = QColorDialog::getColor(Qt::black, this, tr("Choose Color"));
    if(color.spec() == QColor::Invalid) {
        return;
    }

    auto colorInt = ColorTranslator::getIntFromColor(&color);
    if(color != _project->findFigure(_figureName)->color()) {
        _paramsChanged.insert("newColor", QString::number(colorInt));
    }
    _ui->colorPB->setStyleSheet("background-color: " + color.name() + ";");
}

const QString FigureEditDialog::convertValueTypeToString(const DimFigure::ValueType &value) {
    return QMetaEnum::fromType<DimFigure::ValueType>().valueToKey(int(value));
}

void FigureEditDialog::fillDimTable(const QVector<DimFigure::Value> &values) {
    QVector<double> parameters;
    for(auto i = 0; i < values.length(); i++) {
        auto &currentValue = values[i];
        parameters = { currentValue.measurement, currentValue.nominal, currentValue.upperTolerance, currentValue.lowerTolerance };
        auto checkBox = new QCheckBox;
        checkBox->setChecked(currentValue.isShow);
        auto checkBoxWidget = new QWidget;
        auto layoutCheckBox = new QHBoxLayout(checkBoxWidget);
        layoutCheckBox->addWidget(checkBox);
        layoutCheckBox->setAlignment(Qt::AlignCenter);
        layoutCheckBox->setContentsMargins(0, 0, 0, 0);
        _ui->dimTable->setCellWidget(i, 0, checkBoxWidget);

        auto tableItem = new QTableWidgetItem(convertValueTypeToString(currentValue.type));
        tableItem->setFlags(tableItem->flags() ^ Qt::ItemIsEditable); //readonly for type
        tableItem->setTextAlignment(Qt::AlignCenter);
        _ui->dimTable->setItem(i, 1, tableItem);

        connect(checkBox, &QCheckBox::stateChanged, this, [this, i]() {
            tableCellChanged(i, 0); }
        );

        for(auto j = 0; j < parameters.size(); j++) {
            tableItem = new QTableWidgetItem(QString::number(parameters[j], 'f', _project->precision()));
            tableItem->setTextAlignment(Qt::AlignCenter);
            _ui->dimTable->setItem(i, j + 2, tableItem);
            if(j == 0) {
                tableItem->setFlags(tableItem->flags() ^ Qt::ItemIsEditable); //readonly for meas
            }
        }
    }
}

void FigureEditDialog::applyChanges() {
    _paramsChanged.insert("figureName", _figureName);
    if(const CurveFigure* curve = dynamic_cast<const CurveFigure*>(_figure)) {
        QString updatedPoint;
        for(auto pointIndex : _tableCellChanged) {
            updatedPoint = "";
            for(auto i = 0; i < _ui->curveTable->columnCount(); i++) {
                updatedPoint += (_ui->curveTable->item(pointIndex, i)->text()) + ',';
            }
            updatedPoint.removeLast();
            _paramsChanged.insert(QString("Point %1").arg(pointIndex + 1), updatedPoint);
        }
    } else if(const PointFigure* point = dynamic_cast<const PointFigure*>(_figure)) {
        _paramsChanged.insert("x", _ui->pointX->text());
        _paramsChanged.insert("y", _ui->pointY->text());
        _paramsChanged.insert("z", _ui->pointZ->text());
        _paramsChanged.insert("i", _ui->pointI->text());
        _paramsChanged.insert("j", _ui->pointJ->text());
        _paramsChanged.insert("k", _ui->pointK->text());
    } else if(const CircleFigure* circle = dynamic_cast<const CircleFigure*>(_figure)) {
        _paramsChanged.insert("x", _ui->circleX->text());
        _paramsChanged.insert("y", _ui->circleY->text());
        _paramsChanged.insert("z", _ui->circleZ->text());
        _paramsChanged.insert("i", _ui->circleI->text());
        _paramsChanged.insert("j", _ui->circleJ->text());
        _paramsChanged.insert("k", _ui->circleK->text());
    } else if(const LineFigure* line = dynamic_cast<const LineFigure*>(_figure)) {
        _paramsChanged.insert("x", _ui->lineX->text());
        _paramsChanged.insert("y", _ui->lineY->text());
        _paramsChanged.insert("z", _ui->lineZ->text());
        _paramsChanged.insert("i", _ui->lineI->text());
        _paramsChanged.insert("j", _ui->lineJ->text());
        _paramsChanged.insert("k", _ui->lineK->text());
        _paramsChanged.insert("Radius", _ui->lineLength->text());
    } else if(const DimFigure* dimension = dynamic_cast<const DimFigure*>(_figure)) {
        _paramsChanged.insert("x", _ui->dimX->text());
        _paramsChanged.insert("y", _ui->dimY->text());
        _paramsChanged.insert("z", _ui->dimZ->text());
        _paramsChanged.insert("Ref1", _ui->dimRef1->text());
        _paramsChanged.insert("Ref2", _ui->dimRef2->text());

        QString updatedValue;
        for(auto valueIndex : _tableCellChanged) {
            updatedValue = "";
            updatedValue += "Type:" + convertValueTypeToString(dimension->values()[valueIndex].type) + ",";
            updatedValue += "Show:";
            updatedValue += _ui->dimTable->cellWidget(valueIndex, 0)->findChild<QCheckBox*>()->isChecked() ? "true," : "false,";
            //updatedValue += "Meas:" + _ui->dimTable->item(valueIndex, 2)->text() + ",";
            updatedValue += "Nom:" + _ui->dimTable->item(valueIndex, 3)->text() + ",";
            updatedValue += "UT:" + _ui->dimTable->item(valueIndex, 4)->text() + ",";
            updatedValue += "LT:" + _ui->dimTable->item(valueIndex, 5)->text();
            _paramsChanged.insert(QString("Dim %1").arg(valueIndex + 1), updatedValue);
        }
    } else if(const TextFigure* txt = dynamic_cast<const TextFigure*>(_figure)) {
        _paramsChanged.insert("text", _ui->txtContent->text());
        _paramsChanged.insert("x", _ui->txtX->text());
        _paramsChanged.insert("y", _ui->txtY->text());
        _paramsChanged.insert("textSize", _ui->zoomSB_2->text());
        _paramsChanged.insert("imageZoom", _ui->zoomSB->text());
        _paramsChanged.insert("reference", _ui->txtRef->text());
    }

    if(_paramsChanged.size() > 1) {
        try {
            _project->editFigure(_figureName, _paramsChanged);
        } catch(...) {
        }
    }
    close();
}

void FigureEditDialog::tableCellChanged(int row, int column) {
    _tableCellChanged << row;
}

void FigureEditDialog::figureNameChanged() {
    if(_ui->nameLE->text() != _figureName) {
        _paramsChanged.insert("newName", _ui->nameLE->text());
    }
}

FigureEditDialog::~FigureEditDialog() {
    delete _doubleValidator;
    delete _ui;
}
