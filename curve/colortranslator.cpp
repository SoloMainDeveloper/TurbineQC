#include "curve/pch.h"

#include "colortranslator.h"

QColor* ColorTranslator::getColorFromInt(const int colorInt) {
    auto red = colorInt % 256;
    auto green = (colorInt / 256) % 256;
    auto blue = (colorInt / 256 / 256) % 256;
    return new QColor(red, green, blue);
}

int ColorTranslator::getIntFromColor(const QColor *color) {
    return color->red() + color->green() * 256 + color->blue() * 256 * 256;
}