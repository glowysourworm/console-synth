/*
 * AirwinConsolidated - an adaptation of the airwindows effect suite
 * for various open source clients
 *
 * This source released under the MIT License, found in ~/LICENSE.md.
 *
 * Copyright 2023 by the authors as described in the github transaction log
 */

#ifndef AIRWINCONSOLIDATED_AIRWINREGISTRY_H
#define AIRWINCONSOLIDATED_AIRWINREGISTRY_H

#include <map>
#include <string>
#include <vector>

#include "AirwinRegistryEntry.h"

class AirwinRegistry
{
public:

    AirwinRegistry();
    ~AirwinRegistry();

    /// <summary>
    /// Loads the effects using registry text files: .txt registry, and all text files for specific effects
    /// </summary>
    bool Load(float samplingRate);

    void GetPlugins(std::vector<std::string>& destination);
    void GetCategories(std::vector<std::string>& destination);
    void GetCategoryList(std::vector<std::string>& destination, const std::string& category);

private:

    void RegisterAirwindow(const AirwinRegistryEntry& r);

    private:

        std::map<std::string, AirwinRegistryEntry*>* _registry;
        std::map<std::string, std::vector<AirwinRegistryEntry*>*>* _byCategory;
};
#endif // AIRWINCONSOLIDATED_AIRWINREGISTRY_H
