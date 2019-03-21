// clazy:skip

#include "proofnetwork/mis/data/job.h"

#include "gtest/proof/test_global.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>

using namespace Proof::Mis;
using testing::Test;
using testing::TestWithParam;

class JobTest : public Test
{
public:
    JobTest() {}

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

    void TearDown() override { delete qmlWrapperUT; }

protected:
    QJsonDocument jsonDoc;
    QJsonDocument jsonDoc2;
    JobSP jobUT;
    JobSP jobUT2;
    JobQmlWrapper *qmlWrapperUT;
};

namespace Proof::Mis {
void PrintTo(const WorkflowElement &x, ::std::ostream *os)
{
    *os << x.toString().toUtf8().constData();
}
void PrintTo(const WorkflowStatus &x, ::std::ostream *os)
{
    *os << workflowStatusToString(x).toUtf8().constData();
}
void PrintTo(const WorkflowAction &x, ::std::ostream *os)
{
    *os << workflowActionToString(x).toUtf8().constData();
}
void PrintTo(const PaperSide &x, ::std::ostream *os)
{
    *os << paperSideToString(x).toUtf8().constData();
}
void PrintTo(const TransitionEvent &x, ::std::ostream *os)
{
    *os << transitionEventToString(x).toUtf8().constData();
}
} // namespace Proof::Mis

using JobWorkflowTuple = std::tuple<WorkflowStatus, QVector<WorkflowElement>, WorkflowAction, PaperSide>;
using StatusAfterTransitionTuple = std::tuple<WorkflowStatus, TransitionEvent>;

class JobWorkflowTest : public TestWithParam<JobWorkflowTuple>
{};

class WorkflowStatusAfterTransitionTest : public TestWithParam<StatusAfterTransitionTuple>
{};

TEST_F(JobTest, qmlWrapperProperties)
{
    QStringList invalidProperties = findWrongChangedSignalsInQmlWrapper(qmlWrapperUT);
    EXPECT_EQ(0, invalidProperties.count()) << invalidProperties.join("\n").toLatin1().constData();
}

TEST_F(JobTest, fromJson)
{
    EXPECT_TRUE(jobUT->isFetched());

    EXPECT_EQ("42", jobUT->id());
    EXPECT_EQ("42", qmlWrapperUT->id());
    EXPECT_EQ("MT-42", jobUT->name());
    EXPECT_EQ("MT-42", qmlWrapperUT->name());
    EXPECT_EQ("metrix", jobUT->source());
    EXPECT_EQ("metrix", qmlWrapperUT->source());
    EXPECT_DOUBLE_EQ(2016.0, jobUT->width());
    EXPECT_DOUBLE_EQ(2016.0, qmlWrapperUT->width());
    EXPECT_DOUBLE_EQ(1350.0, jobUT->height());
    EXPECT_DOUBLE_EQ(1350.0, qmlWrapperUT->height());
    EXPECT_EQ(50, jobUT->quantity());
    EXPECT_EQ(50, qmlWrapperUT->quantity());
    EXPECT_EQ(WorkflowStatus::IsReadyForStatus, jobUT->workflowStatus(WorkflowAction::CuttingAction));
    EXPECT_EQ(WorkflowStatus::IsReadyForStatus, qmlWrapperUT->workflowStatus(WorkflowAction::CuttingAction));
    EXPECT_EQ(WorkflowStatus::NeedsStatus, jobUT->workflowStatus(WorkflowAction::BoxingAction));
    EXPECT_EQ(WorkflowStatus::NeedsStatus, qmlWrapperUT->workflowStatus(WorkflowAction::BoxingAction));
    EXPECT_EQ(EntityStatus::ValidEntity, jobUT->status());
    EXPECT_EQ(EntityStatus::ValidEntity, qmlWrapperUT->status());
    EXPECT_TRUE(jobUT->hasPreview());
    EXPECT_TRUE(qmlWrapperUT->hasPreview());
    EXPECT_TRUE(jobUT->hasBackSide());
    EXPECT_TRUE(qmlWrapperUT->hasBackSide());
    EXPECT_EQ(10, jobUT->pageCount());
    EXPECT_EQ(10, qmlWrapperUT->pageCount());
}

