#ifndef PROOFUTILS_BASIC_PACKAGE_H
#define PROOFUTILS_BASIC_PACKAGE_H

#include "proofcore/proofobject.h"
#include "proofcore/proofalgorithms.h"
#include "proofcore/future.h"
#include "proofcore/tasks.h"
#include "proofcore/coreapplication.h"
#include "proofcore/settings.h"
#include "proofcore/settingsgroup.h"
#include "proofnetwork/apicall.h"

using Proof::ProofObject;

namespace algorithms = Proof::algorithms;

using Proof::Future;
using Proof::FutureSP;
using Proof::FutureWP;
using Proof::Promise;
using Proof::PromiseSP;
using Proof::PromiseWP;
using Proof::Failure;
using Proof::WithFailure;
using Proof::CancelableFuture;

namespace tasks = Proof::tasks;

using Proof::Settings;
using Proof::SettingsGroup;

using Proof::ApiCall;

#endif // PROOFUTILS_BASIC_PACKAGE_H
