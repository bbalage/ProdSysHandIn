#include "Planner.hpp"

Plan PlannerSimple::plan(const Model &model,
                         const Plan &oldPlan,
                         const std::vector<Order> &newOrders,
                         ModelState &modelState,
                         long t_ref) const
{
    // 2 requirements (TODO):
    // - new orders in time
    // - planning given a base plan...

    Plan plan = oldPlan;
    const auto oldNumberOfJobs = plan.jobs.size();
    plan.orders.insert(plan.orders.end(), newOrders.begin(), newOrders.end());

    // Create jobs from orders

    for (i_t orI = oldPlan.orders.size(); orI < plan.orders.size(); ++orI)
    {
        const auto &order = plan.orders[orI];
        for (const auto &orderedProd : order.products)
        {
            for (uint amI = 0; amI < orderedProd.amount; ++amI)
            {
                const i_t prodI = orderedProd.thing;
                const auto &prod = model.products[prodI];

                // Randomly choose which way to produce the product
                i_t tpI = randBtw(0, prod.techPlans.size());
                plan.jobs.push_back(Job{.order = orI,
                                        .product = prodI,
                                        .techPlan = tpI});
            }
        }
    }
    modelState.jobOpLogs.resize(plan.jobs.size());
    // TODO: CONTINUE HERE
    // for (size_t i = 0; i < oldPlan.jobs.size(); ++i)

    // Find out which jobs can be reordered
    // 1. For each row of the wsOpLogs matrix, get the last job that can be reordered
    // 2. Preload the schedule matrix

    // Create schedule matrix from jobs

    std::vector<i_t> workstationTypeCounters(model.workstationTypes, 0);
    for (i_t jI = oldNumberOfJobs; jI < plan.jobs.size(); ++jI)
    {
        Job &job = plan.jobs[jI];
        const auto &tp = model.techPlans[job.techPlan];
        for (i_t opI = 0; opI < tp.operations.size(); ++opI)
        {
            const Operation &op = tp.operations[opI];
            i_t &counter = workstationTypeCounters[op.workstationType];
            const auto &wss_of_type = model.workstationTypeMap.at(op.workstationType);
            i_t wsI = wss_of_type[counter];
            ++counter;
            if (counter == wss_of_type.size())
                counter = 0;

            // job.sch.push_back(WSSchPair{.ws = wsI, .sch = plan.sch_matrix[wsI].size()});
            plan.sch_matrix[wsI].push_back(JobOp{.job = jI, .op = opI});
        }
    }

    return plan;
}