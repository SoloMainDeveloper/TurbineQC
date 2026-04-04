#pragma once

#include "functionparams.h"

class Project;
class ReportSettings;

class FunctionParamsFactory {
public:
    explicit FunctionParamsFactory(const std::shared_ptr<ReportSettings> reportSettings);
    ~FunctionParamsFactory() = default;

    Function4Params params4(const Function18Params& params18, bool isClosed = false);

    Function6Params params6ForGlobalFit();
    Function6Params params6ForLocalFit();

    Function18Params params18();

    Function21Params params21ForGlobalFit();
    Function21Params params21ForLocalFit();

private:
    Project* _project;
    std::shared_ptr<ReportSettings> _reportSettings;

    Function6Params createParams6UsingOption(bool needMinimize, bool isClosed, Function6Params::Method method);
    Function21Params createParams21UsingOption(bool isClosed, int limInterpMethod);

    template<class T>
    void setConstraintsToParams(T& functionParams);
};
