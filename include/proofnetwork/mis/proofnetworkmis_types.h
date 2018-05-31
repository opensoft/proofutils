#ifndef PROOFNETWORKMIS_TYPES_H
#define PROOFNETWORKMIS_TYPES_H

#include "proofnetwork/proofnetwork_types.h"

#include <QSharedPointer>
#include <QWeakPointer>

namespace Proof {
namespace Mis {

class Job;
using JobSP = QSharedPointer<Job>;
using JobWP = QWeakPointer<Job>;

} // namespace Mis
} // namespace Proof

#endif // PROOFNETWORKMIS_TYPES_H
