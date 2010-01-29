#include "StreamDataRequest.h"

#include "data/DataRequirements.h"
#include "utility/memCheck.h"

namespace pelican {


// class StreamDataRequest 
StreamDataRequest::StreamDataRequest()
    : ServerRequest(ServerRequest::StreamData)
{
}

StreamDataRequest::~StreamDataRequest()
{
}

void StreamDataRequest::addDataOption(const DataRequirements& data)
{
    _dataOptions.append(data);
}

StreamDataRequest::DataRequirementsIterator StreamDataRequest::begin() const
{
    return _dataOptions.begin();
}

StreamDataRequest::DataRequirementsIterator StreamDataRequest::end() const
{
    return _dataOptions.end();
}

} // namespace pelican