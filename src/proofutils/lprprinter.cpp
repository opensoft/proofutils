#include "lprprinter.h"

#include "proofseed/tasks.h"

#include "proofcore/proofobject_p.h"

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QThread>

static const QString EMPTY_PRINTER_TEXT = QStringLiteral("Printing aborted.\n Empty printer.");
static const QString RESTRICTOR = QStringLiteral("__Proof_Lpr");

namespace Proof {
namespace Hardware {
class LprPrinterPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(LprPrinter)

    FutureSP<bool> printRawData(const QByteArray &data, bool ignorePrinterState) const;
    FutureSP<bool> printFile(const QString &fileName, unsigned int quantity, bool ignorePrinterState) const;

    FutureSP<bool> printerIsReady() const;
    FutureSP<bool> checkLpOptions() const;

    QString printerName;
    QString printerHost;
    bool strictPrinterCheck = false;
};
} // namespace Hardware
} // namespace Proof

#ifdef Q_OS_WIN
static QString system32Path()
{
    static QString result;
    if (result.isEmpty()) {
        if (QFile::exists("c:\\Windows\\sysnative\\lpr.exe"))
            result = "c:\\Windows\\sysnative";
        else
            result = "c:\\Windows\\system32";
    }
    return result;
}
#endif

using namespace Proof;
using namespace Proof::Hardware;

LprPrinter::LprPrinter(const QString &printerHost, const QString &printerName, bool strictPrinterCheck, QObject *parent)
    : ProofObject(*new LprPrinterPrivate, parent)
{
    Q_D(LprPrinter);
    d->printerName = printerName.trimmed();
    d->printerHost = printerHost.trimmed();
    d->strictPrinterCheck = strictPrinterCheck;
    qCDebug(proofUtilsLprPrinterInfoLog) << "Label printer name:" << printerName << "at host" << printerHost;
    if (printerHost.isEmpty() && printerName.isEmpty())
        qCWarning(proofUtilsLprPrinterInfoLog) << QStringLiteral("Empty printer!");
}

FutureSP<bool> LprPrinter::printRawData(const QByteArray &data, bool ignorePrinterState) const
{
    Q_D_CONST(LprPrinter);
    return d->printRawData(data, ignorePrinterState);
}

FutureSP<bool> LprPrinter::printFile(const QString &fileName, unsigned int quantity, bool ignorePrinterState) const
{
    Q_D_CONST(LprPrinter);
    return d->printFile(fileName, quantity, ignorePrinterState);
}

FutureSP<bool> LprPrinter::printerIsReady() const
{
    Q_D_CONST(LprPrinter);
    return d->printerIsReady();
}

FutureSP<bool> LprPrinterPrivate::printRawData(const QByteArray &data, bool ignorePrinterState) const
{
    FutureSP<bool> status = ignorePrinterState ? Future<>::successful(true) : printerIsReady();
    return status->andThen([this, data] {
        return tasks::run(tasks::RestrictionType::ThreadBound, RESTRICTOR, [this, data]() -> bool {
            QScopedPointer<QProcess> printProcess(new QProcess);

            QStringList args;
            if (!printerHost.isEmpty()) {
#ifdef Q_OS_WIN
                args << "-S" << printerHost;
#else
                args << QStringLiteral("-H") << printerHost;
#endif
            }
            if (!printerName.isEmpty())
                args << QStringLiteral("-P") << printerName;

#ifdef Q_OS_WIN
            QFile printFile;
            printFile.setFileName(QStringLiteral("%1/proof_last_label_to_print").arg(QDir::tempPath()));
            if (!printFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
                qCWarning(proofUtilsLprPrinterInfoLog) << "Can't open temporary file";
                return WithFailure(QStringLiteral("Printing aborted.\nCan't open temporary file."), UTILS_MODULE_CODE,
                                   UtilsErrorCode::TemporaryFileError);
            }
            printFile.write(data);
            printFile.close();
            args << QStringLiteral("-o") << QStringLiteral("l") << printFile.fileName().replace("/", "\\");
            printProcess->start(system32Path() + "\\lpr.exe", args);
#else
                printProcess->start(QStringLiteral("lpr"), args);
#endif
            qCDebug(proofUtilsLprPrinterDataLog) << "Lpr started as" << printProcess->program() << args;

            printProcess->waitForStarted();
            if (printProcess->error() == QProcess::UnknownError) {
#ifndef Q_OS_WIN
                printProcess->write(data);
                while (printProcess->bytesToWrite())
                    printProcess->waitForBytesWritten();
                printProcess->closeWriteChannel();
#endif
                printProcess->waitForFinished();
            } else {
                printProcess->waitForFinished();
                qCWarning(proofUtilsLprPrinterInfoLog) << "lpr can't be started";
                return WithFailure(QStringLiteral("Printing aborted.\nCan't start lpr."), UTILS_MODULE_CODE,
                                   UtilsErrorCode::LprCannotBeStarted);
            }

            if (printProcess->exitCode()) {
                qCWarning(proofUtilsLprPrinterInfoLog)
                    << "lpr finished with non-zero code, probably nothing was printed" << printProcess->exitCode()
                    << printProcess->readAllStandardError().replace("\n", " ")
                    << printProcess->readAllStandardOutput().replace("\n", " ");
                return WithFailure(QStringLiteral("Printing probably not finished.\nProcess exited with code %1.")
                                       .arg(printProcess->exitCode()),
                                   UTILS_MODULE_CODE, UtilsErrorCode::LprProcessNonZeroExitCode);
            }
            qCDebug(proofUtilsLprPrinterInfoLog) << "Raw data printed";
            return true;
        });
    });
}

