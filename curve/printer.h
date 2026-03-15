#pragma once

class Printer : public QObject {
    Q_OBJECT

public:
    enum class PrintType {
        HTML,
        PDF,
    };
    Q_ENUM(PrintType)

        static Printer& instance();

    void print(const QList<int> &pagesToTake);
    void printAll();
    void addPage(const QString &page, const QMap<QString, QString> &information);
    void removePage(int index);
    void clear();

    static QString printTypeToQString(PrintType type);
    static PrintType qStringToPrintType(const QString &type);
    void setPrinterSettings(const PrintType type);

signals:
    void pageAdded(const QMap<QString, QString> &information);
    void pageRemoved(int index);
    void printerPagesCleared();

private:
    QStringList _printPages;
    PrintType _printType;
};
