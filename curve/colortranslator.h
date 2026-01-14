#pragma once
#include "pch.h"

class ColorTranslator {
public:
    static QColor* getColorFromInt(const int colorInt);
    static int getIntFromColor(const QColor *color);
};

