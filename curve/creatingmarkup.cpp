#include "curve/pch.h"
#include "creatingmarkup.h"

CurveParts::CurveParts() : CurveParts(QVector<CurvePoint>(), QVector<CurvePoint>(), QVector<CurvePoint>(), QVector<CurvePoint>()) {}

CurveParts::CurveParts(QVector<CurvePoint> pointsOfLE, QVector<CurvePoint> pointsOfTE, QVector<CurvePoint> pointsOfHigh, QVector<CurvePoint> pointsOfLow) {
    _pointsOfLE = pointsOfLE;
    _pointsOfTE = pointsOfTE;
    _pointsOfHigh = pointsOfHigh;
    _pointsOfLow = pointsOfLow;
}

QVector<CurvePoint> CurveParts::pointsOfLE() const {
    return _pointsOfLE;
}

QVector<CurvePoint> CurveParts::pointsOfTE() const {
    return _pointsOfTE;
}

QVector<CurvePoint> CurveParts::pointsOfHigh() const {
    return _pointsOfHigh;
}

QVector<CurvePoint> CurveParts::pointsOfLow() const {
    return _pointsOfLow;
}

CreatingMarkup::CreatingMarkup(std::shared_ptr<ReportSettings> reportSettings, CurveParts &curveParts) {
    _reportSettings = reportSettings;
    _curveParts = curveParts;

    _index = QString("<!DOCTYPE html>\n \
              <html lang=\"en\">\n \
                  <head>\n \
                      <meta charset=\"UTF-8\">\n \
                      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n \
                      <title>%1</title>\n \
                  </head>\n \
                  <body style=\"margin: 0; padding : 0;\">\n \
                      %2\n \
                      <div style=\"width: 90vw; height: 96vh; border: 1px solid; margin: auto; display: grid; grid-template-columns: 1fr 1fr 1.5fr; grid-template-rows: 2.5fr 1.5fr;\">\n \
                          %3\n \
                          %4\n \
                          %5\n \
                          %6\n \
                          %7\n \
                      </div>\n \
                  </body>\n \
              </html>").arg(reportSettings->reportName());

    _tableRowTemplate = "<tr>\n \
                             <td style=\"font-size: 12px;\"><b>%1</b></td>\n \
                         </tr>\n \
                         <tr>\n \
                             <td style=\"font-size: 12px; text-align: center;\"><b>%2</b></td>\n \
                             <td style=\"font-size: 12px; text-align: center;\">%3</td>\n \
                             <td style=\"font-size: 12px; text-align: center;\">%4</td>\n \
                             <td style=\"font-size: 12px; text-align: center;\">%5</td>\n \
                             <td style=\"font-size: 12px; text-align: center;\">%6</td>\n \
                             <td style=\"font-size: 12px; text-align: center;\">%7</td>\n \
                             %8\n \
                         </tr>";
}

void CreatingMarkup::run(Project *project) {
    auto comment = getComment();
    auto globalView = getGlobalView();
    auto parameters = getParameters();
    auto LEView = getLEView();
    auto TEView = getTEView();
    auto partData = getPartData(project);

    QFile file(_reportSettings->reportPath());
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << _index.arg(comment).arg(globalView).arg(parameters).arg(LEView).arg(TEView).arg(partData);
        file.close();
    }
}

QString CreatingMarkup::getComment() {
    auto comment = _reportSettings->comment();
    return QString("<p style=\"width: 90vw; margin: 0; margin:auto;\">%1</p>").arg(comment);
}

