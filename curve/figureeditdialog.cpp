#include "curve/pch.h"
#include "figureeditdialog.h"

FigureEditDialog::FigureEditDialog(Project* mainProject, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::FigureEditDialogClass())
{
    _ui->setupUi(this);

    _project = mainProject;
    _table = _ui->tableWidget;
    _doubleValidator = new QDoubleValidator();
    connect(_project, &Project::figureEditDialogRequested, this, &FigureEditDialog::dialogInitialization);
    connect(_ui->colorPB, &QPushButton::clicked, this, &FigureEditDialog::chooseColor);
    connect(this, &FigureEditDialog::dimensionValueChanged, _project, &Project::changeDimensionValue);
    connect(_project, &Project::dimensionValueChanged, this, &FigureEditDialog::changeDimensionValue);
    connect(_ui->applyChangesButton, &QPushButton::clicked, this, &FigureEditDialog::applyChanges);
    connect(_table, &QTableWidget::cellChanged, this, &FigureEditDialog::tableValueChanged);
    connect(_ui->nameLE, &QLineEdit::editingFinished, this, &FigureEditDialog::figureNameChanged);
}

void FigureEditDialog::dialogInitialization(const QString figureName) {
    _figureName = figureName;
    _figure = _project->findFigure(figureName);
    auto &color = _figure->color();
    auto precision = _project->precision();
    
    setWindowTitle("Edit " + figureName);
    resize(600, 400);

    _ui->nameLE->blockSignals(true);
    _ui->nameLE->setText(figureName);
    _ui->nameLE->blockSignals(false);
    _ui->colorPB->setStyleSheet("background-color: " + color.name() + ";");
    _table->clear();
    _table->hide();

    _table->blockSignals(true);
    if(const CurveFigure* curve = dynamic_cast<const CurveFigure*>(_figure)) {
        _table->show();
        auto points = curve->points();
        auto pointsCount = points.length();
        _table->setRowCount(pointsCount);
        _table->setColumnCount(9);
        _table->setHorizontalHeaderLabels({ "X", "Y", "Z", "I", "J", "K", "Dev.", "UT", "LT" });
        _table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        for(auto i = 0; i < pointsCount; i++) {
            auto currentPoint = points[i];
            auto parameters = QVector<double>{ currentPoint.x, currentPoint.y, currentPoint.z,
                currentPoint.i, currentPoint.j, currentPoint.k, currentPoint.dev, currentPoint.ut, currentPoint.lt };
            for(auto j = 0; j < _table->columnCount(); j++) {
                auto text = QString::number(parameters[j], 'f', precision);
                QTableWidgetItem* item = new QTableWidgetItem;
                item->setText(text);
                _table->setItem(i, j, item);
            }
        }
    } else if(const PointFigure* point = dynamic_cast<const PointFigure*>(_figure)) {
        auto coordinates = point->point();
        addPositionLayout(coordinates.x, coordinates.y, coordinates.z);
        addDirectionLayout(coordinates.i, coordinates.j, coordinates.k);
        addParameterLayout("A", 0, _doubleValidator); // TODO: define parameter A
    } else if(const CircleFigure* circle = dynamic_cast<const CircleFigure*>(_figure)) {
        auto center = circle->center();
        auto normal = circle->normal();
        auto radius = circle->radius();
        addPositionLayout(center.x, center.y, center.z);
        addDirectionLayout(normal.x, normal.y, normal.z);
        addParameterLayout("Radius", QString::number(radius, 'f', precision), _doubleValidator);
    } else if(const LineFigure* line = dynamic_cast<const LineFigure*>(_figure)) {
        auto origin = line->origin();
        auto direction = line->direction();
        auto length = line->length();
        addPositionLayout(origin.x, origin.y, origin.z);
        addDirectionLayout(direction.x, direction.y, direction.z);
        addParameterLayout("Length", QString::number(length, 'f', precision), _doubleValidator);
    } else if(const DimFigure* dimension = dynamic_cast<const DimFigure*>(_figure)) {
        auto &labelPoint = dimension->labelPoint();
        auto firstReference = dimension->firstReference();
        auto secondReference = dimension->secondReference();
        auto &values = dimension->values();

        QStringList figureNames;
        for(auto &figure : _project->figures()) { 
            figureNames.append(figure->name()); 
        }

        addPositionLayout(labelPoint.x, labelPoint.y, labelPoint.z);
        addParameterLayout("Ref. 1", firstReference != nullptr ? firstReference->name() : QString());
        addParameterLayout("Ref. 2", secondReference != nullptr ? secondReference->name() : QString());

        _table->show();
        _table->setRowCount(values.length());
        _table->setColumnCount(6);
        _table->setHorizontalHeaderLabels({ "Show", "Type", "Meas", "Nom", "UT", "LT"});
        _table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        _table->horizontalHeader()->setStretchLastSection(true);
        fillDimTable(values);
    }
    _table->blockSignals(false);
    
    _ui->parametersGB->update();
    _rowsChanged.clear();
    _paramsChanged.clear();
    adjustSize();
    adjustSize();
    exec();
}

