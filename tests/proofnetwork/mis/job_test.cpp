#include "gtest/test_global.h"

#include "proofnetwork/mis/data/job.h"

#include <QSignalSpy>

using namespace Proof::Mis;
using testing::Test;

class JobTest: public Test
{
public:
    JobTest()
    {
    }

protected:
    void SetUp() override
    {
        jobUT = Job::create("123");
        jobUT->setName("I-123");
        jobUT->setQuantity(100);
        jobUT->setWorkflowStatus(ApiHelper::WorkflowAction::CuttingAction,
                                 ApiHelper::WorkflowStatus::IsReadyForStatus);
        ASSERT_TRUE(jobUT);

        jobUT2 = Job::create("ABC");
        jobUT->setName("T-ABC");
        jobUT->setQuantity(1000);
        jobUT->setWorkflowStatus(ApiHelper::WorkflowAction::CuttingAction,
                                 ApiHelper::WorkflowStatus::InProgressStatus);
        ASSERT_TRUE(jobUT2);

        qmlWrapperUT = jobUT->toQmlWrapper();
    }

    void TearDown() override
    {
        delete qmlWrapperUT;
    }

protected:
    JobSP jobUT;
    JobSP jobUT2;
    JobQmlWrapper *qmlWrapperUT;
};

TEST_F(JobTest, updateFrom)
{
    QList<QSignalSpy *> spies = spiesForObject(jobUT.data());
    QList<QSignalSpy *> qmlspies = spiesForObject(qmlWrapperUT);

    jobUT->updateFrom(jobUT2);

    for (QSignalSpy *spy: qmlspies)
        EXPECT_EQ(1, spy->count()) << spy->signal().constData();

    for (QSignalSpy *spy: spies)
        EXPECT_EQ(1, spy->count()) << spy->signal().constData();

    qDeleteAll(spies);
    spies.clear();
    qDeleteAll(qmlspies);
    qmlspies.clear();

    EXPECT_EQ(jobUT2->id(), jobUT->id());
    EXPECT_EQ(jobUT2->name(), jobUT->name());
    EXPECT_EQ(jobUT2->quantity(), jobUT->quantity());
    EXPECT_EQ(jobUT2->workflowStatus(ApiHelper::WorkflowAction::CuttingAction),
              jobUT->workflowStatus(ApiHelper::WorkflowAction::CuttingAction));
}