QString CreatingMarkup::getGlobalView() {
    auto xShift = _reportSettings->xShift();
    auto yShift = _reportSettings->yShift();
    auto rotation = _reportSettings->rotation();
    auto xShiftCV = _reportSettings->xShiftCV();
    auto yShiftCV = _reportSettings->yShiftCV();
    auto rotationCV = _reportSettings->rotationCV();
    auto xShiftCC = _reportSettings->xShiftCC();
    auto yShiftCC = _reportSettings->yShiftCC();
    auto rotationCC = _reportSettings->rotationCC();
    auto profileType = _reportSettings->profileType();
    auto title = QString("<b>Section %1 - global view</b> Fit: Whole profile (LSQ) / ").arg(_reportSettings->nominalName());
    QString bestFitInfo = "";

    if(_reportSettings->bestFitType() == ReportSettings::BestFitType::OnlyTranslation) {
        title += "Translation";
        if(profileType == ReportSettings::Profile::Whole || profileType == ReportSettings::Profile::WithoutTE || profileType == ReportSettings::Profile::WithoutEdges) {
            bestFitInfo = QString("Best-fit: X: %1, Y: %2")
                .arg(QString::number(xShift, 'f', 3)).arg(QString::number(yShift, 'f', 3));
        } else {
            bestFitInfo = QString("Best-fit: CV - X: %1, Y: %2 / CC - X: %3, Y: %4")
                .arg(QString::number(xShiftCV, 'f', 3)).arg(QString::number(yShiftCV, 'f', 3))
                .arg(QString::number(xShiftCC, 'f', 3)).arg(QString::number(yShiftCC, 'f', 3));
        }
    } else if(_reportSettings->bestFitType() == ReportSettings::BestFitType::OnlyRotation) {
        title += "Rotation";
        if(profileType == ReportSettings::Profile::Whole || profileType == ReportSettings::Profile::WithoutTE || profileType == ReportSettings::Profile::WithoutEdges) {
            bestFitInfo = QString("Best-fit: Rotation: %1").arg(QString::number(rotation, 'f', 3));
        } else {
            bestFitInfo = QString("Best-fit: CV - Rotation: %1 / CC - Rotation: %2")
                .arg(QString::number(rotationCV, 'f', 3)).arg(QString::number(rotationCC, 'f', 3));
        }
    } else if(_reportSettings->bestFitType() == ReportSettings::BestFitType::OnlyXTranslation) {
        title += "X Translation";
        if(profileType == ReportSettings::Profile::Whole || profileType == ReportSettings::Profile::WithoutTE || profileType == ReportSettings::Profile::WithoutEdges) {
            bestFitInfo = QString("Best-fit: X: %1").arg(QString::number(xShift, 'f', 3));
        } else {
            bestFitInfo = QString("Best-fit: CV - X: %1 / CC - X: %2").arg(QString::number(xShiftCV, 'f', 3)).arg(QString::number(xShiftCC, 'f', 3));
        }
    } else if(_reportSettings->bestFitType() == ReportSettings::BestFitType::OnlyYTranslation) {
        title += "Y Translation";
        if(profileType == ReportSettings::Profile::Whole || profileType == ReportSettings::Profile::WithoutTE || profileType == ReportSettings::Profile::WithoutEdges) {
            bestFitInfo = QString("Best-fit: Y: %1").arg(QString::number(yShift, 'f', 3));
        } else {
            bestFitInfo = QString("Best-fit: CV - Y: %1 / CC - Y: %2").arg(QString::number(yShiftCV, 'f', 3)).arg(QString::number(yShiftCC, 'f', 3));
        }
    } else if(_reportSettings->bestFitType() == ReportSettings::BestFitType::XTranslationAndRotation) {
        title += "X Translation and Rotation";
        if(profileType == ReportSettings::Profile::Whole || profileType == ReportSettings::Profile::WithoutTE || profileType == ReportSettings::Profile::WithoutEdges) {
            bestFitInfo = QString("Best-fit: X: %1, Rotation: %2").arg(QString::number(xShift, 'f', 3)).arg(QString::number(rotation, 'f', 3));
        } else {
            bestFitInfo = QString("Best-fit: CV - X: %1, Rotation: %2 / CC - X: %3, Rotation: %4")
                .arg(QString::number(xShiftCV, 'f', 3)).arg(QString::number(rotationCV, 'f', 3))
                .arg(QString::number(xShiftCC, 'f', 3)).arg(QString::number(rotationCC, 'f', 3));
        }
    } else if(_reportSettings->bestFitType() == ReportSettings::BestFitType::YTranslationAndRotation) {
        title += "Y Translation and Rotation";
        if(profileType == ReportSettings::Profile::Whole || profileType == ReportSettings::Profile::WithoutTE || profileType == ReportSettings::Profile::WithoutEdges) {
            bestFitInfo = QString("Best-fit: Y: %1, Rotation: %2").arg(QString::number(yShift, 'f', 3)).arg(QString::number(rotation, 'f', 3));
        } else {
            bestFitInfo = QString("Best-fit: CV - Y: %1, Rotation: %2 / CC - Y: %3, Rotation: %4")
                .arg(QString::number(yShiftCV, 'f', 3)).arg(QString::number(rotationCV, 'f', 3))
                .arg(QString::number(yShiftCC, 'f', 3)).arg(QString::number(rotationCC, 'f', 3));
        }
    } else {
        title += "Translation and Rotation";
        if(profileType == ReportSettings::Profile::Whole || profileType == ReportSettings::Profile::WithoutTE || profileType == ReportSettings::Profile::WithoutEdges) {
            bestFitInfo = QString("Best-fit: X: %1, Y: %2, Rotation: %3")
                .arg(QString::number(xShift, 'f', 3)).arg(QString::number(yShift, 'f', 3)).arg(QString::number(rotation, 'f', 3));
        } else {
            bestFitInfo = QString("Best-fit: CV - X: %1, Y: %2, Rotation: %3 / CC - X: %4, Y: %5, Rotation: %6")
                .arg(QString::number(xShiftCV, 'f', 3)).arg(QString::number(yShiftCV, 'f', 3)).arg(QString::number(rotationCV, 'f', 3))
                .arg(QString::number(xShiftCC, 'f', 3)).arg(QString::number(yShiftCC, 'f', 3)).arg(QString::number(rotationCC, 'f', 3));
        }
    }

    title += QString(" / Error amp: %1x").arg(QString::number(_reportSettings->globalAmplification(), 'f', 0));
    auto convexFormTable = getTable(_curveParts.pointsOfHigh(), QString("Convex_%1").arg(_reportSettings->nominalName()));
    auto concaveFormTable = getTable(_curveParts.pointsOfLow(), QString("Concave_%1").arg(_reportSettings->nominalName()));
    auto encodedImage = getEncodedScreenshot(_reportSettings->screenshotOfGlobal());

    return QString("<div class = \"global-view\" id=\"rectangle\" style=\"border: 1px solid; grid-column: span 2; margin: 5px 2.5px 2.5px 5px;\">\n \
                       <div id=\"rectangle\" style=\"background-color: orange; width: 100%; height: 10%; text-align: center;\">\n \
                           <p style=\"margin: 0;\">%1</p>\n \
                           <p style=\"margin: 0;\">%2</p>\n \
                       </div>\n \
                       <div style=\"position: absolute; display: flex; flex-direction: column; gap: 20px;\">\n \
                           %3\n \
                           %4\n \
                       </div>\n \
                       <img style=\"width: 100%; max-height: 340px; display: block;\" src=\"data:image/png;base64,%5\" alt=\"Global form\">\n \
                    </div>").arg(title).arg(bestFitInfo).arg(convexFormTable).arg(concaveFormTable).arg(encodedImage);
}

