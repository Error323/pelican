#include "PipelineApplicationTest.h"
#include "PipelineApplication.h"
#include <QCoreApplication>


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( PipelineApplicationTest );
// class PipelineApplicationTest 
PipelineApplicationTest::PipelineApplicationTest()
    : CppUnit::TestFixture()
{
}

PipelineApplicationTest::~PipelineApplicationTest()
{
}

void PipelineApplicationTest::setUp()
{
}

void PipelineApplicationTest::tearDown()
{
}

void PipelineApplicationTest::test_noQApplication()
{
    int argc = 0;
    PipelineApplication* pApp = 0;

    CPPUNIT_ASSERT_THROW( new PipelineApplication(0, NULL), QString );

    QCoreApplication app( argc, NULL  );
    CPPUNIT_ASSERT_NO_THROW( pApp = new PipelineApplication(0, NULL) );
    delete pApp;
}

} // namespace pelican