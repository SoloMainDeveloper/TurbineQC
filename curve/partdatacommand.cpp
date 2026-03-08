#include "curve/pch.h"

#include "partdatacommand.h"

PartDataCommand::PartDataCommand(
    QString reportTitle, QString description, QString drawing,
    QString orderNumber, QString partNumber, QString projectOperator,
    QString note, QString machine, QString tool, QString fixturing,
    QString batch, QString supplier, QString revision, bool needShowWindowWhenMacroRuns) {
    _reportTitle = reportTitle;
    _description = description;
    _drawing = drawing;
    _orderNumber = orderNumber;
    _partNumber = partNumber;
    _projectOperator = projectOperator;
    _note = note;
    _machine = machine;
    _tool = tool;
    _fixturing = fixturing;
    _batch = batch;
    _supplier = supplier;
    _revision = revision;
    _needShowWindowWhenMacroRuns = needShowWindowWhenMacroRuns;
}

void PartDataCommand::run() {
    Project::instance().changePartData(_reportTitle, _description, _drawing, _orderNumber, _partNumber, _projectOperator,
        _note, _machine, _tool, _fixturing, _batch, _supplier, _revision, _needShowWindowWhenMacroRuns);
}

CommandType PartDataCommand::getType() const {
    return CommandType::PartData;
}

QMap<QString, QVariant> PartDataCommand::getParameters() const {
    return {
        { "reportTitle", _reportTitle },
        { "description", _description },
        { "drawing", _drawing },
        { "orderNumber", _orderNumber },
        { "partNumber", _partNumber },
        { "projectOperator", _projectOperator },
        { "note", _note },
        { "machine", _machine },
        { "tool", _tool },
        { "fixturing", _fixturing },
        { "batch", _batch },
        { "supplier", _supplier },
        { "revision", _revision },
        { "showPartDataWindowWhenMacroRuns", _needShowWindowWhenMacroRuns }
    };
}

void PartDataCommand::setParameters(QMap<QString, QVariant> params) {
    _reportTitle = params["reportTitle"].toString();
    _description = params["description"].toString();
    _drawing = params["drawing"].toString();
    _orderNumber = params["orderNumber"].toString();
    _partNumber = params["partNumber"].toString();
    _projectOperator = params["projectOperator"].toString();
    _note = params["note"].toString();
    _machine = params["machine"].toString();
    _tool = params["tool"].toString();
    _fixturing = params["fixturing"].toString();
    _batch = params["batch"].toString();
    _supplier = params["supplier"].toString();
    _revision = params["revision"].toString();
    _needShowWindowWhenMacroRuns = params["needShowWindowWhenMacroRuns"].toBool();
}

QString PartDataCommand::getName() const {
    return "Данные детали";
}

QString PartDataCommand::getDescription() const {
    return "Part Data";
}