TEST_F(JobTest, toJson)
{
    JobSP job = Job::fromJson(jobUT->toJson());
    EXPECT_EQ("42", job->id());
    EXPECT_EQ("MT-42", job->name());
    EXPECT_EQ("metrix", job->source());
    EXPECT_DOUBLE_EQ(2016.0, job->width());
    EXPECT_DOUBLE_EQ(1350.0, job->height());
    EXPECT_EQ(50, job->quantity());
    EXPECT_EQ(WorkflowStatus::IsReadyForStatus, job->workflowStatus(WorkflowAction::CuttingAction));
    EXPECT_EQ(WorkflowStatus::NeedsStatus, job->workflowStatus(WorkflowAction::BoxingAction));
    EXPECT_EQ(EntityStatus::ValidEntity, job->status());
    EXPECT_TRUE(job->hasPreview());
    EXPECT_TRUE(job->hasBackSide());
    EXPECT_EQ(10, job->pageCount());
}

TEST_F(JobTest, customJob)
{
    JobSP job = Job::create("123");
    job->setName("I-123");
    job->setSource("ProFIT");
    job->setQuantity(100);
    job->setWidth(1024.0);
    job->setHeight(512.0);
    job->setHasPreview(true);
    job->setHasBackSide(true);
    job->setWorkflowStatus(WorkflowAction::CuttingAction, WorkflowStatus::IsReadyForStatus);
    job->setStatus(EntityStatus::NotReadyEntity);
    job->setPageCount(5);
    EXPECT_EQ("123", job->id());
    EXPECT_EQ("I-123", job->name());
    EXPECT_EQ("ProFIT", job->source());
    EXPECT_DOUBLE_EQ(1024.0, job->width());
    EXPECT_DOUBLE_EQ(512.0, job->height());
    EXPECT_EQ(100, job->quantity());
    EXPECT_EQ(WorkflowStatus::IsReadyForStatus, job->workflowStatus(WorkflowAction::CuttingAction));
    EXPECT_EQ(EntityStatus::NotReadyEntity, job->status());
    EXPECT_TRUE(job->hasPreview());
    EXPECT_TRUE(job->hasBackSide());
    EXPECT_EQ(5, job->pageCount());
}

TEST_F(JobTest, updateFrom)
{
    QVector<QSignalSpy *> spies = spiesForObject(jobUT.data());
    QVector<QSignalSpy *> qmlspies = spiesForObject(qmlWrapperUT);

    jobUT->updateFrom(jobUT2);

    for (QSignalSpy *spy : qmlspies)
        EXPECT_EQ(1, spy->count()) << spy->signal().constData();

    for (QSignalSpy *spy : spies)
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
    EXPECT_EQ(jobUT2->status(), jobUT->status());
    EXPECT_EQ(jobUT2->hasPreview(), jobUT->hasPreview());
    EXPECT_EQ(jobUT2->hasBackSide(), jobUT->hasBackSide());
    EXPECT_EQ(jobUT2->pageCount(), jobUT->pageCount());
    EXPECT_EQ(jobUT2->workflowStatus(WorkflowAction::CuttingAction),
              jobUT->workflowStatus(WorkflowAction::CuttingAction));
    EXPECT_EQ(jobUT2->workflowStatus(WorkflowAction::BoxingAction), jobUT->workflowStatus(WorkflowAction::BoxingAction));
}

TEST_F(JobTest, setWorkflowStatus)
{
    WorkflowStatus workflowStatus = jobUT->workflowStatus(WorkflowAction::CuttingAction);
    WorkflowStatus workflowStatusUpdate = WorkflowStatus::InProgressStatus;
    EXPECT_TRUE(workflowStatus != workflowStatusUpdate);
    jobUT->setWorkflowStatus(WorkflowAction::CuttingAction, workflowStatusUpdate);
    EXPECT_EQ(jobUT->workflowStatus(WorkflowAction::CuttingAction), workflowStatusUpdate);
}

