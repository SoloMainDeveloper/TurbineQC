#pragma once

#include "icommand.h"

class ChangeFigureVisibilityCommand : public ICommand {
public:
	ChangeFigureVisibilityCommand() = default;
	ChangeFigureVisibilityCommand(QString figureName, bool isVisible);

	void run() override;
	CommandType getType() const override;
	QMap<QString, QVariant> getParameters() const override;
	void setParameters(QMap<QString, QVariant> params) override;
	QString getName() const override;
	QString getDescription() const override;

private:
	QString _figureName;
	bool _isVisible;
};