void FigureEditDialog::addPositionLayout(double x, double y, double z) {
    auto horizontalLayout = new QHBoxLayout();
    auto label = new QLabel("X,Y,Z:");
    _positionXLE = new QLineEdit(QString::number(x));
    _positionYLE = new QLineEdit(QString::number(y));
    _positionZLE = new QLineEdit(QString::number(z));
    _positionXLE->setValidator(_doubleValidator);
    _positionYLE->setValidator(_doubleValidator);
    _positionZLE->setValidator(_doubleValidator);
    _positionXLE->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _positionYLE->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _positionZLE->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    horizontalLayout->addWidget(label);

    connect(_positionXLE, &QLineEdit::editingFinished, this, [&]() { _paramsChanged.insert("X", _positionXLE->text()); });
    connect(_positionYLE, &QLineEdit::editingFinished, this, [&]() { _paramsChanged.insert("Y", _positionYLE->text()); });
    connect(_positionZLE, &QLineEdit::editingFinished, this, [&]() { _paramsChanged.insert("Z", _positionZLE->text()); });

    horizontalLayout->addWidget(_positionXLE);
    horizontalLayout->addWidget(_positionYLE);
    horizontalLayout->addWidget(_positionZLE);
    _layoutWidgets.push_back(_positionXLE);
    _layoutWidgets.push_back(_positionYLE);
    _layoutWidgets.push_back(_positionZLE);
    
    _layouts.push_back(horizontalLayout);
    _ui->dynamicVL->addLayout(horizontalLayout);
    _layoutWidgets.push_back(label);
}

void FigureEditDialog::addDirectionLayout(double i, double j, double k) {
    auto horizontalLayout = new QHBoxLayout(_ui->parametersGB);
    auto label = new QLabel("I,J,K:");
    _directionILE = new QLineEdit(QString::number(i));
    _directionJLE = new QLineEdit(QString::number(j));
    _directionKLE = new QLineEdit(QString::number(k));
    _directionILE->setValidator(_doubleValidator);
    _directionJLE->setValidator(_doubleValidator);
    _directionKLE->setValidator(_doubleValidator);
    _directionILE->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _directionJLE->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _directionKLE->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    horizontalLayout->addWidget(label);

    connect(_directionILE, &QLineEdit::editingFinished, this, [&]() { _paramsChanged.insert("I", _directionILE->text()); });
    connect(_directionJLE, &QLineEdit::editingFinished, this, [&]() { _paramsChanged.insert("J", _directionJLE->text()); });
    connect(_directionKLE, &QLineEdit::editingFinished, this, [&]() { _paramsChanged.insert("K", _directionKLE->text()); });

    horizontalLayout->addWidget(_directionILE);
    horizontalLayout->addWidget(_directionJLE);
    horizontalLayout->addWidget(_directionKLE);
    _layoutWidgets.push_back(_directionILE);
    _layoutWidgets.push_back(_directionJLE);
    _layoutWidgets.push_back(_directionKLE);

    _layouts.push_back(horizontalLayout);
    _ui->dynamicVL->addLayout(horizontalLayout);
    _layoutWidgets.push_back(label);
}

