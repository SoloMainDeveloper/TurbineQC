#pragma once

#include "figure.h"

class Project {
public:
    const QList<Figure*>& figures() const;
    Figure* findFigure(const QString &name) const;
    void insertFigure(Figure* figure);
    void renameFigure(const QString &name, const QString &newName);
    void removeFigure(const QString &name);

private:
    QList<Figure*> _figures;
};