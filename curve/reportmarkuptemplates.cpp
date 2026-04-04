#include "curve/pch.h"

#include "reportmarkuptemplates.h"

QString ReportMarkupTemplates::indexTemplate = R"(
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title></title>
    </head>
    <body style="margin: 0; padding : 0;">
        <main style="width: 90vw; height: 90vh; margin: auto;">
            %1
        </main> 
    </body>
</html>
)";

QString ReportMarkupTemplates::reportPageTemplate = R"(
            %1
            <div style="margin-bottom: 20px; padding: 5px; display: grid; gap: 5px; 
                grid-template-columns: 1fr 1fr 1.5fr; grid-template-rows: 2fr 1fr; border: 1px solid;">
                %2
                %3
                %4
                %5
                %6
            </div>
)";

QString ReportMarkupTemplates::commentTemplate = R"(
            <p style="margin: 0; padding: 0;">%1</p>
)";

QString ReportMarkupTemplates::globalViewTemplate = R"(
                <div class = "global-view" id="rectangle" style="border: 1px solid; grid-column: span 2; overflow: hidden;">
                    <div id="rectangle" style="background-color: orange; width: 100%; height: 10%; 
                        display: flex; flex-direction: column; align-items: center; justify-content: center;">
                        %1
                    </div>
                    <div style="position: absolute; display: flex; flex-direction: column; gap: 20px;">
                        %2
                        %3
                    </div>
                    <img style="width: 100%; height: auto; object-fit: cover; 
                        display: block;" src="data:image/png;base64,%4" alt="Global form">
                </div>
)";

QString ReportMarkupTemplates::globalTitleTemplate = R"(
                        <p style="margin: 0;">Section %1 - Global Fit: %2 / %3 / Error amp: %4x</p>
                        <p style="margin: 0;">%5</p>
)";

QString ReportMarkupTemplates::bestFitValuesTemplate = R"(Best-fit: X: %1, Y: %2, Rotation: %3)";

QString ReportMarkupTemplates::bestFitDualValuesTemplate = R"(Best-fit: CV - X: %1, Y: %2, Rotation: %3 / CC - X: %4, Y: %5, Rotation: %6)";

QString ReportMarkupTemplates::leadingEdgeViewTemplate = R"(
                <div class="LE-view" id="rectangle" style="border: 1px solid; overflow: hidden;">
                    <div id="rectangle" style="background-color: orange; width: 100%; height: 10%; 
                        display: flex; align-items: center; justify-content: center;">
                        %1
                    </div>
                    <div style="position: absolute;">
                        %2
                    </div>
                    %3
                </div>
)";

QString ReportMarkupTemplates::leadingEdgeTitleTemplate = R"(
                        <p style="margin: 0;">LE - Local Fit: %1 / Error amp: %2x</p>
)";

QString ReportMarkupTemplates::leadingEdgeImageTemplate = R"(
                    <img style="width: 100%; height: auto; display: block;" src="data:image/png;base64,%1" alt="Leading edge form">
)";

QString ReportMarkupTemplates::trailingEdgeViewTemplate = R"(
                <div class="TE-view" id="rectangle" style="border: 1px solid; overflow: hidden;">
                    <div id="rectangle" style="background-color: orange; width: 100%; height: 10%; 
                        display: flex; align-items: center; justify-content: center;">
                        %1
                    </div>
                    <div style="position: absolute;">
                        %2
                    </div>
                    %3
                </div>
)";

QString ReportMarkupTemplates::trailingEdgeTitleTemplate = R"(
                        <p style="margin: 0;">TE - Local Fit: %1 / Error amp: %2x</p>
)";

QString ReportMarkupTemplates::trailingEdgeImageTemplate = R"(
                    <img style="width: 100%; height: auto; display: block;" src="data:image/png;base64,%1" alt="Trailing edge form">
)";

QString ReportMarkupTemplates::statisticsTableTemplate = R"(
                        <table>
                            <caption style="font-size: 14px;"><b>%1</b></caption>
                            %2
                        </table>
)";

