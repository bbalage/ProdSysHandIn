#ifndef PROD_SYS_HAND_IN_MODEL_MODELCREATOR_HPP
#define PROD_SYS_HAND_IN_MODEL_MODELCREATOR_HPP

#include <string>
#include "Model.hpp"

class ModelCreator
{
public:
    ModelCreator() {}
    ~ModelCreator() {}
    ModelCreator(const ModelCreator &other) = delete;
    ModelCreator &operator=(const ModelCreator &other) = delete;
    ModelCreator(ModelCreator &&other) = default;
    ModelCreator &operator=(ModelCreator &&other) = default;

    virtual Model createModel() const = 0;
};

class ModelCreatorRandom : public ModelCreator
{
public:
    ModelCreatorRandom() {}

    Model createModel() const override;
};

class ModelCreatorReader : public ModelCreator
{
    ModelCreatorReader(std::string desc) {}
};

#endif