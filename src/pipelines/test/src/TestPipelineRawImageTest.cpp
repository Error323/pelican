#include "TestPipelineRawImageTest.h"
#include "core/PipelineApplication.h"
#include "core/AbstractPipeline.h"
#include "pipelines/TestPipelineRawImage.h"
#include <QCoreApplication>

#include "utility/pelicanTimer.h"
#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( TestPipelineRawImageTest );
/**
 *@details TestPipelineRawImageTest
 */
TestPipelineRawImageTest::TestPipelineRawImageTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
TestPipelineRawImageTest::~TestPipelineRawImageTest()
{
}

void TestPipelineRawImageTest::setUp()
{
    int ac = 0;
    _app = new QCoreApplication(ac, NULL);
}

void TestPipelineRawImageTest::tearDown()
{
    delete _app;
}

void TestPipelineRawImageTest::test_method()
{
    TIMER_START
    int argc = 2;
    char *argv[] = {
            (char*)"pelican",
            (char*)"--config=data/TestPipelineRawImage.xml"
    };
    QCoreApplication app(argc, argv);

    PipelineApplication pApp(argc, argv);
    pApp.registerPipeline(new TestPipelineRawImage);
    pApp.setDataClient("FileDataClient");
    pApp.start();
    TIMER_STOP("TestPipelineRawImageTest");
}

} // namespace pelican