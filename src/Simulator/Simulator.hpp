#ifndef PROD_SYS_HAND_IN_SIMULATOR_SIMULATOR_HPP
#define PROD_SYS_HAND_IN_SIMULATOR_SIMULATOR_HPP

#include "../Model/Model.hpp"
#include "../Planner/Planner.hpp"

struct Report
{
    double timeElapsed;
};

class Simulator
{
public:
    Simulator() {}
    ~Simulator() {}
    Simulator(const Simulator &other) = delete;
    Simulator &operator=(const Simulator &other) = delete;
    Simulator(Simulator &&other) = default;
    Simulator &operator=(Simulator &&other) = default;

    /**
     * Sets which model to simulate and what planner to use.
     *
     * @param model
     * @param planner
     * @param t_ref The starting (reference) time of the simulation.
     */
    virtual void simulate(Model &model, Planner &planner, double t_ref) = 0;
};

class SimulatorSimple : public Simulator
{
public:
    SimulatorSimple() {}

    void simulate(Model &model, Planner &planner, double t_ref = 0) override;
};

#endif