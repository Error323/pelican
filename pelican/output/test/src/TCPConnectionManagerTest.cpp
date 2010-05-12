#include "TCPConnectionManagerTest.h"
#include "TCPConnectionManager.h"
#include <QTcpSocket>
#include <QCoreApplication>
#include "pelican/comms/ServerRequest.h"
#include "pelican/comms/ServerResponse.h"
#include "pelican/comms/DataBlobResponse.h"
#include "pelican/comms/StreamDataRequest.h"
#include "pelican/comms/PelicanClientProtocol.h"
#include "pelican/data/test/TestDataBlob.h"


#include "pelican/utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( TCPConnectionManagerTest );
/**
 *@details TCPConnectionManagerTest 
 */
TCPConnectionManagerTest::TCPConnectionManagerTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
TCPConnectionManagerTest::~TCPConnectionManagerTest()
{
}

void TCPConnectionManagerTest::setUp()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    _app = new QCoreApplication(argc,argv);

    _clientProtocol = new PelicanClientProtocol;
    _server = new TCPConnectionManager(0);
    _server->start();
    bool i;
    do { i = _server->isRunning(); sleep(1); } while ( ! i );
}

void TCPConnectionManagerTest::tearDown()
{
    _server->quit();
    delete _server;
    delete _clientProtocol;
    delete _app;
}

void TCPConnectionManagerTest::test_send()
{
    // Use Case:
    // client requests a connection
    // expect client to be registered for any data
    StreamDataRequest req;
    DataRequirements require;
    require.setStreamData("testData");
    req.addDataOption(require);

    QTcpSocket* client = _createClient();
    _sendRequest( client, req );
    sleep(1);
    CPPUNIT_ASSERT_EQUAL( 1, _server->clientsForType("testData") );
    TestDataBlob blob;
    blob.setData("sometestData");
    _server->send("testData",blob);
    sleep(1);

    CPPUNIT_ASSERT( client->state() == QAbstractSocket::ConnectedState );
    boost::shared_ptr<ServerResponse> r = _clientProtocol->receive(*client);
    CPPUNIT_ASSERT( r->type() == ServerResponse::Blob );
    DataBlobResponse* res = static_cast<DataBlobResponse*>(r.get());
    CPPUNIT_ASSERT( res->dataName() == "testData" );
    CPPUNIT_ASSERT( res->blobClass() == "TestDataBlob" );
    TestDataBlob recvBlob;
    recvBlob.deserialise(*client);
    CPPUNIT_ASSERT(recvBlob == blob);

}

void TCPConnectionManagerTest::test_brokenConnection()
{
    // Use Case:
    // client requests a connection
    // expect client to be registered for any data
    StreamDataRequest req;
    DataRequirements require;
    require.setStreamData("testData");
    req.addDataOption(require);

    QTcpSocket* client = _createClient();
    _sendRequest( client, req );
    sleep(1);
    CPPUNIT_ASSERT_EQUAL( 1, _server->clientsForType("testData") );

    // Use Case:
    // client dies after connection
    // expect to be removed from the system
    delete client;
    sleep(1);
    CPPUNIT_ASSERT_EQUAL( 0, _server->clientsForType("testData") );
}

void TCPConnectionManagerTest::test_multiClients()
{
}

QTcpSocket* TCPConnectionManagerTest::_createClient() const
{
   // Create a client and connect it to the server
   QTcpSocket* tcpSocket = new QTcpSocket;
   tcpSocket->connectToHost( QHostAddress::LocalHost, _server->serverPort() );
   if (!tcpSocket->waitForConnected(5000) || tcpSocket->state() == QAbstractSocket::UnconnectedState)
   {
       delete tcpSocket;
       tcpSocket = 0;
       CPPUNIT_FAIL("Client could not connect to server");
   }
   return tcpSocket;
}

void TCPConnectionManagerTest::_sendRequest(QTcpSocket* tcpSocket, const ServerRequest& req)
{
    QByteArray data = _clientProtocol->serialise(req);
    tcpSocket->write(data);
    tcpSocket->waitForBytesWritten(data.size());
    tcpSocket->flush();
}

} // namespace pelican