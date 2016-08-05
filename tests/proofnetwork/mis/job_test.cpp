#include "gtest/test_global.h"

#include "proofnetwork/mis/data/job.h"

#include <QJsonDocument>
#include <QJsonObject>
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
        jsonDoc = QJsonDocument::fromJson(dataFromFile(":/data/job.json"));
        ASSERT_FALSE(jsonDoc.isEmpty());
        jsonDoc2 = QJsonDocument::fromJson(dataFromFile(":/data/job2.json"));
        ASSERT_FALSE(jsonDoc2.isEmpty());


        jobUT = Job::fromJson(jsonDoc.object());
        ASSERT_TRUE(jobUT);

        jobUT2 = Job::fromJson(jsonDoc2.object());
        ASSERT_TRUE(jobUT2);

        qmlWrapperUT = jobUT->toQmlWrapper();
    }

    void TearDown() override
    {
        delete qmlWrapperUT;
    }

protected:
    QJsonDocument jsonDoc;
    QJsonDocument jsonDoc2;
    JobSP jobUT;
    JobSP jobUT2;
    JobQmlWrapper *qmlWrapperUT;
};

TEST_F(JobTest, fromJson)
{
    EXPECT_TRUE(jobUT->isFetched());

    EXPECT_EQ("42", jobUT->id());
    EXPECT_EQ("MT-42", jobUT->name());
    EXPECT_EQ("metrix", jobUT->source());
    EXPECT_DOUBLE_EQ(2016.0, jobUT->width());
    EXPECT_DOUBLE_EQ(1350.0, jobUT->height());
    EXPECT_EQ(ApiHelper::WorkflowStatus::IsReadyForStatus, jobUT->workflowStatus(ApiHelper::WorkflowAction::CuttingAction));
    EXPECT_EQ(ApiHelper::WorkflowStatus::NeedsStatus, jobUT->workflowStatus(ApiHelper::WorkflowAction::BoxingAction));
}

TEST_F(JobTest, toJson)
{
    JobSP job = Job::fromJson(jobUT->toJson());
    EXPECT_EQ("42", jobUT->id());
    EXPECT_EQ("MT-42", jobUT->name());
    EXPECT_EQ("metrix", jobUT->source());
    EXPECT_DOUBLE_EQ(2016.0, jobUT->width());
    EXPECT_DOUBLE_EQ(1350.0, jobUT->height());
    EXPECT_EQ(ApiHelper::WorkflowStatus::IsReadyForStatus, jobUT->workflowStatus(ApiHelper::WorkflowAction::CuttingAction));
    EXPECT_EQ(ApiHelper::WorkflowStatus::NeedsStatus, jobUT->workflowStatus(ApiHelper::WorkflowAction::BoxingAction));
}

TEST_F(JobTest, customJob)
{
    JobSP job = Job::create("123");
    job->setName("I-123");
    job->setSource("ProFIT");
    job->setQuantity(100);
    job->setWidth(1024.0);
    job->setHeight(512.0);
    job->setWorkflowStatus(ApiHelper::WorkflowAction::CuttingAction,
                             ApiHelper::WorkflowStatus::IsReadyForStatus);
    EXPECT_EQ("123", job->id());
    EXPECT_EQ("I-123", job->name());
    EXPECT_EQ("ProFIT", job->source());
    EXPECT_DOUBLE_EQ(1024.0, job->width());
    EXPECT_DOUBLE_EQ(512.0, job->height());
    EXPECT_EQ(ApiHelper::WorkflowStatus::IsReadyForStatus, job->workflowStatus(ApiHelper::WorkflowAction::CuttingAction));
}

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
    EXPECT_EQ(jobUT2->width(), jobUT->width());
    EXPECT_EQ(jobUT2->height(), jobUT->height());
    EXPECT_EQ(jobUT2->source(), jobUT->source());
    EXPECT_EQ(jobUT2->workflowStatus(ApiHelper::WorkflowAction::CuttingAction),
              jobUT->workflowStatus(ApiHelper::WorkflowAction::CuttingAction));
    EXPECT_EQ(jobUT2->workflowStatus(ApiHelper::WorkflowAction::BoxingAction),
              jobUT->workflowStatus(ApiHelper::WorkflowAction::BoxingAction));
}

TEST_F(JobTest, setWorkflowStatus)
{
    ApiHelper::WorkflowStatus workflowStatus = jobUT->workflowStatus(ApiHelper::WorkflowAction::CuttingAction);
    ApiHelper::WorkflowStatus workflowStatusUpdate = ApiHelper::WorkflowStatus::InProgressStatus;
    EXPECT_TRUE(workflowStatus != workflowStatusUpdate);
    jobUT->setWorkflowStatus(ApiHelper::WorkflowAction::CuttingAction, workflowStatusUpdate);
    EXPECT_EQ(jobUT->workflowStatus(ApiHelper::WorkflowAction::CuttingAction), workflowStatusUpdate);
}