QString CreatingMarkup::getParameters() {
    auto maxWidth = _reportSettings->onMaxWidth() ? getMaxWidth() : "";
    auto xMaxWidth = _reportSettings->onXMaxWidth() ? getXMaxWidth() : "";
    auto yMaxWidth = _reportSettings->onYMaxWidth() ? getYMaxWidth() : "";
    auto contactLineLength = _reportSettings->onContactLineLength() ? getContactLineLength() : "";
    auto LEWidth = _reportSettings->onLEWidth() ? getLEWidth() : "";
    auto TEWidth = _reportSettings->onTEWidth() ? getTEWidth() : "";
    auto LERadius = _reportSettings->onLERadius() ? getLERadius() : "";
    auto TERadius = _reportSettings->onTERadius() ? getTERadius() : "";

    return QString("<div class=\"parameters\" id=\"rectangle\" style=\"border: 1px solid; margin: 5px 5px 2.5px 2.5px;\">\n \
                       <div id=\"rectangle\" style=\"background-color: orange; width: 100%; height: 10%; display: flex; align-items: center; justify-content: center;\">\n \
                           <p style=\"margin: 0;\"><b>Airfoil parameters</b></p>\n \
                       </div>\n \
                       <div>\n \
                           <table style=\"width: 100%;\">\n \
                               <tr>\n \
                                   <th style=\"font-size: 14px; text-align: center;\">Axis</th>\n \
                                   <th style=\"font-size: 14px; text-align: center;\">Nominal</th>\n \
                                   <th style=\"font-size: 14px; text-align: center;\">Upper tol.</th>\n \
                                   <th style=\"font-size: 14px; text-align: center;\">Lower tol.</th>\n \
                                   <th style=\"font-size: 14px; text-align: center;\">Measured</th>\n \
                                   <th style=\"font-size: 14px; text-align: center;\">Dev.</th>\n \
                                   <th style=\"font-size: 14px; text-align: center;\">OOT</th>\n \
                               </tr>\n \
                                   %1\n \
                                   %2\n \
                                   %3\n \
                                   %4\n \
                                   %5\n \
                                   %6\n \
                                   %7\n \
                                   %8\n \
                           </table>\n \
                         </div>\n \
                     </div>").arg(maxWidth).arg(xMaxWidth).arg(yMaxWidth).arg(contactLineLength).arg(LEWidth).arg(TEWidth).arg(LERadius).arg(TERadius);
}

