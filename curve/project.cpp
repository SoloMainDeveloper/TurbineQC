#include "curve/pch.h"
#include "project.h"

const QList<Figure*>& Project::figures() const {
    return _figures;
}

Figure * Project::findFigure(const QString &name) const {
    for(auto i = 0; i < _figures.length(); i++) {
        if(_figures[i]->name() == name) {
            return _figures[i];
        }
    }
    return nullptr;
}

void Project::insertFigure(Figure * figure) {
    _figures.append(figure);
}

void Project::renameFigure(const QString &name, const QString &newName) {
    findFigure(name)->setName(newName);
}

void Project::removeFigure(const QString &name) {
    _figures.removeAt(_figures.indexOf(findFigure(name)));
}