TEST_F(JobTest, workflowElementFromStringSanity)
{
    {
        WorkflowElement e("in progress:printing");
        EXPECT_EQ("in progress:printing", e.toString());
        EXPECT_EQ(WorkflowAction::PrintingAction, e.action());
        EXPECT_EQ(WorkflowStatus::InProgressStatus, e.status());
        EXPECT_EQ(PaperSide::NotSetSide, e.paperSide());
    }
    {
        WorkflowElement e("in progress:printing:front");
        EXPECT_EQ("in progress:printing:front", e.toString());
        EXPECT_EQ(WorkflowAction::PrintingAction, e.action());
        EXPECT_EQ(WorkflowStatus::InProgressStatus, e.status());
        EXPECT_EQ(PaperSide::FrontSide, e.paperSide());
    }
    {
        WorkflowElement e("in progress:printing:back");
        EXPECT_EQ("in progress:printing:back", e.toString());
        EXPECT_EQ(WorkflowAction::PrintingAction, e.action());
        EXPECT_EQ(WorkflowStatus::InProgressStatus, e.status());
        EXPECT_EQ(PaperSide::BackSide, e.paperSide());
    }
    {
        WorkflowElement e("in progress:printing:metaside");
        EXPECT_EQ("in progress:printing", e.toString());
        EXPECT_EQ(WorkflowAction::PrintingAction, e.action());
        EXPECT_EQ(WorkflowStatus::InProgressStatus, e.status());
        EXPECT_EQ(PaperSide::NotSetSide, e.paperSide());
    }
}

TEST_F(JobTest, workflowElementSanity)
{
    {
        WorkflowElement e(WorkflowAction::PrintingAction, WorkflowStatus::InProgressStatus);
        EXPECT_EQ("in progress:printing", e.toString());
        EXPECT_EQ(WorkflowAction::PrintingAction, e.action());
        EXPECT_EQ(WorkflowStatus::InProgressStatus, e.status());
        EXPECT_EQ(PaperSide::NotSetSide, e.paperSide());
    }
    {
        WorkflowElement e(WorkflowAction::PrintingAction, WorkflowStatus::InProgressStatus, PaperSide::FrontSide);
        EXPECT_EQ("in progress:printing:front", e.toString());
        EXPECT_EQ(WorkflowAction::PrintingAction, e.action());
        EXPECT_EQ(WorkflowStatus::InProgressStatus, e.status());
        EXPECT_EQ(PaperSide::FrontSide, e.paperSide());
    }
    {
        WorkflowElement e(WorkflowAction::PrintingAction, WorkflowStatus::InProgressStatus, PaperSide::BackSide);
        EXPECT_EQ("in progress:printing:back", e.toString());
        EXPECT_EQ(WorkflowAction::PrintingAction, e.action());
        EXPECT_EQ(WorkflowStatus::InProgressStatus, e.status());
        EXPECT_EQ(PaperSide::BackSide, e.paperSide());
    }
}

TEST_F(JobTest, workflowElementModificationSanity)
{
    WorkflowElement e(WorkflowAction::PrintingAction, WorkflowStatus::InProgressStatus, PaperSide::BackSide);
    e.setAction(WorkflowAction::CuttingAction);
    EXPECT_EQ("in progress:cutting:back", e.toString());
    EXPECT_EQ(WorkflowAction::CuttingAction, e.action());
    EXPECT_EQ(WorkflowStatus::InProgressStatus, e.status());
    EXPECT_EQ(PaperSide::BackSide, e.paperSide());
    e.setStatus(WorkflowStatus::SuspendedStatus);
    EXPECT_EQ("suspended:cutting:back", e.toString());
    EXPECT_EQ(WorkflowAction::CuttingAction, e.action());
    EXPECT_EQ(WorkflowStatus::SuspendedStatus, e.status());
    EXPECT_EQ(PaperSide::BackSide, e.paperSide());
    e.setPaperSide(PaperSide::FrontSide);
    EXPECT_EQ("suspended:cutting:front", e.toString());
    EXPECT_EQ(WorkflowAction::CuttingAction, e.action());
    EXPECT_EQ(WorkflowStatus::SuspendedStatus, e.status());
    EXPECT_EQ(PaperSide::FrontSide, e.paperSide());
    e.setPaperSide(PaperSide::NotSetSide);
    EXPECT_EQ("suspended:cutting", e.toString());
    EXPECT_EQ(WorkflowAction::CuttingAction, e.action());
    EXPECT_EQ(WorkflowStatus::SuspendedStatus, e.status());
    EXPECT_EQ(PaperSide::NotSetSide, e.paperSide());
}

