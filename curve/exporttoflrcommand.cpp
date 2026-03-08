#include "curve/pch.h"

#include "exporttoflrcommand.h"
#include "filesystem.h"

ExportToFLRCommand::ExportToFLRCommand(QString filepath, QStringList curvesToTake) {
    _filepath = filepath;
    _curvesToTake = curvesToTake;
}

void ExportToFLRCommand::run() {
    FileSystem::exportToFLR(_filepath, &_curvesToTake);
}

CommandType ExportToFLRCommand::getType() const {
    return CommandType::ExportToFLR;
}

QMap<QString, QVariant> ExportToFLRCommand::getParameters() const {
    QVariantList curves;
    for(QString curve : _curvesToTake) {
        curves.append(curve);
    }
    return {
        { "filepath", _filepath },
        { "curves", curves },
    };
}

void ExportToFLRCommand::setParameters(QMap<QString, QVariant> params) {
    _filepath = params["filepath"].toString();
    if(params.contains("curves")) {
        QVariantList curves = params["curves"].toList();
        _curvesToTake.clear();
        for(const QVariant& v : curves) {
            _curvesToTake.append(v.toString());
        }
    }
}

QString ExportToFLRCommand::getName() const {
    return "Экспорт в FLR";
}

QString ExportToFLRCommand::getDescription() const {
    return "Export to FLR";
}