FutureSP<bool> LprPrinterPrivate::printFile(const QString &fileName, unsigned int quantity, bool ignorePrinterState) const
{
    FutureSP<bool> status = ignorePrinterState ? Future<>::successful(true) : printerIsReady();
    return status->andThen([this, fileName, quantity] {
        return tasks::run(tasks::RestrictionType::ThreadBound, RESTRICTOR, [this, fileName, quantity]() -> bool {
#ifdef Q_OS_WIN
            unsigned int qty = quantity;
            while (qty--) {
#endif
                QScopedPointer<QProcess> printProcess(new QProcess);
                QStringList args;
                if (!printerHost.isEmpty()) {
#ifdef Q_OS_WIN
                    args << "-S" << printerHost;
#else
                    args << QStringLiteral("-H") << printerHost;
#endif
                }
                if (!printerName.isEmpty())
                    args << QStringLiteral("-P") << printerName;

#ifdef Q_OS_WIN
                args << QStringLiteral("-o") << QStringLiteral("l") << QString(fileName).replace("/", "\\");
                printProcess->start(system32Path() + "\\lpr.exe", args);
#else
                args << QStringLiteral("-#") << QString::number(quantity) << fileName;
                printProcess->start(QStringLiteral("lpr"), args);
#endif

                qCDebug(proofUtilsLprPrinterDataLog) << "Lpr started as" << printProcess->program() << args;
                printProcess->waitForStarted();
                QProcess::ProcessError startError = printProcess->error();
                printProcess->waitForFinished();
                if (startError != QProcess::UnknownError) {
                    qCWarning(proofUtilsLprPrinterInfoLog) << "lpr can't be started";
                    return WithFailure(QStringLiteral("Printing aborted.\nCan't start lpr."), UTILS_MODULE_CODE,
                                       UtilsErrorCode::LprCannotBeStarted);
                }

                if (printProcess->exitCode()) {
                    qCWarning(proofUtilsLprPrinterInfoLog)
                        << "lpr finished with non-zero code, probably nothing was printed" << printProcess->exitCode()
                        << printProcess->readAllStandardError().replace("\n", " ")
                        << printProcess->readAllStandardOutput().replace("\n", " ");
                    return WithFailure(QStringLiteral("Printing probably not finished.\nProcess exited with code %1.")
                                           .arg(printProcess->exitCode()),
                                       UTILS_MODULE_CODE, UtilsErrorCode::LprProcessNonZeroExitCode);
                }
                qCDebug(proofUtilsLprPrinterInfoLog) << "File printed";
#ifdef Q_OS_WIN
            }
#endif
            return true;
        });
    });
}

