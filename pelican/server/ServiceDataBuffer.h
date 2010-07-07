#ifndef SERVICEDATABUFFER_H
#define SERVICEDATABUFFER_H

#include "pelican/server/AbstractDataBuffer.h"

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QHash>


/**
 * @file ServiceDataBuffer.h
 */

namespace pelican {

class LockableServiceData;
class LockedData;


/**
 * @class ServiceDataBuffer
 *
 * @brief
 *    Maintains service data
 * @details
 *    Access to service data is assumed to be by direct reference
 *    either through an explicit identifier, or through the current()
 *    method.
 *    Multiple threads may access the same Data at the same time for
 *    reading.
 *
 */

class ServiceDataBuffer : public AbstractDataBuffer
{
    Q_OBJECT

    public:
        ServiceDataBuffer(const QString& type);
        ~ServiceDataBuffer();
        /// return a specific version of the data
        //  if this data is unavailable then return an invalid
        void getData(LockedData& locker, const QString& version);

        /// return the current version
        void getCurrent(LockedData& locker);

        /// getWritable returns an object for filling
        //  this object is locked and on unlocking
        //  will become the current.
        WritableData getWritable(size_t size);

    protected slots:
        void activateData();
        void deactivateData();

    protected:
        void activateData(LockableServiceData*);
        void deactivateData(LockableServiceData*);

    private:
        QHash<QString, LockableServiceData*> _data;
        QList<LockableServiceData*> _expiredData;
        LockableServiceData* _newData; // temporary store until activated
        QString _current;
        size_t _max;
        size_t _maxChunkSize;
        size_t _space;
        unsigned long _id;

    friend class ServiceDataBufferTest;
};

} // namespace pelican
#endif // SERVICEDATABUFFER_H