QString ReportMarkupTemplates::statisticsTableRowTemplate = R"(
                            <tr>
                                <td style="text-align: center; font-size: 10px;">%1:</td>
                                <td style="text-align: center; font-size: 10px;">%2</td>
                            </tr>
)";

QString ReportMarkupTemplates::parameterTableTemplate = R"(
                <div class="parameters" id="rectangle" style="border: 1px solid;">
                    <div id="rectangle" style="background-color: orange; width: 100%; height: 10%; 
                        display: flex; align-items: center; justify-content: center;">
                        <p style="margin: 0;">Airfoil parameters</p>
                    </div>
                    <div>
                        <table style="width: 100%;">
                            <tr>
                                <th style="font-size: 14px; text-align: center;">Axis</th>
                                <th style="font-size: 14px; text-align: center;">Nominal</th>
                                <th style="font-size: 14px; text-align: center;">Upper tol.</th>
                                <th style="font-size: 14px; text-align: center;">Lower tol.</th>
                                <th style="font-size: 14px; text-align: center;">Measured</th>
                                <th style="font-size: 14px; text-align: center;">Dev.</th>
                                <th style="font-size: 14px; text-align: center;">OOT</th>
                            </tr>
                            %1
                       </table>
                     </div>
                 </div>
)";

QString ReportMarkupTemplates::parameterTableRowTitleTemplate = R"(
                            <tr>
                                <td style="font-size: 12px;"><b>%1</b></td>
                            </tr>
)";

QString ReportMarkupTemplates::parameterTableRowTemplate = R"(
                            <tr>
                                <td style="font-size: 12px; text-align: center;"><b>%1</b></td>
                                <td style="font-size: 12px; text-align: center;">%2</td>
                                <td style="font-size: 12px; text-align: center;">%3</td>
                                <td style="font-size: 12px; text-align: center;">%4</td>
                                <td style="font-size: 12px; text-align: center;">%5</td>
                                <td style="font-size: 12px; text-align: center;">%6</td>
                                %8
                            </tr>
)";

QString ReportMarkupTemplates::defaultOutOfToleranceTemplate = R"(
                                <td></td>
)";

QString ReportMarkupTemplates::valueInToleranceTemplate = R"(
                                <td style="text-align: center;">
                                    <input type="range" min="%1" max="%2" step="%3" value="%4"
                                    style="width: 40px; height: 4px; accent-color: green; pointer-events: none; vertical-align:middle;">
                                </td>
)";

QString ReportMarkupTemplates::valueOutOfToleranceTemplate = R"(
                                <td style="font-size: 14px; text-align: center; color: red;">
                                    %1
                                </td>
)";

QString ReportMarkupTemplates::partDataTemplate = R"(
                <div class="part-data" id="rectangle" style="display: grid; gap: 5px; grid-template-rows: 0.6fr 0.4fr;">
                    <div id="rectangle" style="border: 1px solid;">
                        <div class="header" id="rectangle" style="background-color: orange; width: 100%; height: 18%; 
                            display: flex; align-items: center; justify-content: center;">
                            <p style="margin: 0;">Part data</p>
                        </div>
                        <div class="info" style="width: 100%; height: 82%; 
                            display: grid; grid-template-columns: 1fr 1fr; grid-template-rows: 1fr 1fr 1fr;">
                            <div style="align-content: center; margin-left: 5%;"><b>Description: %1</b></div>
                            <div style="align-content: center; margin-left: 5%;"><b>Part number: %2</b></div>
                            <div style="align-content: center; margin-left: 5%;"><b>Drawing: %3</b></div>
                            <div style="align-content: center; margin-left: 5%;"><b>Operator: %4</b></div>
                            <div style="align-content: center; margin-left: 5%;"><b>Order number: %5</b></div>
                            <div style="align-content: center; margin-left: 5%;"><b>Note: %6</b></div>
                        </div>
                    </div>
                    <div id="rectangle" style="border: 1px solid; display: flex; justify-content: center; align-items: center;">
                        <p style="margin: 0;"><b>Time: %7</b></p>
                    </div>
                </div>
)";
