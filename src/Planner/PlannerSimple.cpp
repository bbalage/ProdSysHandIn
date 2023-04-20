#include "Planner.hpp"

Plan PlannerSimple::plan(const Model &model) const
{
    Plan plan;
    plan.sch_matrix.resize(model.workstations.size(), std::vector<Job>());
    std::vector<Job> jobs;
    std::vector<i_t> workstationTypeCounters(model.workstationTypes, 0);
    for (const auto &order : model.orders)
    {
        for (const auto &orderedProd : order.products)
        {
            for (uint amI = 0; amI < orderedProd.amount; ++amI)
            {
                const auto &prod = model.products[orderedProd.thing];

                // Randomly choose which way to produce the product
                auto tpI = prod.techPlans[randBtw(0, prod.techPlans.size())];
                const TechPlan &tp = model.techPlans[tpI];
                for (i_t opI = 0; opI < tp.operations.size(); ++opI)
                {
                    const Operation &op = tp.operations[opI];
                    i_t &counter = workstationTypeCounters[op.workstationType];
                    const auto &wss_of_type = model.workstationTypeMap.at(op.workstationType);
                    i_t wsI = wss_of_type[counter];
                    ++counter;
                    if (counter == wss_of_type.size())
                        counter = 0;
                    plan.sch_matrix[wsI].push_back(Job{
                        .techPlan = tpI,
                        .operation = opI});
                }
            }
        }
    }

    return plan;
}