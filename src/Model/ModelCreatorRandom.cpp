#include "ModelCreator.hpp"
#include <cstdlib>
#include <time.h>

Model ModelCreatorRandom::createModel() const
{
    srand(time(0));
    Model model;
    // constexpr uint numberOfCargos = 8;
    // constexpr uint numberOfAgents = 3;
    // for (uint i = 0; i < numberOfCargos; ++i)
    // {
    //     Vec3 pos{
    //         rand() % 100,
    //         rand() % 100,
    //         0};
    //     model.cargos.push_back(Cargo{.pos = pos, .dueDelivery = 100, .weight = 10});
    // }
    // for (uint i = 0; i < numberOfAgents; ++i)
    // {
    //     Vec3 pos{
    //         rand() % 100,
    //         rand() % 100,
    //         0};
    //     model.agents.push_back(Agent{.pos = pos, .weightCap = 10, .speed = 1.0});
    // }
    return model;
}