QString CreatingMarkup::getMaxWidth() {
    auto title = "Maximum diameter";
    auto type = "Diameter";
    QString oot = "<td></td>";

    if(_reportSettings->nominalMaxWidth() == 0 && _reportSettings->upTolMaxWidth() == 0 && _reportSettings->downTolMaxWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportSettings->measuredMaxWidth(), 'f', 3)).arg("").arg(oot);
    } else if(_reportSettings->upTolMaxWidth() == 0 && _reportSettings->downTolMaxWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalMaxWidth(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportSettings->measuredMaxWidth(), 'f', 3)).arg(QString::number(_reportSettings->deviationMaxWidth(), 'f', 3)).arg(oot);
    } else {
        oot = getOOT(_reportSettings->upTolMaxWidth(), _reportSettings->downTolMaxWidth(), _reportSettings->deviationMaxWidth());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalMaxWidth(), 'f', 3))
            .arg(QString::number(_reportSettings->upTolMaxWidth(), 'f', 3)).arg(QString::number(_reportSettings->downTolMaxWidth(), 'f', 3))
            .arg(QString::number(_reportSettings->measuredMaxWidth(), 'f', 3)).arg(QString::number(_reportSettings->deviationMaxWidth(), 'f', 3)).arg(oot);
    }

}

QString CreatingMarkup::getXMaxWidth() {
    auto title = "XDiameter";
    auto type = "X";
    QString oot = "<td></td>";

    if(_reportSettings->nominalXMaxWidth() == 0 && _reportSettings->upTolXMaxWidth() == 0 && _reportSettings->downTolXMaxWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportSettings->measuredXMaxWidth(), 'f', 3)).arg("").arg(oot);
    } else if(_reportSettings->upTolXMaxWidth() == 0 && _reportSettings->downTolXMaxWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalXMaxWidth(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportSettings->measuredXMaxWidth(), 'f', 3)).arg(QString::number(_reportSettings->deviationXMaxWidth(), 'f', 3)).arg(oot);
    } else {
        auto oot = getOOT(_reportSettings->upTolXMaxWidth(), _reportSettings->downTolXMaxWidth(), _reportSettings->deviationXMaxWidth());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalXMaxWidth(), 'f', 3))
            .arg(QString::number(_reportSettings->upTolXMaxWidth(), 'f', 3)).arg(QString::number(_reportSettings->downTolXMaxWidth(), 'f', 3))
            .arg(QString::number(_reportSettings->measuredXMaxWidth(), 'f', 3)).arg(QString::number(_reportSettings->deviationXMaxWidth(), 'f', 3)).arg(oot);
    }
}

