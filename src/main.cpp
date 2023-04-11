#include <iostream>
#include <vector>

#include "Model/ModelCreator.hpp"
#include "Planner/Planner.hpp"
#include "Simulator/Simulator.hpp"
#include "Evaluator/Evaluator.hpp"
#include "util/output.hpp"

int main(int argc, char **argv)
{
    // Build model
    ModelCreatorRandom modelCreator;
    auto model = modelCreator.createModel();

    // Simulate
    PlannerSimple planner;
    SimulatorSimple simulator;
    simulator.simulate(model, planner);

    // Evaluate
    // Eval eval = evaluate(model, report);

    // Output
    // print_eval(eval);
}