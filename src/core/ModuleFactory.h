#ifndef MODULEFACTORY_H
#define MODULEFACTORY_H

#include <QString>
#include <QVector>
#include "utility/Config.h"

class QDomElement;

/**
 * @file ModuleFactory.h
 */

namespace pelican {

class AbstractModule;

/**
 * @class ModuleFactory
 *  
 * @brief
 * Factory to create pre-configured modules.
 * 
 * @details
 * Instantiates module objects and configures them with details taken
 * from the appropriate configuration file and/or command line.
 * The modules are owned by the factory and will be destroyed with
 * the factory.
 */
class ModuleFactory
{
    public:
        /// Module factory constructor.
        ModuleFactory(const Config *config);

        /// Module factory destructor.
        ~ModuleFactory();

        /// Creates a new module.
        AbstractModule* createModule(const QString& name);

        /// Returns the tree node address that marks the start
        /// of the module configuration block.
        Config::TreeAddress_t configRoot() const { return _configRoot; }

    private:
        /// Creates a new module.
        AbstractModule* _createModule(const QString& name, const QDomElement& config);

    private:
        const Config* _config; ///< Pointer to the configuration object.
        Config::TreeAddress_t _configRoot;
        QVector<AbstractModule*> _modules; ///< Holds pointers to the created modules.
};

} // namespace pelican

#endif // MODULEFACTORY_H 
