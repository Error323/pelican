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

#ifndef EMULATORDRIVER_H
#define EMULATORDRIVER_H

/**
 * @file EmulatorDriver.h
 */

#include "emulator/EmulatorDriver.h"
#include "utility/ConfigNode.h"
#include <QtCore/QThread>

class QIODevice;

namespace pelican {

class AbstractEmulator;

/**
 * @ingroup c_emulator
 *
 * @class EmulatorDriver
 *
 * @brief Drives the data emulator using its own thread.
 *
 * @details
 * This class is constructed with a pointer to the data emulator to use.
 * The emulator driver takes ownership of the emulator, and will delete
 * it on destruction.
 *
 * The thread created by this class repeatedly calls getPacketData() on the
 * emulator, and then sleeps for the specified interval.
 */
class EmulatorDriver : public QThread
{
    Q_OBJECT

    public:
        /// Constructor.
        EmulatorDriver(AbstractEmulator* emulator);

        /// Destructor.
        ~EmulatorDriver();

        /// Return the emulator object
        AbstractEmulator* emulator() const { return _emulator; };

        /// Return the number of bytes that have been sent
        unsigned long dataCount() const { return _dataCount; };

        /// Return the number of packets sent by the emulator.
        long int packetCount() const { return _packetCount; }

        /// Stops the emulator driver.
        void abort() { _abort = true; }

    protected:
        /// Runs the thread owned by the emulator driver.
        void run();

    private:
        bool _abort;
        QIODevice* _device;
        AbstractEmulator* _emulator;
        unsigned long _dataCount;
        long int _packetCount;
};

} // namespace pelican

#endif // EMULATORDRIVER_H
