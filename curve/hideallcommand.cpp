#include "curve/pch.h"

#include "hideallcommand.h"

HideAllCommand::HideAllCommand(QString figuresType)
{
    _figuresType = figuresType;
}

void HideAllCommand::run()
{
    Project::instance().hideAllFigures(_figuresType);
}

CommandType HideAllCommand::getType() const
{
    return CommandType::HideAll;
}

QMap<QString, QVariant> HideAllCommand::getParameters() const
{
    return QMap<QString, QVariant>({ { "figuresType", _figuresType } });
}

void HideAllCommand::setParameters(QMap<QString, QVariant> params)
{
    _figuresType = params["figuresType"].toString();
}

QString HideAllCommand::getName() const
{
    return tr("Hide All");
}

QString HideAllCommand::getDescription() const
{
    return "Hide All";
}
