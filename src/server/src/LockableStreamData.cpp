#include "LockableStreamData.h"
#include "LockableData.h"
#include "LockedData.h"
#include "comms/Data.h"
#include "comms/StreamData.h"

#include <iostream>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * LockableStreamData constructor.
 */
LockableStreamData::LockableStreamData(const QString& name, void* memory,
        size_t size, QObject* parent) : AbstractLockableData(size, parent)
{
    _data.reset( new StreamData(name, memory, size) );
    _served = false;
}

LockableStreamData::~LockableStreamData()
{
}

void LockableStreamData::addAssociatedData(const LockedData& data)
{
    _serviceDataTypes.insert(data.name());
    _serviceData.append(data);
    LockableData* lockable = static_cast<LockableData*>(data.object());
    boost::shared_ptr<Data> dataPtr( lockable->data() );
    streamData()->addAssociatedData( dataPtr );
}

bool LockableStreamData::isValid() const
{
    return isValid( _serviceDataTypes );
    /*
    bool rv = Data::isValid();
    foreach(LockedData data, _serviceData )
    {
        rv = rv && data.isValid();
    }
    return rv;
    */
}

bool LockableStreamData::isValid(const QSet<QString>& associates) const
{
    Q_ASSERT( data() != 0 );
    bool rv = data()->isValid();
    foreach(const QString& assoc, associates )
    {
        foreach(LockedData data, _serviceData ) 
        {
            if( data.name() == assoc ) 
                rv = rv && data.isValid();
        }
    }
    return rv;
}

void LockableStreamData::reset()
{
    _serviceData.clear();
    _serviceDataTypes.clear();
    streamData()->reset();
}

StreamData* LockableStreamData::streamData() const { 
    return static_cast<StreamData*>(_data.get()); 
}

} // namespace pelican