void FigureEditDialog::addParameterLayout(const QString &name, const QString &value, const QValidator *validator) {
    auto horizontalLayout = new QHBoxLayout(_ui->parametersGB);
    auto label = new QLabel(name + ":");
    _parameterLE = new QLineEdit(value);
    _parameterLE->setValidator(validator);
    _parameterLE->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    horizontalLayout->addWidget(label);

    connect(_parameterLE, &QLineEdit::editingFinished, this, [&]() { _paramsChanged.insert(name, _parameterLE->text()); });
    
    horizontalLayout->addWidget(_parameterLE);

    _layouts.push_back(horizontalLayout);
    _ui->dynamicVL->addLayout(horizontalLayout);
    _layoutWidgets.push_back(label);
}

FigureEditDialog::~FigureEditDialog() {
    delete _doubleValidator;

    for (auto widget : _layoutWidgets) {
        delete widget;
    }

    for (auto layout : _layouts) {
        delete layout;
    }

    delete _table;
    delete _ui;
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

void FigureEditDialog::changeDimensionValue(const QString &dimName, const DimFigure::Value &value) {
    if(dimName != _figureName) return;

    if(auto dimFigure = dynamic_cast<const DimFigure*>(_project->findFigure(dimName))) {
        _table->clear();
        fillDimTable(dimFigure->values());
    }
}

void FigureEditDialog::changeCurrentDimension(const QString &dimName) {

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
        _table->setCellWidget(i, 0, checkBoxWidget);
        connect(checkBox, &QCheckBox::stateChanged, this, [&](int state) {
            auto newValue = currentValue;
            newValue.isShow = state == Qt::Checked;
            emit dimensionValueChanged(_figureName, newValue);
        });

        auto tableItem = new QTableWidgetItem(convertValueTypeToString(currentValue.type));
        tableItem->setTextAlignment(Qt::AlignCenter);
        _table->setItem(i, 1, tableItem);

        for(auto j = 0; j < parameters.size(); j++) {
            tableItem = new QTableWidgetItem(QString::number(parameters[j], 'f', _project->precision()));
            tableItem->setTextAlignment(Qt::AlignCenter);
            _table->setItem(i, j + 2, tableItem);
        }
    }
}

void FigureEditDialog::resetFields() {
    _table->clear();
    _table->hide();
}

void FigureEditDialog::applyChanges() {
    _paramsChanged.insert("figureName", _figureName);

    auto figure = _project->findFigure(_figureName);
    if(auto curveFigure = dynamic_cast<const CurveFigure*>(figure)) {
        QString updatedPoint;
        for(auto pointIndex : _rowsChanged) {
            updatedPoint = "";
            for(auto i = 0; i < _table->columnCount(); i++) {
                updatedPoint += (_table->item(pointIndex, i)->text()) + ',';
            }
            updatedPoint.removeLast();
            _paramsChanged.insert(QString("Point %1").arg(pointIndex + 1), updatedPoint);
        }
    } else if(auto dimFigure = dynamic_cast<const DimFigure*>(figure)) {
        for(auto valueIndex : _rowsChanged) {
            _paramsChanged.insert("Index", QString::number(valueIndex));
            _paramsChanged.insert("isShow", 
                QString::number(_table->cellWidget(valueIndex, 0)->findChild<QCheckBox*>()->isChecked()));
            _paramsChanged.insert("ValueType", convertValueTypeToString(dimFigure->values()[valueIndex].type));
            _paramsChanged.insert("Measurement", _table->item(valueIndex, 2)->text());
            _paramsChanged.insert("Nominal", _table->item(valueIndex, 3)->text());
            _paramsChanged.insert("UT", _table->item(valueIndex, 4)->text());
            _paramsChanged.insert("LT", _table->item(valueIndex, 5)->text());
        }
    }

    if(_paramsChanged.size() > 1) {
        try {
            _project->editFigure(_figureName, _paramsChanged);
        } catch(...) {
        }
    }
    close();
}

void FigureEditDialog::tableValueChanged(int row, int column) {
    _rowsChanged << row;
}

void FigureEditDialog::figureNameChanged() {
    if(_ui->nameLE->text() != _figureName) {
        _paramsChanged.insert("newName", _ui->nameLE->text());
    }
}
