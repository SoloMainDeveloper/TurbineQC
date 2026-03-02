#include "curve/pch.h"
#include "markupcreator.h"

const QString MarkupCreator::index = "<!DOCTYPE html>\n \
                                       <html lang=\"en\">\n \
                                           <head>\n \
                                               <meta charset=\"UTF-8\">\n \
                                               <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n \
                                               <title></title>\n \
                                           </head>\n \
                                           <body style=\"margin: 0; padding : 0;\">\n \
                                               <main style=\"width: 90vw; height: 90vh; margin: auto;\">\n \
                                                   %1\
                                               </main> \n \
                                           </body>\n \
                                       </html>";

MarkupCreator::MarkupCreator(Project *project, std::shared_ptr<ReportSettings> reportSettings) : _project(project), _reportSettings(reportSettings) {
    _reportTemplate = "%1\n \
                       <div style=\"margin-bottom: 20px; padding: 5px; display: grid; gap: 5px; grid-template-columns: 1fr 1fr 1.5fr; grid-template-rows: 2fr 1fr; border: 1px solid;\">\n \
                           %2\n \
                           %3\n \
                           %4\n \
                           %5\n \
                           %6\n \
                       </div>";

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

QString MarkupCreator::run(const GlobalCurveMap &analyzedGlobalCurves) {
    _creationTime = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss");

    auto comment = getCommentMarkup();
    auto globalView = getGlobalViewMarkup(analyzedGlobalCurves);
    auto parameters = getParametersMarkup();
    auto LEView = getViewLEMarkup(analyzedGlobalCurves);
    auto TEView = getViewTEMarkup(analyzedGlobalCurves);
    auto partData = getPartDataMarkup();

    return _reportTemplate.arg(comment).arg(globalView).arg(parameters).arg(LEView).arg(TEView).arg(partData);
}

QString MarkupCreator::getCommentMarkup() {
    auto comment = _reportSettings->comment();
    return QString("<p style=\"margin: 0; padding: 0;\">%1</p>").arg(comment);
}

QString MarkupCreator::getGlobalViewMarkup(const GlobalCurveMap &analyzedGlobalCurves) {
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
    auto [nameCV, pointsCV] = analyzedGlobalCurves[CurveType::GlobalCV];
    auto [nameCC, pointsCC] = analyzedGlobalCurves[CurveType::GlobalCC];
    auto convexFormTable = getTableMarkup(pointsCV, QString("Convex_%1").arg(_reportSettings->nominalName()));
    auto concaveFormTable = getTableMarkup(pointsCC, QString("Concave_%1").arg(_reportSettings->nominalName()));
    auto encodedImage = getEncodedScreenshot(_reportSettings->screenshotOfGlobal());

    return QString("<div class = \"global-view\" id=\"rectangle\" style=\"border: 1px solid; grid-column: span 2; overflow: hidden;\">\n \
                       <div id=\"rectangle\" style=\"background-color: orange; width: 100%; height: 10%; text-align: center;\">\n \
                           <p style=\"margin: 0;\">%1</p>\n \
                           <p style=\"margin: 0;\">%2</p>\n \
                       </div>\n \
                       <div style=\"position: absolute; display: flex; flex-direction: column; gap: 20px;\">\n \
                           %3\n \
                           %4\n \
                       </div>\n \
                       <img style=\"width: 100%; height: auto; object-fit: cover; display: block;\" src=\"data:image/png;base64,%5\" alt=\"Global form\">\n \
                    </div>").arg(title).arg(bestFitInfo).arg(convexFormTable).arg(concaveFormTable).arg(encodedImage);
}

QString MarkupCreator::getParametersMarkup() {
    auto maxWidth = _reportSettings->needCalculateParam(TurbineParamType::MaxWidth) ? getMaxWidthMarkup() : "";
    auto xMaxWidth = _reportSettings->needCalculateParam(TurbineParamType::MaxWidthX) ? getXMaxWidthMarkup() : "";
    auto yMaxWidth = _reportSettings->needCalculateParam(TurbineParamType::MaxWidthY) ? getYMaxWidthMarkup() : "";
    auto contactLineLength = _reportSettings->needCalculateParam(TurbineParamType::ContactLineLength) ? getContactLineLengthMarkup() : "";
    auto LEWidth = _reportSettings->needCalculateParam(TurbineParamType::LEWidth) ? getWidthLEMarkup() : "";
    auto TEWidth = _reportSettings->needCalculateParam(TurbineParamType::TEWidth) ? getWidthTEMarkup() : "";
    auto LERadius = _reportSettings->needCalculateParam(TurbineParamType::LERadius) ? getRadiusLEMarkup() : "";
    auto TERadius = _reportSettings->needCalculateParam(TurbineParamType::TERadius) ? getRadiusTEMarkup() : "";
    auto minX = _reportSettings->needCalculateParam(TurbineParamType::MinX) ? getMinXMarkup() : "";

    return QString("<div class=\"parameters\" id=\"rectangle\" style=\"border: 1px solid;\">\n \
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
                                   %9\n \
                           </table>\n \
                         </div>\n \
                     </div>").arg(maxWidth, xMaxWidth, yMaxWidth, contactLineLength, LEWidth, TEWidth, LERadius, TERadius, minX);
}

QString MarkupCreator::getMaxWidthMarkup() {
    auto params = _reportSettings->getTurbineParameter(TurbineParamType::MaxWidth);
    QString result;
    for(auto param : params)
        result += createParameterMarkup(param, "Maximum diameter", "Diameter");
    return result;
}

QString MarkupCreator::getXMaxWidthMarkup() {
    auto params = _reportSettings->getTurbineParameter(TurbineParamType::MaxWidthX);
    QString result;
    for(auto param : params)
        result += createParameterMarkup(param, "XDiameter", "X");
    return result;
}

QString MarkupCreator::getYMaxWidthMarkup() {
    auto params = _reportSettings->getTurbineParameter(TurbineParamType::MaxWidthY);
    QString result;
    for(auto param : params)
        result += createParameterMarkup(param, "YDiameter", "Y");
    return result;
}

QString MarkupCreator::getContactLineLengthMarkup() {
    auto params = _reportSettings->getTurbineParameter(TurbineParamType::ContactLineLength);
    QString result;
    for(auto param : params)
        result += createParameterMarkup(param, "Chord length", "Distance 2D");
    return result;
}

QString MarkupCreator::getWidthLEMarkup() {
    auto params = _reportSettings->getTurbineParameter(TurbineParamType::LEWidth);
    QString result;
    for(auto param : params)
        result += createParameterMarkup(param, "LE width", "Distance 2D");
    return result;
}

QString MarkupCreator::getWidthTEMarkup() {
    auto params = _reportSettings->getTurbineParameter(TurbineParamType::TEWidth);
    QString result;
    for(auto param : params)
        result += createParameterMarkup(param, "TE width", "Distance 2D");
    return result;
}

QString MarkupCreator::getRadiusLEMarkup() {
    auto params = _reportSettings->getTurbineParameter(TurbineParamType::LERadius);
    QString result;
    for(auto param : params)
        result += createParameterMarkup(param, "LE radius", "Radius");
    return result;
}

QString MarkupCreator::getRadiusTEMarkup() {
    auto params = _reportSettings->getTurbineParameter(TurbineParamType::TERadius);
    QString result;
    for(auto param : params)
        result += createParameterMarkup(param, "TE radius", "Radius");
    return result;
}

QString MarkupCreator::getMinXMarkup() {
    auto params = _reportSettings->getTurbineParameter(TurbineParamType::MinX);
    QString result;
    for(auto param : params)
        result += createParameterMarkup(param, "MinX", "X");
    return result;
}

QString MarkupCreator::createParameterMarkup(ReportSettings::TurbineParameter param, QString title, QString type) {
    QString oot = "<td></td>";
    auto deviation = param.measured - param.nominal;
    if(param.nominal == 0 && param.UT == 0 && param.LT == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg("").arg("").arg("")
            .arg(QString::number(param.measured, 'f', 3)).arg("").arg(oot);
    } else if(param.UT == 0 && param.LT == 0) {
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(param.nominal, 'f', 3)).arg("").arg("")
            .arg(QString::number(param.measured, 'f', 3)).arg(QString::number(deviation, 'f', 3)).arg(oot);
    } else {
        oot = getOOTMarkup(param.UT, param.LT, deviation);
        return _tableRowTemplate.arg(title).arg(type).arg(QString::number(param.nominal, 'f', 3))
            .arg(QString::number(param.UT, 'f', 3)).arg(QString::number(param.LT, 'f', 3))
            .arg(QString::number(param.measured, 'f', 3)).arg(QString::number(deviation, 'f', 3)).arg(oot);
    }
}

QString MarkupCreator::getOOTMarkup(double upperTolerance, double downTolerance, double deviation) {
    auto difference = 0.0;
    if(deviation >= downTolerance && deviation <= upperTolerance) {
        auto step = (upperTolerance - downTolerance) / 10;
        return QString("<td style=\"text-align: center;\">\n \
                            <input type=\"range\" min=\"%1\" max=\"%2\" step=\"%3\" value=\"%4\" \
                            style=\"width: 40px; height: 4px; accent-color: green; pointer-events: none; vertical-align:middle;\">\n \
                        </td>").arg(downTolerance).arg(upperTolerance).arg(step).arg(deviation);
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

QString MarkupCreator::getViewLEMarkup(const GlobalCurveMap &analyzedGlobalCurves) {
    auto result = QString("<div class=\"LE-view\" id=\"rectangle\" style=\"border: 1px solid; overflow: hidden;\">\n \
                               <div id=\"rectangle\" style=\"background-color: orange; width: 100%; height: 10%; display: flex; align-items: center; justify-content: center;\">\n \
                                   %1\n \
                               </div>\n \
                               <div style=\"position: absolute;\">\n \
                                    %2\n \
                               </div>\n \
                               %3\n \
                           </div>");
    QString title = "";
    QString table = "";
    QString imgHTMLCode = "";
    if(_reportSettings->isLEVisible()) {
        title = "<p style=\"margin: 0;\"><b>LE - local view</b> Fit: %1 / Error amp: %2x</p>";
        auto edgeBestFit = _reportSettings->bestFitOfLE() == ReportSettings::EdgeBestFit::GlobalFit ? "Use global fit" : "No fit";
        auto amplification = QString::number(_reportSettings->amplificationOfLE(), 'f', 0);
        title = title.arg(edgeBestFit).arg(amplification);
        auto [name, points] = analyzedGlobalCurves[CurveType::GlobalLE];
        table = getTableMarkup(points, QString("LE_%1").arg(_reportSettings->nominalName()));
        auto encodedImage = getEncodedScreenshot(_reportSettings->screenshotOfLE());
        imgHTMLCode = QString("<img style=\"width: 100%; height: auto; display: block;\" src=\"data:image/png;base64,%1\" alt=\"Leading edge form\">").arg(encodedImage);
    }
    return result.arg(title).arg(table).arg(imgHTMLCode);
}

QString MarkupCreator::getViewTEMarkup(const GlobalCurveMap &analyzedGlobalCurves) {
    auto result = QString("<div class=\"TE-view\" id=\"rectangle\" style=\"border: 1px solid; overflow: hidden;\">\n \
                               <div id=\"rectangle\" style=\"background-color: orange; width: 100%; height: 10%; display: flex; align-items: center; justify-content: center;\">\n \
                                   %1\n \
                               </div>\n \
                               <div style=\"position: relative; display: flex; justify-content: right;\">\n \
                                   %2\n \
                               </div>\n \
                               %3\n \
                           </div>");
    QString title = "";
    QString table = "";
    QString imgHTMLCode = "";
    if(_reportSettings->isTEVisible()) {
        title = "<p style=\"margin: 0;\"><b>TE - local view</b> Fit: %1 / Error amp: %2x</p>";
        auto edgeBestFit = _reportSettings->bestFitOfTE() == ReportSettings::EdgeBestFit::GlobalFit ? "Use global fit" : "No fit";
        auto amplification = QString::number(_reportSettings->amplificationOfTE(), 'f', 0);
        title = title.arg(edgeBestFit).arg(amplification);
        auto [name, points] = analyzedGlobalCurves[CurveType::GlobalTE];
        table = getTableMarkup(points, QString("TE_%1").arg(_reportSettings->nominalName()), "style=\"position: absolute\"");
        auto encodedImage = getEncodedScreenshot(_reportSettings->screenshotOfTE());
        imgHTMLCode = QString("<img style=\"width: 100%; height: auto; display: block;\" src=\"data:image/png;base64,%1\" alt=\"Trailing edge form\">").arg(encodedImage);
    }
    return result.arg(title).arg(table).arg(imgHTMLCode);
}

QString MarkupCreator::getPartDataMarkup() {
    return QString("<div class=\"part-data\" id=\"rectangle\" style=\"display: grid; gap: 5px; grid-template-rows: 0.6fr 0.4fr;\">\n \
                        <div id=\"rectangle\" style=\"border: 1px solid;\">\n \
                            <div class=\"header\" id=\"rectangle\" style=\"background-color: orange; width: 100%; height: 18%; display: flex; align-items: center; justify-content: center;\">\n \
                                <p style=\"margin: 0;\"><b>Part data</b></p>\n \
                            </div>\n \
                            <div class=\"info\" style=\"width: 100%; height: 82%; display: grid; grid-template-columns: 1fr 1fr; grid-template-rows: 1fr 1fr 1fr;\">\n \
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
                    </div>").arg(_project->description()).arg(_project->partNumber()).arg(_project->drawing()).
        arg(_project->projectOperator()).arg(_project->orderNumber()).arg(_project->note()).arg(_creationTime);
}

QString MarkupCreator::getTableMarkup(const QVector<CurvePoint> &points, const QString &caption, const QString &style) {
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

    auto outputFormMode = _reportSettings->outputFormMode();
    const auto formValuesNumber = 7;
    const auto base = 2;
    auto binCode = QString("%1").arg(outputFormMode, formValuesNumber, base, QChar('0'));

    auto min = *std::min_element(deviations.begin(), deviations.end());
    auto max = *std::max_element(deviations.begin(), deviations.end());
    if(QString(binCode[6]).toInt()) {
        auto minMax = abs(max - min);
        minMaxPart = tableRowTemplate.arg("MinMax").arg(QString::number(minMax, 'f', 3));
    }
    if(QString(binCode[5]).toInt()) {
        auto form = std::max(abs(min), abs(max)) * 2;
        formPart = tableRowTemplate.arg("Form").arg(QString::number(form, 'f', 3));
    }
    if(QString(binCode[4]).toInt()) {
        minPart = tableRowTemplate.arg("Min").arg(QString::number(min, 'f', 3));
    }
    if(QString(binCode[3]).toInt()) {
        maxPart = tableRowTemplate.arg("Max").arg(QString::number(max, 'f', 3));
    }
    if(QString(binCode[2]).toInt()) {
        auto maxAbs = std::max(abs(min), abs(max));
        maxAbsPart = tableRowTemplate.arg("MaxAbs").arg(QString::number(maxAbs, 'f', 3));
    }
    if(QString(binCode[1]).toInt()) {
        auto supUT = max > 0 ? max : 0;
        supUTPart = tableRowTemplate.arg("SupUT").arg(QString::number(supUT, 'f', 3));
    }
    if(QString(binCode[0]).toInt()) {
        auto infLT = min < 0 ? min : 0;
        infLTPart = tableRowTemplate.arg("InfLT").arg(QString::number(infLT, 'f', 3));
    }
    return result.arg(style).arg(caption).arg(minMaxPart).arg(formPart)
        .arg(minPart).arg(maxPart).arg(maxAbsPart).arg(supUTPart).arg(infLTPart);
}

QString MarkupCreator::getEncodedScreenshot(const QImage &screenshot) {
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    screenshot.save(&buffer, "PNG");
    buffer.close();

    return QString::fromUtf8(byteArray.toBase64());
}

QString MarkupCreator::reportCreationTime() {
    return _creationTime;
}
