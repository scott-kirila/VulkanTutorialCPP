//
// Created by hetan on 6/10/2024.
//

#ifndef VALIDATIONLAYERSMANAGER_H
#define VALIDATIONLAYERSMANAGER_H
#include <vector>


struct ValidationLayersManager {
    bool CheckValidationLayerSupport() const;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
};



#endif //VALIDATIONLAYERSMANAGER_H