QString CreatingMarkup::getYMaxWidth() {
    auto title = "YDiameter";
    auto type = "Y";
    QString oot = "<td></td>";

    if(_reportSettings->nominalYMaxWidth() == 0 && _reportSettings->upTolYMaxWidth() == 0 && _reportSettings->downTolYMaxWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportSettings->measuredYMaxWidth(), 'f', 3)).arg("").arg(oot);
    } else if(_reportSettings->upTolYMaxWidth() == 0 && _reportSettings->downTolYMaxWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalYMaxWidth(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportSettings->measuredYMaxWidth(), 'f', 3)).arg(QString::number(_reportSettings->deviationYMaxWidth(), 'f', 3)).arg(oot);
    } else {
        auto oot = getOOT(_reportSettings->upTolYMaxWidth(), _reportSettings->downTolYMaxWidth(), _reportSettings->deviationYMaxWidth());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalYMaxWidth(), 'f', 3))
            .arg(QString::number(_reportSettings->upTolYMaxWidth(), 'f', 3)).arg(QString::number(_reportSettings->downTolYMaxWidth(), 'f', 3))
            .arg(QString::number(_reportSettings->measuredYMaxWidth(), 'f', 3)).arg(QString::number(_reportSettings->deviationYMaxWidth(), 'f', 3)).arg(oot);
    }
}

QString CreatingMarkup::getContactLineLength() {
    auto title = "Chord length";
    auto type = "Distance 2D";
    QString oot = "<td></td>";

    if(_reportSettings->nominalContactLineLength() == 0 && _reportSettings->upTolContactLineLength() == 0 && _reportSettings->downTolContactLineLength() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportSettings->measuredContactLineLength(), 'f', 3)).arg("").arg(oot);
    } else if(_reportSettings->upTolContactLineLength() == 0 && _reportSettings->downTolContactLineLength() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalContactLineLength(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportSettings->measuredContactLineLength(), 'f', 3)).arg(QString::number(_reportSettings->deviationMaxWidth(), 'f', 3)).arg(oot);
    } else {
        auto oot = getOOT(_reportSettings->upTolContactLineLength(), _reportSettings->downTolContactLineLength(), _reportSettings->deviationMaxWidth());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalContactLineLength(), 'f', 3))
            .arg(QString::number(_reportSettings->upTolContactLineLength(), 'f', 3)).arg(QString::number(_reportSettings->downTolContactLineLength(), 'f', 3))
            .arg(QString::number(_reportSettings->measuredContactLineLength(), 'f', 3)).arg(QString::number(_reportSettings->deviationMaxWidth(), 'f', 3)).arg(oot);
    }
}

QString CreatingMarkup::getLEWidth() {
    auto title = "LE width";
    auto type = "Distance 2D";
    QString oot = "<td></td>";

    if(_reportSettings->nominalLEWidth() == 0 && _reportSettings->upTolLEWidth() == 0 && _reportSettings->downTolLEWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportSettings->measuredLEWidth(), 'f', 3)).arg("").arg(oot);
    } else if(_reportSettings->upTolLEWidth() == 0 && _reportSettings->downTolLEWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalLEWidth(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportSettings->measuredLEWidth(), 'f', 3)).arg(QString::number(_reportSettings->deviationLEWidth(), 'f', 3)).arg(oot);
    } else {
        auto oot = getOOT(_reportSettings->upTolLEWidth(), _reportSettings->downTolLEWidth(), _reportSettings->deviationLEWidth());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalLEWidth(), 'f', 3))
            .arg(QString::number(_reportSettings->upTolLEWidth(), 'f', 3)).arg(QString::number(_reportSettings->downTolLEWidth(), 'f', 3))
            .arg(QString::number(_reportSettings->measuredLEWidth(), 'f', 3)).arg(QString::number(_reportSettings->deviationLEWidth(), 'f', 3)).arg(oot);
    }
}

