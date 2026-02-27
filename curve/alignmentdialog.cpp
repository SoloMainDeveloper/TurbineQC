#include "curve/pch.h"
#include "alignmentdialog.h"

AlignmentDialog::AlignmentDialog(Project *project, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::AlignmentDialogClass())
{
    _ui->setupUi(this);

    _project = project; 

    _ui->axisCB->addItem("+X");
    _ui->axisCB->addItem("-X");
    _ui->axisCB->addItem("+Y");
    _ui->axisCB->addItem("-Y");

    _ui->angleLE->setValidator(new QDoubleValidator);
    _ui->offsetXLE->setValidator(new QDoubleValidator);
    _ui->offsetYLE->setValidator(new QDoubleValidator);

    connect(_ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &AlignmentDialog::applyAlignment);
    connect(_ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &AlignmentDialog::reject);
    connect(_ui->rotateLineCB, &QComboBox::textActivated, this, &AlignmentDialog::changeAngle);
    connect(_ui->axisCB, &QComboBox::textActivated, this, &AlignmentDialog::changeAngle);
    connect(_ui->originXCB, &QComboBox::textActivated, this, &AlignmentDialog::changeOffsetX);
    connect(_ui->originYCB, &QComboBox::textActivated, this, &AlignmentDialog::changeOffsetY);
    connect(_ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &AlignmentDialog::changeAngle);
    connect(_ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &AlignmentDialog::changeOffsetX);
    connect(_ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &AlignmentDialog::changeOffsetY);
    connect(this, &AlignmentDialog::alignmentRequested, _project, static_cast<void (Project::*) (QString, QString, QString, QString)> (&Project::alignment));

    connect(_ui->angleLE, &QLineEdit::textChanged, this, [&] {_angleMacrosFlag = false; _ui->rotateLineCB->setCurrentIndex(-1); });
    connect(_ui->offsetXLE, &QLineEdit::textChanged, this, [&] {_offsetXMacrosFlag = false; _ui->originXCB->setCurrentIndex(-1); });
    connect(_ui->offsetYLE, &QLineEdit::textChanged, this, [&] {_offsetYMacrosFlag = false; _ui->originYCB->setCurrentIndex(-1); });
}

void AlignmentDialog::initialization() {
    _ui->rotateLineCB->blockSignals(true);
    _ui->axisCB->blockSignals(true);
    _ui->originXCB->blockSignals(true);
    _ui->originYCB->blockSignals(true);
    _ui->angleLE->setText("0");
    _ui->rotateLineCB->clear();
    _ui->axisCB->setCurrentText("+X");
    _ui->originXCB->clear();
    _ui->originYCB->clear();
    _ui->offsetXLE->setText("0");
    _ui->offsetYLE->setText("0");

    auto figures = _project->figures();
    for(auto &figure : figures) {
        if(dynamic_cast<LineFigure*>(figure)) {
            _ui->rotateLineCB->addItem(figure->name());
            _ui->originXCB->addItem(figure->name());
            _ui->originYCB->addItem(figure->name());
        } else if(dynamic_cast<PointFigure*>(figure)) {
            _ui->originXCB->addItem(figure->name());
            _ui->originYCB->addItem(figure->name());
        } else if(dynamic_cast<CircleFigure*>(figure)) {
            _ui->originXCB->addItem(figure->name());
            _ui->originYCB->addItem(figure->name());
        }
    }

    _ui->rotateLineCB->setCurrentIndex(-1);
    _ui->originXCB->setCurrentIndex(-1);
    _ui->originYCB->setCurrentIndex(-1);

    _ui->rotateLineCB->blockSignals(false);
    _ui->axisCB->blockSignals(false);
    _ui->originXCB->blockSignals(false);
    _ui->originYCB->blockSignals(false);

    adjustSize();
    show();
}

void AlignmentDialog::applyAlignment() {
    auto angle = _angleMacrosFlag ? _ui->rotateLineCB->currentText() : _ui->angleLE->text();
    auto axis = _ui->axisCB->currentText();
    auto offsetX = _offsetXMacrosFlag ? _ui->originXCB->currentText() : _ui->offsetXLE->text();
    auto offsetY = _offsetYMacrosFlag ? _ui->originYCB->currentText() : _ui->offsetYLE->text();

    emit alignmentRequested(angle, axis, offsetX, offsetY);
}

void AlignmentDialog::changeAngle() {
    if(_ui->rotateLineCB->currentIndex() == -1 || _ui->rotateLineCB->currentText().isEmpty()) {
        return;
    }

    auto line = dynamic_cast<const LineFigure*>(_project->findFigure(_ui->rotateLineCB->currentText()));
    Eigen::Vector2d lineDir(line->direction().x, line->direction().y);

    Eigen::Vector2d axis;
    int sign = 1;
    if(_ui->axisCB->currentText() == "+X") {
        axis << 1, 0;
        if(lineDir.y() > 0) {
            sign = -1;
        }
    } else if(_ui->axisCB->currentText() == "-X") {
        axis << -1, 0;
        if(lineDir.y() < 0) {
            sign = -1;
        }
    } else if(_ui->axisCB->currentText() == "+Y") {
        axis << 0, 1;
        if(lineDir.x() < 0) {
            sign = -1;
        }
    } else if(_ui->axisCB->currentText() == "-Y") {
        axis << 0, -1;
        if(lineDir.x() > 0) {
            sign = -1;
        }
    }

    auto cos = axis.dot(lineDir) / (axis.norm() * lineDir.norm());
    auto angle = sign * std::acos(cos) * 180 / M_PI;

    _ui->angleLE->blockSignals(true);
    _ui->angleLE->setText(QString::number(angle, 'f', 6));
    _ui->angleLE->blockSignals(false);

    _angleMacrosFlag = true;
}

void AlignmentDialog::changeOffsetX() {
    if(_ui->originXCB->currentIndex() == -1 || _ui->originXCB->currentText().isEmpty()) {
        return;
    }

    auto figure = _project->findFigure(_ui->originXCB->currentText());
    double offsetX;
    if(auto line = dynamic_cast<const LineFigure*>(figure)) {
        offsetX = -line->origin().x;
    } else if(auto circle = dynamic_cast<const CircleFigure*>(figure)) {
        offsetX = -circle->center().x;
    } else if(auto point = dynamic_cast<const PointFigure*>(figure)) {
        offsetX = -point->point().x;
    }

    _ui->offsetXLE->blockSignals(true);
    _ui->offsetXLE->setText(QString::number(offsetX, 'f', 6));
    _ui->offsetXLE->blockSignals(false);

    _offsetXMacrosFlag = true;
}

void AlignmentDialog::changeOffsetY() {
    if(_ui->originYCB->currentIndex() == -1 || _ui->originYCB->currentText().isEmpty()) {
        return;
    }

    auto figure = _project->findFigure(_ui->originYCB->currentText());
    double offsetY;
    if(auto line = dynamic_cast<const LineFigure*>(figure)) {
        offsetY = -line->origin().y;
    } else if(auto circle = dynamic_cast<const CircleFigure*>(figure)) {
        offsetY = -circle->center().y;
    } else if(auto point = dynamic_cast<const PointFigure*>(figure)) {
        offsetY = -point->point().y;
    }

    _ui->offsetYLE->blockSignals(true);
    _ui->offsetYLE->setText(QString::number(offsetY, 'f', 6));
    _ui->offsetYLE->blockSignals(false);

    _offsetYMacrosFlag = true;
}

AlignmentDialog::~AlignmentDialog()
{
    delete _ui;
}
