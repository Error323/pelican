/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "PelicanServerClientTestMT.h"
#include "PelicanServerClient.h"
#include "server/test/TestServer.h"
#include "comms/StreamData.h"
#include "comms/DataChunk.h"
#include "core/test/TestServiceAdapter.h"
#include "core/test/TestStreamAdapter.h"
#include "data/test/TestDataBlob.h"
#include "utility/Config.h"
#include "utility/ConfigNode.h"

#include <QtCore/QCoreApplication>
#include <iostream>

using namespace std;

namespace pelican {

using test::TestDataBlob;
using test::TestServiceAdapter;
using test::TestStreamAdapter;
using test::TestServer;

CPPUNIT_TEST_SUITE_REGISTRATION( PelicanServerClientTestMT );
/**
 *@details PelicanServerClientTestMT
 */
PelicanServerClientTestMT::PelicanServerClientTestMT()
: CppUnit::TestFixture()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    _coreApp = new QCoreApplication(argc, argv);
}

/**
 *@details
 */
PelicanServerClientTestMT::~PelicanServerClientTestMT()
{
}

void PelicanServerClientTestMT::test_getData()
{
    try{
        // set up the test server
        TestServiceAdapter serviceAdapter;
        TestStreamAdapter streamAdapter;
        TestServer server;
        QString stream1("stream1");
        QString service1("service1");
        QString version1("version1");
        QString version2("version2");
        DataSpec reqStream1;
        reqStream1.addStreamData(stream1);
        QByteArray data1("data1");
        QByteArray data2("data2");
        unsigned int port = server.port();

        // common client configuration
        Config config;
        config.setFromString("",
                "<testconfig>"
                "   <server host=\"127.0.0.1\"/>"
                "</testconfig>"
        );
        Config::TreeAddress address;
        address << Config::NodeId("server", "");
        address << Config::NodeId("testconfig", "");
        ConfigNode configNode = config.get(address);
        {
            // Use Case:
            //   Request for Service Data only
            // Expect :
            //  Throw
            DataChunk sd(service1, version1, data1 );
            CPPUNIT_ASSERT_EQUAL( (long)data1.size(), (long)sd.size() );

            // setup the test
            DataSpec req;
            req.addServiceData(service1);
            QList<DataSpec> lreq;
            lreq.append(req);
            DataTypes dt;
            dt.setAdapter(service1, &serviceAdapter);
            dt.addData(lreq);
            PelicanServerClient client(configNode, dt, 0);
            client.setPort(port);

            QHash<QString, DataBlob*> dataHash;
            DataBlob db("DataBlob");
            dataHash.insert(service1, &db);
            CPPUNIT_ASSERT_THROW(client.getData(dataHash), QString);
        }
        {
            // Use Case:
            //   Single Request for an existing stream dataset
            //   no service data
            // Expect:
            //   return the required data stream
            StreamData sd(stream1, version1, data1 );
            CPPUNIT_ASSERT_EQUAL( (long)data1.size(), (long)sd.size() );
            server.serveStreamData(sd);

            // setup the test
            QList<DataSpec> lreq;
            lreq.append(reqStream1);
            DataTypes dt;
            dt.setAdapter(stream1, &streamAdapter);
            dt.addData(lreq);
            PelicanServerClient client(configNode, dt, 0);
            client.setPort(port);

            QHash<QString, DataBlob*> dataHash;
            TestDataBlob db;
            dataHash.insert(stream1, &db);

            client.getData(dataHash);
            CPPUNIT_ASSERT_EQUAL(version1.toStdString(), db.version().toStdString());
            CPPUNIT_ASSERT_EQUAL(std::string(data1.data()), std::string(db.data()));
        }
        {
            // Use Case:
            // Single Request for an existing stream dataset
            // with existing service data
            // Expect:
            // return the required data stream
            DataChunk servd(service1, version2, data2 );
            server.serveServiceData(servd);
            StreamData sd(stream1, version1, data1 );
            CPPUNIT_ASSERT_EQUAL( (long)data1.size(), (long)sd.size() );
            server.serveStreamData(sd);

            // setup the test
            DataSpec req;
            req.addServiceData(service1);
            req.addStreamData(stream1);
            QList<DataSpec> lreq;
            lreq.append(req);
            DataTypes dt;
            dt.setAdapter(stream1, &streamAdapter);
            dt.setAdapter(service1, &serviceAdapter);
            dt.addData(lreq);
            PelicanServerClient client(configNode, dt, 0);
            client.setPort(port);

            QHash<QString, DataBlob*> dataHash;
            TestDataBlob db;
            TestDataBlob db_service;
            dataHash.insert(stream1, &db);
            dataHash.insert(service1, &db_service);
            client.getData(dataHash);
            CPPUNIT_ASSERT_EQUAL( version1.toStdString(), db.version().toStdString() );
            CPPUNIT_ASSERT_EQUAL( version2.toStdString(), db_service.version().toStdString() );
            CPPUNIT_ASSERT_EQUAL( std::string(data2.data()), std::string(db_service.data()) );
            CPPUNIT_ASSERT_EQUAL( std::string(data1.data()), std::string(db.data()) );
        }
    }
    catch (const QString& e)
    {
        std::cerr << "error: uncaught exception" << e.toStdString();
        throw e;
    }
}

} // namespace pelican