QString CreatingMarkup::getTEWidth() {
    auto title = "TE width";
    auto type = "Distance 2D";
    QString oot = "<td></td>";

    if(_reportSettings->nominalTEWidth() == 0 && _reportSettings->upTolTEWidth() == 0 && _reportSettings->downTolTEWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportSettings->measuredTEWidth(), 'f', 3)).arg("").arg(oot);
    } else if(_reportSettings->upTolTEWidth() == 0 && _reportSettings->downTolTEWidth() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalTEWidth(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportSettings->measuredTEWidth(), 'f', 3)).arg(QString::number(_reportSettings->deviationTEWidth(), 'f', 3)).arg(oot);
    } else {
        auto oot = getOOT(_reportSettings->upTolTEWidth(), _reportSettings->downTolTEWidth(), _reportSettings->deviationTEWidth());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalTEWidth(), 'f', 3))
            .arg(QString::number(_reportSettings->upTolTEWidth(), 'f', 3)).arg(QString::number(_reportSettings->downTolTEWidth(), 'f', 3))
            .arg(QString::number(_reportSettings->measuredTEWidth(), 'f', 3)).arg(QString::number(_reportSettings->deviationTEWidth(), 'f', 3)).arg(oot);
    }
}

QString CreatingMarkup::getLERadius() {
    auto title = "LE radius";
    auto type = "Radius";
    QString oot = "<td></td>";

    if(_reportSettings->nominalLERadius() == 0 && _reportSettings->upTolLERadius() == 0 && _reportSettings->downTolLERadius() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportSettings->measuredLERadius(), 'f', 3)).arg("").arg(oot);
    } else if(_reportSettings->upTolLERadius() == 0 && _reportSettings->downTolLERadius() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalLERadius(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportSettings->measuredLERadius(), 'f', 3)).arg(QString::number(_reportSettings->deviationLERadius(), 'f', 3)).arg(oot);
    } else {
        auto oot = getOOT(_reportSettings->upTolLERadius(), _reportSettings->downTolLERadius(), _reportSettings->deviationLERadius());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalLERadius(), 'f', 3))
            .arg(QString::number(_reportSettings->upTolLERadius(), 'f', 3)).arg(QString::number(_reportSettings->downTolLERadius(), 'f', 3))
            .arg(QString::number(_reportSettings->measuredLERadius(), 'f', 3)).arg(QString::number(_reportSettings->deviationLERadius(), 'f', 3)).arg(oot);
    }
}

QString CreatingMarkup::getTERadius() {
    auto title = "TE radius";
    auto type = "Radius";
    QString oot = "<td></td>";

    if(_reportSettings->nominalTERadius() == 0 && _reportSettings->upTolTERadius() == 0 && _reportSettings->downTolTERadius() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(_reportSettings->measuredTERadius(), 'f', 3)).arg("").arg(oot);
    } else if(_reportSettings->upTolTERadius() == 0 && _reportSettings->downTolTERadius() == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalTERadius(), 'f', 3)).arg("").arg("")
            .arg(QString::number(_reportSettings->measuredTERadius(), 'f', 3)).arg(QString::number(_reportSettings->deviationTERadius(), 'f', 3)).arg(oot);
    } else {
        auto oot = getOOT(_reportSettings->upTolTERadius(), _reportSettings->downTolTERadius(), _reportSettings->deviationTERadius());
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(_reportSettings->nominalTERadius(), 'f', 3))
            .arg(QString::number(_reportSettings->upTolTERadius(), 'f', 3)).arg(QString::number(_reportSettings->downTolTERadius(), 'f', 3))
            .arg(QString::number(_reportSettings->measuredTERadius(), 'f', 3)).arg(QString::number(_reportSettings->deviationTERadius(), 'f', 3)).arg(oot);
    }
}

QString CreatingMarkup::getOOT(double upperTolerance, double downTolerance, double deviation) {
    auto difference = 0.0;
    if(deviation >= downTolerance && deviation <= upperTolerance) {
        auto step = (upperTolerance - downTolerance) / 10;
        return QString("<td><input type=\"range\" min=\"%1\" max=\"%2\" step=\"%3\" value=\"%4\" \
            style=\"width: 40px; height: 4px; accent-color: green; pointer-events: none; vertical-align:middle;\"></td>")
            .arg(downTolerance).arg(upperTolerance).arg(step).arg(deviation);
    } else {
        if(deviation < downTolerance) {
            difference = deviation - downTolerance;
        } else {
            difference = deviation - upperTolerance;
        }
        return QString("<td style=\"font-size: 14px; text-align: center; color: red;\">%1</td>\n")
            .arg(QString::number(difference, 'f', 3));
    }
}

