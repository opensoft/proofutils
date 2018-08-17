ProofUtils Changelog
====================

## Not Released
#### Features
 * Utils: IpAddressValidator is now in Gui module
 * Utils: PowerManager is now in Gui module
 * Utils: NetworkConfigurationManager is now in Gui module
 * Utils: MobileUtils is now merged into AndroidManager from Gui module
 * Hardware: LprPrinter module moved to Utils as LprPrinter class

#### Bug Fixing
 * --

## 0.18.8.10
#### Features
 * Q_D_CONST/Q_Q_CONST macro for any usages of const d-ptr/q-ptr
 * Network: Signals from API classes removed

#### Bug Fixing
 * --

## 0.18.7.6
#### Features
 * Network: All apis moved to BaseRestApi
 * UMS: email field is now used as username
 * Mis: Job::pageCount property added
 * Almost all QList usages replaced with QVector

#### Bug Fixing
 * --

## 0.18.3.14
#### Features
 * UMS: Renamed Role::locale to Role::location
 * UMS: Removed Role::fromJson
 * UMS: Add Role::fromString
 * Utils: basic_package.h header for less includes/usages in apps code
 * Utils: LabelPrinter public interface changed to Futures
 * Copies of errorOccurred signal in ProofObject children removed

#### Bug Fixing
 * --

## Changelog for releases older than 2018 can be found in main proof repository
