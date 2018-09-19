[![Build Status](https://travis-ci.com/opensoft/proofutils.svg?branch=develop)](https://travis-ci.com/opensoft/proofutils)

Proof Utils
=============
Set of proof modules used by almost all stations services: utils, network/lprprinter, network/mis, network/ums

## ProofUtils
Various non-gui helpers.

#### EplLabelGenerator
Generates EPL-compliant label that can be used in thermal printers such as Zebra.

#### QrCodeGenerator
Generates `QImage` with QR code or EPL-compliant binary data.

#### Hardware::LprPrinter
Helper class for working with lpr/lpq utilities to print using LPR subsystem.

#### LabelPrinter
Helper class that combines Hardware::LprPrinter and ProofNetworkLprPrinter for printing labels.

## ProofNetworkLprPrinter
API class for lpr-printer proofservice.

## ProofNetworkMis
Base classes for MIS-related modules.

## ProofNetworkUms
API and DTO classes to work with UMS (internal Opensoft auth system).
