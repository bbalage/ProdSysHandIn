#include <iostream>
#include <vector>
#include <fstream>

#include "Planner/Planner.hpp"
#include "Simulator/Simulator.hpp"
#include "Evaluator/Evaluator.hpp"
#include "util/output.hpp"
#include "Model/Model.hpp"
#include "Model/ModelHandler.hpp"
#include "Optimizer/Optimizer.hpp"

int main(int argc, char **argv)
{
    // Initialize
    // std::mt19937 rng(time(0));
    srand(time(0));
    constexpr long t_frozen = 60;

    // Build model
    PlannerSimple planner(t_frozen);
    SimulatorSimple simulator;
    AmountLoggerSimple amountLogger;
    Evaluator evaluator(EvalWeights{.wCmax = 1,
                                    .wCsum = 1,
                                    .wLmax = 10,
                                    .wTsum = 1,
                                    .wUsum = 100});
    OptimizerLocalSearch optimizer(simulator, evaluator, 100, 100);
    long t_ref = 1440;

    ModelHandler mhandler(
        planner, simulator, optimizer, amountLogger, generateRandomModel(), t_ref);
    const Model &model = mhandler.model();

    // Receive order...
    // e2e test:
    // generate 5 orders
    auto orders = generateOrders(5, model.products.size());
    mhandler.addOrders(orders);
    orders = generateOrders(2, model.products.size());
    mhandler.addOrders(orders);
    AmountLogs amLogs = mhandler.calcLogs();

    std::ofstream ofstream;
    ofstream.open("/home/bbalage/For_University/Bead/prod_sys/ProdSysHandIn/resources/prod_log.txt", std::ios_base::out);
    print_prodAmountLogs(ofstream, amLogs);
    ofstream.close();

    ofstream.open("/home/bbalage/For_University/Bead/prod_sys/ProdSysHandIn/resources/mat_log.txt", std::ios_base::out);
    print_matAmountLogs(ofstream, amLogs);
    ofstream.close();

    // Plan, simulate

    // advance time
    // generate 3 more orders
    // advance time

    // Output
    // std::cout << "Before: " << std::endl;
    // print_gantt(n_mstate);
    // print_eval(eval);

    // optimizer.optimize(model, mstate, plan, n_mstate, eval, t_ref);

    // std::cout << "After: " << std::endl;
    // print_gantt(n_mstate);
    // print_eval(eval);

    // Planning model:
    // - Given a state of the model new orders arrive
    // - Create a plan based on the current state
    // - Optimize the new plan
    // - Install new plan
    // - Advance time
    return 0;
}