TEST_F(JobTest, workflowElementCopySanity)
{
    {
        WorkflowElement e(WorkflowAction::PrintingAction, WorkflowStatus::InProgressStatus, PaperSide::FrontSide);
        WorkflowElement other(e);
        EXPECT_EQ("in progress:printing:front", other.toString());
        EXPECT_EQ(WorkflowAction::PrintingAction, other.action());
        EXPECT_EQ(WorkflowStatus::InProgressStatus, other.status());
        EXPECT_EQ(PaperSide::FrontSide, other.paperSide());
    }
    {
        WorkflowElement e(WorkflowAction::PrintingAction, WorkflowStatus::InProgressStatus, PaperSide::FrontSide);
        WorkflowElement other;
        other = e;
        EXPECT_EQ("in progress:printing:front", other.toString());
        EXPECT_EQ(WorkflowAction::PrintingAction, other.action());
        EXPECT_EQ(WorkflowStatus::InProgressStatus, other.status());
        EXPECT_EQ(PaperSide::FrontSide, other.paperSide());
    }
    {
        WorkflowElement e(WorkflowAction::PrintingAction, WorkflowStatus::InProgressStatus, PaperSide::FrontSide);
        WorkflowElement other(std::move(e));
        EXPECT_EQ("in progress:printing:front", other.toString());
        EXPECT_EQ(WorkflowAction::PrintingAction, other.action());
        EXPECT_EQ(WorkflowStatus::InProgressStatus, other.status());
        EXPECT_EQ(PaperSide::FrontSide, other.paperSide());
    }
    {
        WorkflowElement e(WorkflowAction::PrintingAction, WorkflowStatus::InProgressStatus, PaperSide::FrontSide);
        WorkflowElement other;
        other = std::move(e);
        EXPECT_EQ("in progress:printing:front", other.toString());
        EXPECT_EQ(WorkflowAction::PrintingAction, other.action());
        EXPECT_EQ(WorkflowStatus::InProgressStatus, other.status());
        EXPECT_EQ(PaperSide::FrontSide, other.paperSide());
    }
    {
        WorkflowElement e(WorkflowAction::PrintingAction, WorkflowStatus::InProgressStatus, PaperSide::FrontSide);
        WorkflowElement other(WorkflowAction::CuttingAction, WorkflowStatus::SuspendedStatus, PaperSide::BackSide);
        e.swap(other);
        EXPECT_EQ("suspended:cutting:back", e.toString());
        EXPECT_EQ(WorkflowAction::CuttingAction, e.action());
        EXPECT_EQ(WorkflowStatus::SuspendedStatus, e.status());
        EXPECT_EQ(PaperSide::BackSide, e.paperSide());
        EXPECT_EQ("in progress:printing:front", other.toString());
        EXPECT_EQ(WorkflowAction::PrintingAction, other.action());
        EXPECT_EQ(WorkflowStatus::InProgressStatus, other.status());
        EXPECT_EQ(PaperSide::FrontSide, other.paperSide());
    }
}

