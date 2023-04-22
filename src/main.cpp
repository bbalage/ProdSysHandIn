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

    // Plan
    PlannerSimple planner;
    Plan plan = planner.plan(model);

    // Simulate
    SimulatorSimple simulator;
    simulator.simulate(model, plan);

    // Evaluate
    // Eval eval = evaluate(model, report);

    // Output
    // print_eval(eval);
}