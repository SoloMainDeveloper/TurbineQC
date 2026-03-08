#pragma once

#include "icommand.h"

class MacrosTranslator : public QObject {
    Q_OBJECT

public:
    enum OperationCRM {
        LoadCRV, PartData, CollectCross, Regen2D, Alignment,
        ShiftCurves1, RotateCurves1, BestFit2D, Airfoil, InsertBFPos,
        ShowOptions, EditDim, SaveFLR, PrinterSettings, PrintFromViewer, ImportQDS
    };
    Q_ENUM(OperationCRM);

    static QStringList splitCRM(const QString &text);
    static OperationCRM operationCRMFromString(QString macrosType);
    static QList<std::shared_ptr<ICommand>>* translateCRM(const QString &operationText);
    static std::shared_ptr<ICommand> parseCRMCommand(OperationCRM type, QStringList operationText);
};
