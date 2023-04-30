#include "Planner.hpp"

Plan PlannerSimple::plan(const Model &model, const ModelState &modelState, const std::vector<Order> &newOrders, long t_ref) const
{
    Plan plan{
        .invalid = false};

    // Create jobs from orders
    for (i_t orI = 0; orI < newOrders.size(); ++orI)
    {
        const auto &order = newOrders[orI];
        for (const auto &orderedProd : order.products)
        {
            for (uint amI = 0; amI < orderedProd.amount; ++amI)
            {
                const i_t prodI = orderedProd.thing;
                const auto &prod = model.products[prodI];

                // Randomly choose which way to produce the product
                i_t tpI = randBtw(0, prod.techPlans.size());
                const TechPlan &tp = model.techPlans[tpI];
                plan.jobs.push_back(Job{.order = orI,
                                        .product = prodI,
                                        .techPlan = tpI,
                                        .opLogs = std::vector<JobOpLog>(
                                            tp.operations.size(),
                                            JobOpLog{
                                                .startTime = 0,
                                                .endTime = 0,
                                                .finished = false})});
            }
        }
    }

    // Create schedule matrix from jobs

    plan.sch_matrix.resize(model.workstations.size(), std::vector<JobOp>());
    std::vector<i_t> workstationTypeCounters(model.workstationTypes, 0);
    for (i_t jI = 0; jI < plan.jobs.size(); ++jI)
    {
        i_t tpI = plan.jobs[jI].techPlan;
        const auto &tp = model.techPlans[tpI];
        for (i_t opI = 0; opI < tp.operations.size(); ++opI)
        {
            const Operation &op = tp.operations[opI];
            i_t &counter = workstationTypeCounters[op.workstationType];
            const auto &wss_of_type = model.workstationTypeMap.at(op.workstationType);
            i_t wsI = wss_of_type[counter];
            ++counter;
            if (counter == wss_of_type.size())
                counter = 0;

            plan.sch_matrix[wsI].push_back(JobOp{.job = jI, .op = opI});
        }
    }

    return plan;
}