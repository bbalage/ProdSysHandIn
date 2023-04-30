#ifndef PROD_SYS_HAND_IN_MODEL_MODELHANDLER_HPP
#define PROD_SYS_HAND_IN_MODEL_MODELHANDLER_HPP

#include <memory>

class ModelHandler
{
    ModelHandler() {}
    ~ModelHandler() {}
    ModelHandler(const ModelHandler &other) = delete;
    ModelHandler &operator=(const ModelHandler &other) = delete;
    ModelHandler(ModelHandler &&other) = default;
    ModelHandler &operator=(ModelHandler &&other) = default;
};

#endif