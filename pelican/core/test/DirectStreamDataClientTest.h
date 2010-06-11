#ifndef DIRECTSTREAMDATACLIENTTEST_H
#define DIRECTSTREAMDATACLIENTTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file DirectStreamDataClientTest.h
 */

class QCoreApplication;

namespace pelican {

class Config;
class ConfigNode;
class VisibilityData;

/**
 * @class DirectStreamDataClientTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class DirectStreamDataClientTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DirectStreamDataClientTest );
//        FIXME: Visibility dat cant be used for general pelican framwork testing.
//        CPPUNIT_TEST( test_singleChunker ); 
//        FIXME: LofarVisibility adapter cant be used for general testing.
//        CPPUNIT_TEST( test_twoChunkersMultipleStarts );      
//        CPPUNIT_TEST( test_twoChunkersSingleStart );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_singleChunker();
        void test_twoChunkersMultipleStarts();
        void test_twoChunkersSingleStart();

    public:
        DirectStreamDataClientTest(  );
        ~DirectStreamDataClientTest();

    private:
        void _printVisibilities(VisibilityData* visData);

    private:
        QCoreApplication* _app;
        Config* _config;
        ConfigNode* _emulatorConfig1;
        ConfigNode* _emulatorConfig2;
};

} // namespace pelican

#endif // DIRECTSTREAMDATACLIENTTEST_H 
