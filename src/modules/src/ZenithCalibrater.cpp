#include "modules/ZenithCalibrater.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Module constructor.
 */
ZenithCalibrater::ZenithCalibrater(const QDomElement& config)
    : AbstractModule(config)
{
}

/**
 * @details
 * Module destructor.
 */
ZenithCalibrater::~ZenithCalibrater()
{
}

} // namespace pelican