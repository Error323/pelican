#include "OutputStreamer.h"
#include "pelican/server/AbstractBlobServer.h"
#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details OutputStreamer 
 */
OutputStreamer::OutputStreamer( const ConfigNode config )
    : AbstractModule(config)
{
    // Get _name
    // TODO set up the _server form the config
    //_server = serverFactory->create(config.getParmaeter("serverType");
}

/**
 *@details
 */
OutputStreamer::~OutputStreamer()
{
}

void OutputStreamer::send(DataBlob* blob)
{
    _server->queue(_name, blob);
}

} // namespace pelican
