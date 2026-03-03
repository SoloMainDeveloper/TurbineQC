#pragma once

#include "reportsettings.h"

class FunctionParamsGenerator {
public:
    static Function4Params params4(Project *project, std::shared_ptr<ReportSettings> reportSettings, const Function18Params *params18, bool isClosed = false);
    static Function6Params params6(std::shared_ptr<ReportSettings> reportSettings, bool isClosed = false);
    static Function18Params params18(Project *project, std::shared_ptr<ReportSettings> reportSettings);
    static Function21Params params21(std::shared_ptr<ReportSettings> reportSettings, bool isClosed = false);
};