QString CreatingMarkup::getLEView() {
    QString title = "<p style=\"margin: 0;\"><b>LE - local view</b> Fit: No fit / Error amp: %1x</p>";
    title = title.arg(QString::number(_reportSettings->amplificationOfLE(), 'f', 0));
    auto table = getTable(_curveParts.pointsOfLE(), QString("LE_%1").arg(_reportSettings->nominalName()));
    auto encodedImage = getEncodedScreenshot(_reportSettings->screenshotOfLE());
    auto imgHTMLCode = QString("<img style=\"width: 100%; height: 90%; display: block;\" src=\"data:image/png;base64,%1\" alt=\"Leading edge form\">").arg(encodedImage);
    auto result = QString("<div class=\"LE-view\" id=\"rectangle\" style=\"border: 1px solid; margin: 2.5px 2.5px 5px 5px;\">\n \
                       <div id=\"rectangle\" style=\"background-color: orange; width: 100%; height: 10%; display: flex; align-items: center; justify-content: center;\">\n \
                           %1\n \
                       </div>\n \
                       <div style=\"position: absolute;\">\n \
                            %2\n \
                        </div>\n \
                       %3\n \
                    </div>");

    if(_reportSettings->isLEVisible()) {
        return result.arg(title).arg(table).arg(imgHTMLCode);
    } else {
        return result.arg("").arg("").arg("");
    }
}

QString CreatingMarkup::getTEView() {
    QString title = "<p style=\"margin: 0;\"><b>TE - local view</b> Fit: No fit / Error amp: %1x</p>";
    title = title.arg(QString::number(_reportSettings->amplificationOfTE(), 'f', 0));
    auto table = getTable(_curveParts.pointsOfTE(), QString("TE_%1").arg(_reportSettings->nominalName()), "style=\"position: absolute\"");
    auto encodedImage = getEncodedScreenshot(_reportSettings->screenshotOfTE());
    auto imgHTMLCode = QString("<img style=\"width: 100%; height: 90%; display: block;\" src=\"data:image/png;base64,%1\" alt=\"Trailing edge form\">").arg(encodedImage);
    auto result = QString("<div class=\"TE-view\" id=\"rectangle\" style=\"border: 1px solid; margin: 2.5px 2.5px 5px 2.5px;\">\n \
                        <div id=\"rectangle\" style=\"background-color: orange; width: 100%; height: 10%; display: flex; align-items: center; justify-content: center;\">\n \
                            %1\n \
                        </div>\n \
                        <div style=\"position: relative; display: flex; justify-content: right;\">\n \
                            %2\n \
                        </div>\n \
                        %3\n \
                    </div>");
    if(_reportSettings->isTEVisible()) {
        return result.arg(title).arg(table).arg(imgHTMLCode);
    } else {
        return result.arg("").arg("").arg("");
    }
}

QString CreatingMarkup::getPartData(Project *project) {
    auto time = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss");
    return QString("<div class=\"part-data\" id=\"rectangle\" style=\"margin: 2.5px 5px 5px 2.5px; display: grid; grid-template-rows: 0.6fr 0.4fr; gap: 5px;\">\n \
                        <div id=\"rectangle\" style=\"border: 1px solid;\">\n \
                            <div class=\"header\" id=\"rectangle\" style=\"background-color: orange; width: 100%; height: 18%; display: flex; align-items: center; justify-content: center;\">\n \
                                <p style=\"margin: 0;\"><b>Part data</b></p>\n \
                            </div>\n \
                            <div class=\"body\" style=\"width: 100%; height: 82%; display: grid; grid-template-columns: 1fr 1fr; grid-template-rows: 1fr 1fr 1fr;\">\n \
                                <div style=\"align-content: center; margin-left: 5%;\"><b>Description: %1</b></div>\n \
                                <div style=\"align-content: center; margin-left: 5%;\"><b>Part number: %2</b></div>\n \
                                <div style=\"align-content: center; margin-left: 5%;\"><b>Drawing: %3</b></div>\n \
                                <div style=\"align-content: center; margin-left: 5%;\"><b>Operator: %4</b></div>\n \
                                <div style=\"align-content: center; margin-left: 5%;\"><b>Order number: %5</b></div>\n \
                                <div style=\"align-content: center; margin-left: 5%;\"><b>Note: %6</b></div>\n \
                            </div>\n \
                        </div>\n \
                        <div id=\"rectangle\" style=\"border: 1px solid; display: flex; justify-content: center; align-items: center;\">\n \
                            <p style=\"margin: 0;\"><b>Time: %7</b></p>\n \
                        </div>\n \
                    </div>").arg(project->description()).arg(project->partNumber()).arg(project->drawing()).
        arg(project->projectOperator()).arg(project->orderNumber()).arg(project->note()).arg(time);
}

