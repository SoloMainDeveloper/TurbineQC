#pragma once

enum class CommandType {
    LoadCloud, //DONE
    LoadProject, //DONE
    SaveProject, //YET
    ClearProject, //YET
    ConstantTolerance,
    EdgesTolerance,
    CreateWidthOfLE,
    CreateWidthOfTE,
    MergeScans, //YET
    RadiusCorrection, //YET
    CalculateDeviations,
    BestFit, //YET
    CreateDimension,
    InsertBestFitPosition,
    RenameFigure, //DONE
    RemoveFigure, //YET
    FigureVisibilityChanged,
    CreateReport, //YET
    ChangeFigureColor,
    ExportToFLR, //YET
    ShiftFigure, //YET
    RotateFigure, //YET
    Alignment, //YET
    CalculateCurve,
    RegenerateCurve, //YET
    EditFigure,
    CreateMaxCircle,
    CreateMiddleCurve,
    CreateContactLine,
    ChangeFigureVisibility,
    ChangeCurveParameters,
    ChangeDimensionParameters,
    PartData, //YET
    InsertText,
    PrintReport, //YET
    ClearReport,
    RemovePage,
    SetPrinterSettings, //YET
    ShowAll,
    HideAll,
    CompareFLR,
    Unknown,
};

static QString commandTypeToString(CommandType type) {
    switch(type) {
        case CommandType::LoadCloud: return "LoadCloud";
        case CommandType::LoadProject: return "LoadProject";
        case CommandType::SaveProject: return "SaveProject";
        case CommandType::ClearProject: return "ClearProject";
        case CommandType::ConstantTolerance: return "ConstantTolerance";
        case CommandType::EdgesTolerance: return "EdgesTolerance";
        case CommandType::CreateWidthOfLE: return "CreateWidthOfLE";
        case CommandType::CreateWidthOfTE: return "CreateWidthOfTE";
        case CommandType::MergeScans: return "MergeScans";
        case CommandType::RadiusCorrection: return "RadiusCorrection";
        case CommandType::CalculateDeviations: return "CalculateDeviations";
        case CommandType::BestFit: return "BestFit";
        case CommandType::CreateDimension: return "CreateDimension";
        case CommandType::InsertBestFitPosition: return "InsertBestFitPosition";
        case CommandType::RenameFigure: return "RenameFigure";
        case CommandType::RemoveFigure: return "RemoveFigure";
        case CommandType::FigureVisibilityChanged: return "FigureVisibilityChanged";
        case CommandType::CreateReport: return "CreateReport";
        case CommandType::ChangeFigureColor: return "ChangeFigureColor";
        case CommandType::ExportToFLR: return "ExportToFLR";
        case CommandType::ShiftFigure: return "ShiftFigure";
        case CommandType::RotateFigure: return "RotateFigure";
        case CommandType::Alignment: return "Alignment";
        case CommandType::CalculateCurve: return "CalculateCurve";
        case CommandType::RegenerateCurve: return "RegenerateCurve";
        case CommandType::EditFigure: return "EditFigure";
        case CommandType::CreateMaxCircle: return "CreateMaxCircle";
        case CommandType::CreateMiddleCurve: return "CreateMiddleCurve";
        case CommandType::CreateContactLine: return "CreateContactLine";
        case CommandType::ChangeFigureVisibility: return "ChangeFigureVisibility";
        case CommandType::ChangeCurveParameters: return "ChangeCurveParameters";
        case CommandType::ChangeDimensionParameters: return "ChangeDimensionParameters";
        case CommandType::PartData: return "PartData";
        case CommandType::InsertText: return "InsertText";
        case CommandType::PrintReport: return "PrintReport";
        case CommandType::ClearReport: return "ClearReport";
        case CommandType::RemovePage: return "RemovePage";
        case CommandType::SetPrinterSettings: return "SetPrinterSettings";
        case CommandType::ShowAll: return "ShowAll";
        case CommandType::HideAll: return "HideAll";
        case CommandType::CompareFLR: return "CompareFLR";
        case CommandType::Unknown: return "Unknown";
        default: return "Unknown";
    }
}

static CommandType stringToCommandType(const QString& str) {
    static QMap<QString, CommandType> map = {
        { "LoadCloud", CommandType::LoadCloud },
        { "LoadProject", CommandType::LoadProject },
        { "SaveProject", CommandType::SaveProject },
        { "ClearProject", CommandType::ClearProject },
        { "ConstantTolerance", CommandType::ConstantTolerance },
        { "EdgesTolerance", CommandType::EdgesTolerance },
        { "CreateWidthOfLE", CommandType::CreateWidthOfLE },
        { "CreateWidthOfTE", CommandType::CreateWidthOfTE },
        { "MergeScans", CommandType::MergeScans },
        { "RadiusCorrection", CommandType::RadiusCorrection },
        { "CalculateDeviations", CommandType::CalculateDeviations },
        { "BestFit", CommandType::BestFit },
        { "CreateDimension", CommandType::CreateDimension },
        { "InsertBestFitPosition", CommandType::InsertBestFitPosition },
        { "RenameFigure", CommandType::RenameFigure },
        { "RemoveFigure", CommandType::RemoveFigure },
        { "FigureVisibilityChanged", CommandType::FigureVisibilityChanged },
        { "CreateReport", CommandType::CreateReport },
        { "ChangeFigureColor", CommandType::ChangeFigureColor },
        { "ExportToFLR", CommandType::ExportToFLR },
        { "ShiftFigure", CommandType::ShiftFigure },
        { "RotateFigure", CommandType::RotateFigure },
        { "Alignment", CommandType::Alignment },
        { "CalculateCurve", CommandType::CalculateCurve },
        { "RegenerateCurve", CommandType::RegenerateCurve },
        { "EditFigure", CommandType::EditFigure },
        { "CreateMaxCircle", CommandType::CreateMaxCircle },
        { "CreateMiddleCurve", CommandType::CreateMiddleCurve },
        { "CreateContactLine", CommandType::CreateContactLine },
        { "ChangeFigureVisibility", CommandType::ChangeFigureVisibility },
        { "ChangeCurveParameters", CommandType::ChangeCurveParameters },
        { "ChangeDimensionParameters", CommandType::ChangeDimensionParameters },
        { "PartData", CommandType::PartData },
        { "InsertText", CommandType::InsertText },
        { "PrintReport", CommandType::PrintReport },
        { "ClearReport", CommandType::ClearReport },
        { "RemovePage", CommandType::RemovePage },
        { "SetPrinterSettings", CommandType::SetPrinterSettings },
        { "ShowAll", CommandType::ShowAll },
        { "HideAll", CommandType::HideAll },
        { "CompareFLR", CommandType::CompareFLR },
        { "Unknown", CommandType::Unknown }
    };
    return map.value(str, CommandType::Unknown);
}
