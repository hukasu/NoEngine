/**
 * Engine
 * © Lucas "Hukasu" Farias
 */

#ifndef __ENGINE__HPP__
#define __ENGINE__HPP__

#ifndef UNICODE
#define UNICODE
#endif 

#include "engine_instance/engine_instance.hpp"

namespace engine {
    /// <summary>Makes a version number, <paramref name='_major' /> and <paramref name='_minor' /> values should 
    /// not be bigger than 1023, <paramref name='_patch' /> should not be bigger than 4095.</summary>
    /// <param name='_major'>The major version</param>
    /// <param name='_minor'>The minor version</param>
    /// <param name='_patch'>The patch version</param>
    /// <exception cref='engine::EngineError'>Throw if <paramref name='_major' />, <paramref name='_minor' />,
    /// or <paramref name='_patch' /> have an value higher than allowed.</exception>
    /// <returns>Version number</returns>
    uint32_t makeVersionNumber(uint16_t _major, uint16_t _minor, uint16_t _patch);

    /// <summary>Creates an Engine Instance</summary>
    /// <param name='_engine_instance_create_info'>Information about the engine instance</param>
    /// <returns>An instance of the engine</returns>
    EngineInstance createEngineInstance(EngineInstanceCreateInfo* _engine_instance_create_info);
}

#endif // __ENGINE__HPP__