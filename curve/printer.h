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

    static void print(const QStringList &pagesToTake);
    static void printAll();
    static void addPage(const QString &page, const QMap<QString, QString> &information);
    static void removePage(int index);
    static void clear();

    static QString printTypeToQString(PrintType type);
    static PrintType qStringToPrintType(const QString &type);
    static void setPrinterSettings(const PrintType type);

signals:
    void pageAdded(const QMap<QString, QString> &information);
    void pageRemoved(int index);
    void printerPagesCleared();

private:
    static QStringList _printPages;
    static PrintType _printType;
};
