#ifndef CHUNKEREXAMPLE_H
#define CHUNKEREXAMPLE_H

#include "pelican/server/AbstractChunker.h"

/**
 * @file ChunkerExample.h
 */

using namespace pelican;

/**
 * @class ChunkerExample
 *  
 * @brief
 * A simple example to demonstrate how to write a data chunker.
 */
class ChunkerExample : public AbstractChunker
{
    public:
        ChunkerExample(const ConfigNode& config);
        virtual QIODevice* newDevice();
        virtual void next(QIODevice*);

    private:
        qint64 _chunkSize;
        qint64 _bytesRead;
};

// Register the chunker.
PELICAN_DECLARE_CHUNKER(ChunkerExample)

#endif // CHUNKEREXAMPLE_H 