FutureSP<bool> LprPrinterPrivate::printerIsReady() const
{
    if (printerHost.isEmpty() && printerName.isEmpty()) {
        return Future<bool>::fail(Failure(EMPTY_PRINTER_TEXT, UTILS_MODULE_CODE, UtilsErrorCode::LpqCannotBeStarted));
    }

    return tasks::run(tasks::RestrictionType::ThreadBound, RESTRICTOR, [this]() -> FutureSP<bool> {
        QScopedPointer<QProcess> queueProcess(new QProcess);
        QStringList args;
        if (!printerHost.isEmpty()) {
#ifdef Q_OS_WIN
            args << "-S" << printerHost;
#else
            args << QStringLiteral("-h") << printerHost;
#endif
        }
        if (!printerName.isEmpty())
            args << QStringLiteral("-P") << printerName;
#ifdef Q_OS_WIN
        queueProcess->start(system32Path() + "\\lpq.exe", args);
#else
        queueProcess->start(QStringLiteral("lpq"), args);
#endif
        queueProcess->waitForStarted();
        if (queueProcess->error() == QProcess::UnknownError) {
            queueProcess->waitForReadyRead();
            queueProcess->waitForFinished();
            QString queueInfo = queueProcess->readAll().trimmed().toLower();
            qCDebug(proofUtilsLprPrinterDataLog) << "Queue info for" << printerHost << printerName << ":" << queueInfo;
            if (queueInfo.isEmpty()) {
                QString errorOutput = queueProcess->readAllStandardError();
                qCWarning(proofUtilsLprPrinterInfoLog) << "Queue info for" << printerHost << printerName
                                                       << "is empty. Probably printer doesn't exist." << errorOutput;
                return WithFailure(QStringLiteral(
                                       "Can't query printer %1@%2 info.\nProbably this printer doesn't exist\n%3")
                                       .arg(printerName.isEmpty() ? QStringLiteral("default") : printerName,
                                            printerHost.isEmpty() ? QStringLiteral("localhost") : printerHost,
                                            errorOutput),
                                   UTILS_MODULE_CODE, UtilsErrorCode::PrinterInfoCannotBeQueried);
            }

            if (queueInfo.contains(QStringLiteral("%1 is not ready").arg(printerName.toLower()))) {
                qCWarning(proofUtilsLprPrinterInfoLog) << printerHost << printerName << "is not ready";
                return WithFailure(QString(QObject::tr("Printer \n%1@%2 is not ready."))
                                       .arg(printerName.isEmpty() ? QStringLiteral("default") : printerName,
                                            printerHost.isEmpty() ? QStringLiteral("localhost") : printerHost),
                                   UTILS_MODULE_CODE, UtilsErrorCode::PrinterNotReady, Failure::UserFriendlyHint);
            }

            if (queueInfo.startsWith(QLatin1String("windows lpd"))) {
                if (queueInfo.contains(QLatin1String("error:"))
                    || (!printerName.isEmpty() && !queueInfo.contains(printerName.toLower()))) {
                    qCWarning(proofUtilsLprPrinterInfoLog)
                        << "Something is wrong with" << printerHost << printerName << ". Info:"
                        << queueInfo.replace(QLatin1String("\n"), QLatin1String(" "))
                               .replace(QLatin1String("\r"), QString());
                    return WithFailure(QString(QObject::tr("Printer \n%1@%2 is not ready."))
                                           .arg(printerName.isEmpty() ? QStringLiteral("default") : printerName,
                                                printerHost.isEmpty() ? QStringLiteral("localhost") : printerHost),
                                       UTILS_MODULE_CODE, UtilsErrorCode::PrinterNotReady, Failure::UserFriendlyHint);
                }
                qCWarning(proofUtilsLprPrinterInfoLog)
                    << printerHost << printerName << "is hosted at Windows and probably is ready. Info:"
                    << queueInfo.replace(QLatin1String("\n"), QLatin1String(" ")).replace(QLatin1String("\r"), QString());
            } else if (!queueInfo.contains(QStringLiteral("%1 is ready").arg(printerName.toLower()))) {
                qCWarning(proofUtilsLprPrinterInfoLog)
                    << "Queue info for" << printerHost << printerName
                    << "contains unrecognized info:" << queueInfo.replace(QLatin1String("\n"), QLatin1String(" "));
                return WithFailure(QStringLiteral("Printer error.\nQueue info for %1@%2:\n%3")
                                       .arg(printerName.isEmpty() ? QStringLiteral("default") : printerName,
                                            printerHost.isEmpty() ? QStringLiteral("localhost") : printerHost, queueInfo),
                                   UTILS_MODULE_CODE, UtilsErrorCode::PrinterInfoError);
            }
        } else {
            queueProcess->waitForFinished();
            qCWarning(proofUtilsLprPrinterInfoLog) << "lpq can't be started";
            if (strictPrinterCheck) {
                return WithFailure(QStringLiteral("Printing aborted.\nCan't start lpq."), UTILS_MODULE_CODE,
                                   UtilsErrorCode::LpqCannotBeStarted);
            }
        }

        return checkLpOptions();
    });
}