QString CreatingMarkup::getTable(const QVector<CurvePoint> &points, const QString &caption, const QString &style) {
    auto typesOfTableValue = _reportSettings->typesOfTableValue();
    QVector<double> deviations;
    for(auto &point : points) {
        deviations.append(point.dev);
    }
    auto result = QString(
        "<table %1>\n \
            <caption style=\"font-size: 14px;\"><b>%2</b></caption>\n \
            %3\n \
            %4\n \
            %5\n \
            %6\n \
            %7\n \
            %8\n \
            %9\n \
        </table>");
    auto tableRowTemplate = QString(
        "<tr>\n \
            <td style=\"text-align: center; font-size: 10px;\">%1:</td>\n \
            <td style=\"text-align: center; font-size: 10px;\">%2</td>\n \
        </tr>");
    QString minMaxPart = "";
    QString formPart = "";
    QString minPart = "";
    QString maxPart = "";
    QString maxAbsPart = "";
    QString supUTPart = "";
    QString infLTPart = "";

    auto min = *std::min_element(deviations.begin(), deviations.end());
    auto max = *std::max_element(deviations.begin(), deviations.end());
    for(auto [type, isNeed] : typesOfTableValue.asKeyValueRange()) {
        if(type == DimFigure::ValueType::MinMax && isNeed) {
            auto minMax = abs(min) + abs(max);
            minMaxPart = tableRowTemplate.arg("MinMax").arg(QString::number(minMax, 'f', 3));
        } else if(type == DimFigure::ValueType::Form && isNeed) {
            auto form = std::max(abs(min), abs(max)) * 2;
            formPart = tableRowTemplate.arg("Form").arg(QString::number(form, 'f', 3));
        } else if(type == DimFigure::ValueType::Min && isNeed) {
            minPart = tableRowTemplate.arg("Min").arg(QString::number(min, 'f', 3));
        } else if(type == DimFigure::ValueType::Max && isNeed) {
            maxPart = tableRowTemplate.arg("Max").arg(QString::number(max, 'f', 3));
        } else if(type == DimFigure::ValueType::MaxAbs && isNeed) {
            auto maxAbs = std::max(abs(min), abs(max));
            maxAbsPart = tableRowTemplate.arg("MaxAbs").arg(QString::number(maxAbs, 'f', 3));
        } else if(type == DimFigure::ValueType::SupUT && isNeed) {
            auto supUT = max;
            supUTPart = tableRowTemplate.arg("SupUT").arg(QString::number(supUT, 'f', 3));
        } else if(type == DimFigure::ValueType::InfLT && isNeed) {
            auto infLT = min;
            infLTPart = tableRowTemplate.arg("InfLT").arg(QString::number(infLT, 'f', 3));
        }
    }
    return result.arg(style).arg(caption).arg(minMaxPart).arg(formPart).arg(minPart).arg(maxPart).arg(maxAbsPart).arg(supUTPart).arg(infLTPart);
}

QString CreatingMarkup::getEncodedScreenshot(const QImage &screenshot) {
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    screenshot.save(&buffer, "PNG");
    buffer.close();

    return QString::fromUtf8(byteArray.toBase64());
}

CreatingMarkup::~CreatingMarkup() {}
