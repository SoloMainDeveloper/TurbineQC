#define _USE_MATH_DEFINES
#define QCUSTOMPLOT_USE_LIBRARY

#include <cmath> // before Qt

#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QContextMenuEvent>
#include <QDialog>
#include <QDir>
#include <QDoubleValidator>
#include <QInputDialog>
#include <QIntValidator>
#include <QLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QTreeWidgetItem>
#include <QWidget>
#include <QFuture>
#include <QtConcurrent>
#include "qcustomplot.h"
#include "macrosmanager.h"
#include <Eigen/Dense>
#include "continuableexception.h"