FutureSP<bool> LprPrinterPrivate::checkLpOptions() const
{
    return tasks::run(tasks::RestrictionType::ThreadBound, RESTRICTOR, [this]() -> bool {
        QScopedPointer<QProcess> optionsProcess(new QProcess);
        QStringList args;
        if (!printerHost.isEmpty())
            args << QStringLiteral("-h") << printerHost;
        if (!printerName.isEmpty())
            args << QStringLiteral("-p") << printerName;
#ifdef Q_OS_WIN
        optionsProcess->start(system32Path() + "lpoptions.exe", args);
#else
        optionsProcess->start(QStringLiteral("lpoptions"), args);
#endif
        optionsProcess->waitForStarted();
        if (optionsProcess->error() == QProcess::UnknownError) {
            optionsProcess->waitForReadyRead();
            optionsProcess->waitForFinished();
            QString options = optionsProcess->readAll().trimmed();
            qCDebug(proofUtilsLprPrinterDataLog) << "LP Options for" << printerHost << printerName << ":" << options;
            if (options.isEmpty()) {
                QString errorOutput = optionsProcess->readAllStandardError();
                qCWarning(proofUtilsLprPrinterInfoLog) << "options for" << printerHost << printerName
                                                       << "are empty. Probably printer doesn't exist." << errorOutput;
                return WithFailure(QStringLiteral("Printing aborted.\nCan't query lpoptions for %1@%2.\nProbably this "
                                                  "printer doesn't exist\n%3")
                                       .arg(printerName.isEmpty() ? QStringLiteral("default") : printerName,
                                            printerHost.isEmpty() ? QStringLiteral("localhost") : printerHost,
                                            errorOutput),
                                   UTILS_MODULE_CODE, UtilsErrorCode::PrinterOptionsCannotBeQueried);
            }
            QRegExp stateRe = QRegExp("printer-state=([^\\s]*)");
            QRegExp stateReasonsRe = QRegExp("printer-state-reasons=([^\\s]*)");
            QString state;
            QString stateReasons;
            if (stateRe.indexIn(options) != -1)
                state = stateRe.cap(1);
            if (stateReasonsRe.indexIn(options) != -1)
                stateReasons = stateReasonsRe.cap(1);
            if (stateReasons.toLower() == QLatin1String("none"))
                stateReasons = QString();
            if (state == QLatin1String("5") || (state == QLatin1String("3") && !stateReasons.isEmpty())) {
                qCWarning(proofUtilsLprPrinterInfoLog) << "lpoptions for" << printerHost << printerName
                                                       << "returned bad state of printer" << state << stateReasons;
                return WithFailure(QStringLiteral("Printing aborted.\nCheck %1@%2 printer.\nProbably it is offline or "
                                                  "is in wrong state.\n%3: %4")
                                       .arg(printerName.isEmpty() ? QStringLiteral("default") : printerName,
                                            printerHost.isEmpty() ? QStringLiteral("localhost") : printerHost, state,
                                            stateReasons),
                                   UTILS_MODULE_CODE, UtilsErrorCode::PrinterOffline);
            }
        } else {
            optionsProcess->waitForFinished();
            qCWarning(proofUtilsLprPrinterInfoLog) << "lpoptions can't be started";
            if (strictPrinterCheck) {
                return WithFailure(QStringLiteral("Printing aborted.\nCan't start lpoptions."), UTILS_MODULE_CODE,
                                   UtilsErrorCode::LpoptionsCannotBeStarted);
            }
        }
        return true;
    });
}
