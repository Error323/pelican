#include "pelican/server/AbstractChunker.h"
#include "pelican/utility/ConfigNode.h"

#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

AbstractChunker::AbstractChunker(const ConfigNode& config)
{
    // Initialise members.
    _dataManager = NULL;
    _device = NULL;
    _type = config.getOption("data", "type", "");
    _host = config.getOption("connection", "host", "");
    _port = config.getOption("connection", "port", "0").toUInt();
}

/**
 * @details
 * Destroys the AbstractChunker.
 */
AbstractChunker::~AbstractChunker()
{
    // FIXME Why don't we delete the socket or device here?
//    if (_device)
//        delete _device;
}

/**
 * @details
 * Returns a writable data object of the specified \p size.
 *
 * This method should be used by the chunker to access memory in a stream
 * or service FIFO buffer to ensure that the chunker only ever writes into an
 * available, locked location.
 *
 * @param[in] size  The size of the chunk requested on the buffer.
 */
WritableData AbstractChunker::getDataStorage(size_t size) const
{
    if (!_dataManager)
        throw QString("AbstractChunker::getDataStorage(): No data manager.");
    return _dataManager->getWritableData(_type, size);
}

} // namespace pelican