INSTANTIATE_TEST_CASE_P(
    JobWorkflowTestParameters, JobWorkflowTest,
    testing::Values(JobWorkflowTuple(WorkflowStatus::IsReadyForStatus,
                                     {WorkflowElement("is ready for:printing"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::NotSetSide),
                    JobWorkflowTuple(WorkflowStatus::IsReadyForStatus,
                                     {WorkflowElement("is ready for:printing"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::FrontSide),
                    JobWorkflowTuple(WorkflowStatus::UnknownStatus,
                                     {WorkflowElement("is ready for:printing"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::BackSide),

                    JobWorkflowTuple(WorkflowStatus::IsReadyForStatus,
                                     {WorkflowElement("is ready for:printing:front"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::NotSetSide),
                    JobWorkflowTuple(WorkflowStatus::IsReadyForStatus,
                                     {WorkflowElement("is ready for:printing:front"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::FrontSide),
                    JobWorkflowTuple(WorkflowStatus::UnknownStatus,
                                     {WorkflowElement("is ready for:printing:front"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::BackSide),

                    JobWorkflowTuple(WorkflowStatus::IsReadyForStatus,
                                     {WorkflowElement("is ready for:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::NotSetSide),
                    JobWorkflowTuple(WorkflowStatus::UnknownStatus,
                                     {WorkflowElement("is ready for:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::FrontSide),
                    JobWorkflowTuple(WorkflowStatus::IsReadyForStatus,
                                     {WorkflowElement("is ready for:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::BackSide),

                    JobWorkflowTuple(WorkflowStatus::InProgressStatus,
                                     {WorkflowElement("is ready for:printing:front"),
                                      WorkflowElement("in progress:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::NotSetSide),
                    JobWorkflowTuple(WorkflowStatus::IsReadyForStatus,
                                     {WorkflowElement("is ready for:printing:front"),
                                      WorkflowElement("in progress:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::FrontSide),
                    JobWorkflowTuple(WorkflowStatus::InProgressStatus,
                                     {WorkflowElement("is ready for:printing:front"),
                                      WorkflowElement("in progress:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::BackSide),

                    JobWorkflowTuple(WorkflowStatus::InProgressStatus,
                                     {WorkflowElement("in progress:printing:front"),
                                      WorkflowElement("is ready for:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::NotSetSide),
                    JobWorkflowTuple(WorkflowStatus::InProgressStatus,
                                     {WorkflowElement("in progress:printing:front"),
                                      WorkflowElement("is ready for:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::FrontSide),
                    JobWorkflowTuple(WorkflowStatus::IsReadyForStatus,
                                     {WorkflowElement("in progress:printing:front"),
                                      WorkflowElement("is ready for:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::BackSide),

                    JobWorkflowTuple(WorkflowStatus::InProgressStatus,
                                     {WorkflowElement("in progress:printing:front"),
                                      WorkflowElement("in progress:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::NotSetSide),
                    JobWorkflowTuple(WorkflowStatus::InProgressStatus,
                                     {WorkflowElement("in progress:printing:front"),
                                      WorkflowElement("in progress:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::FrontSide),
                    JobWorkflowTuple(WorkflowStatus::InProgressStatus,
                                     {WorkflowElement("in progress:printing:front"),
                                      WorkflowElement("in progress:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::BackSide),

                    JobWorkflowTuple(WorkflowStatus::SuspendedStatus,
                                     {WorkflowElement("in progress:printing:front"),
                                      WorkflowElement("suspended:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::NotSetSide),
                    JobWorkflowTuple(WorkflowStatus::InProgressStatus,
                                     {WorkflowElement("in progress:printing:front"),
                                      WorkflowElement("suspended:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::FrontSide),
                    JobWorkflowTuple(WorkflowStatus::SuspendedStatus,
                                     {WorkflowElement("in progress:printing:front"),
                                      WorkflowElement("suspended:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::BackSide),

                    JobWorkflowTuple(WorkflowStatus::SuspendedStatus,
                                     {WorkflowElement("suspended:printing:front"),
                                      WorkflowElement("in progress:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::NotSetSide),
                    JobWorkflowTuple(WorkflowStatus::SuspendedStatus,
                                     {WorkflowElement("suspended:printing:front"),
                                      WorkflowElement("in progress:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::FrontSide),
                    JobWorkflowTuple(WorkflowStatus::InProgressStatus,
                                     {WorkflowElement("suspended:printing:front"),
                                      WorkflowElement("in progress:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::BackSide),

                    JobWorkflowTuple(WorkflowStatus::IsReadyForStatus,
                                     {WorkflowElement("done:printing:front"),
                                      WorkflowElement("is ready for:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::NotSetSide),
                    JobWorkflowTuple(WorkflowStatus::DoneStatus,
                                     {WorkflowElement("done:printing:front"),
                                      WorkflowElement("is ready for:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::FrontSide),
                    JobWorkflowTuple(WorkflowStatus::IsReadyForStatus,
                                     {WorkflowElement("done:printing:front"),
                                      WorkflowElement("is ready for:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::BackSide),

                    JobWorkflowTuple(WorkflowStatus::InProgressStatus,
                                     {WorkflowElement("done:printing:front"),
                                      WorkflowElement("in progress:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::NotSetSide),
                    JobWorkflowTuple(WorkflowStatus::DoneStatus,
                                     {WorkflowElement("done:printing:front"),
                                      WorkflowElement("in progress:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::FrontSide),
                    JobWorkflowTuple(WorkflowStatus::InProgressStatus,
                                     {WorkflowElement("done:printing:front"),
                                      WorkflowElement("in progress:printing:back"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::BackSide),

                    JobWorkflowTuple(WorkflowStatus::NeedsStatus,
                                     {WorkflowElement("done:printing:front"), WorkflowElement("needs:printing:back"),
                                      WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::NotSetSide),
                    JobWorkflowTuple(WorkflowStatus::DoneStatus,
                                     {WorkflowElement("done:printing:front"), WorkflowElement("needs:printing:back"),
                                      WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::FrontSide),
                    JobWorkflowTuple(WorkflowStatus::NeedsStatus,
                                     {WorkflowElement("done:printing:front"), WorkflowElement("needs:printing:back"),
                                      WorkflowElement("needs:cutting")},
                                     WorkflowAction::PrintingAction, PaperSide::BackSide),

                    JobWorkflowTuple(WorkflowStatus::NeedsStatus,
                                     {WorkflowElement("is ready for:printing"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::CuttingAction, PaperSide::NotSetSide),
                    JobWorkflowTuple(WorkflowStatus::UnknownStatus,
                                     {WorkflowElement("is ready for:printing"), WorkflowElement("needs:cutting")},
                                     WorkflowAction::BoxingAction, PaperSide::NotSetSide)));

TEST_P(JobWorkflowTest, workflowStatus)
{
    auto job = Job::create();
    job->setWorkflow(std::get<1>(GetParam()));
    WorkflowStatus result = job->workflowStatus(std::get<2>(GetParam()), std::get<3>(GetParam()));
    EXPECT_EQ(std::get<0>(GetParam()), result);
}

INSTANTIATE_TEST_CASE_P(
    WorkflowStatusAfterTransitionTestParameters, WorkflowStatusAfterTransitionTest,
    testing::Values(StatusAfterTransitionTuple(WorkflowStatus::InProgressStatus, TransitionEvent::StartEvent),
                    StatusAfterTransitionTuple(WorkflowStatus::SuspendedStatus, TransitionEvent::SuspendEvent),
                    StatusAfterTransitionTuple(WorkflowStatus::InProgressStatus, TransitionEvent::ResumeEvent),
                    StatusAfterTransitionTuple(WorkflowStatus::DoneStatus, TransitionEvent::StopEvent),
                    StatusAfterTransitionTuple(WorkflowStatus::IsReadyForStatus, TransitionEvent::AbortEvent),
                    StatusAfterTransitionTuple(WorkflowStatus::IsReadyForStatus, TransitionEvent::RevertEvent),
                    StatusAfterTransitionTuple(WorkflowStatus::UnknownStatus, TransitionEvent::RequestEvent),
                    StatusAfterTransitionTuple(WorkflowStatus::UnknownStatus, TransitionEvent::PerformEvent),
                    StatusAfterTransitionTuple(WorkflowStatus::UnknownStatus, TransitionEvent::UnknownEvent)));

TEST_P(WorkflowStatusAfterTransitionTest, statusCheck)
{
    EXPECT_EQ(std::get<0>(GetParam()), workflowStatusAfterTransitionEvent(std::get<1>(GetParam())));
}
