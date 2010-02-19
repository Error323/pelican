#include "AdapterAntennaList.h"
#include <QTextStream>

#include "utility/memCheck.h"

namespace pelican {


// class AdapterAntennaList
AdapterAntennaList::AdapterAntennaList(const ConfigNode& config)
    : AbstractServiceAdapter(config)
{
    _nAnt = config.getOption("antennas", "number", "96").toUInt();
}


/**
 * @details
 */
AdapterAntennaList::~AdapterAntennaList()
{
}

/**
 * @details
 * Desearalise a LOFAR station antenna list file.
 * Assumes that the QIODevice points to a ascii text file handle.
 */
void AdapterAntennaList::deserialise(QIODevice* in)
{
    _setData();

    QTextStream s(in);

    real_t* x = _antPos->xPtr();
    real_t* y = _antPos->yPtr();
    real_t* z = _antPos->zPtr();
    real_t temp;

    for (unsigned a = 0; a < _nAnt; a++) {
        s >> x[a] >> y[a] >> z[a] >> temp >> temp >> temp;
//        std::cout << a << " " << x[a] << " " << y[a] << " " << z[a] << std::endl;
    }
}



/**
 * @details
 */
void AdapterAntennaList::_setData()
{
    if (_chunkSize == 0) {
        throw QString("No data to read. Stream chunk size set to zero.");
    }

    if (_data == NULL) {
        throw QString("Cannot deserialise into an unallocated blob!");
    }

    _antPos = static_cast<AntennaPositions*>(_data);
    _antPos->resize(_nAnt);
}



} // namespace pelican
