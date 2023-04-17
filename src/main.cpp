#include <iostream>
#include <vector>

#include "Planner/Planner.hpp"
#include "Simulator/Simulator.hpp"
#include "Evaluator/Evaluator.hpp"
#include "util/output.hpp"
#include "Model/Model.hpp"

int main(int argc, char **argv)
{
    // Initialize
    // std::mt19937 rng(time(0));
    srand(time(0));

    // Build model
    auto model = createRandomModel();

    // Simulate
    PlannerSimple planner;
    SimulatorSimple simulator;
    simulator.simulate(model, planner);

    // Evaluate
    // Eval eval = evaluate(model, report);

    // Output
    // print_eval(eval);
}