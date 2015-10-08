#ifndef PROOFNETWORKMIS_TYPES_H
#define PROOFNETWORKMIS_TYPES_H

#include "proofnetwork/proofnetwork_types.h"

#include <QSharedPointer>
#include <QWeakPointer>

namespace Proof {
namespace Mis {

class Job;
typedef QSharedPointer<Job> JobSP;
typedef QWeakPointer<Job> JobWP;

}
}

#endif // PROOFNETWORKMIS_TYPES_H
