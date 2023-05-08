#include "ModelHandler.hpp"

void ModelHandler::addOrders(std::vector<Order> orders)
{
    // Add new orders
    ModelState new_mstate = m_mstate_current;

    // Create plan
    Plan new_plan = m_planner.plan(m_model, m_plan, orders, new_mstate, m_t_cur);

    // Optimize the plan
    ModelState sim_mstate = m_simulator.simulate(m_model, new_mstate, new_plan, m_t_cur);
    Eval eval = analyse(m_model, sim_mstate);
    std::cout << "Before optimization:" << std::endl;
    print_eval(eval);

    ModelState out_mstate;
    m_optimizer.optimize(m_model, new_mstate, new_plan, out_mstate, eval, m_t_cur);

    std::cout << "After optimization:" << std::endl;
    print_eval(eval);

    // Accept the new plan and state
    m_mstate_current = new_mstate;
    m_mstate_predicted = out_mstate;
    m_plan = new_plan;
}

void ModelHandler::advanceTime(long t_adv)
{
    auto t_cur = m_t_cur + t_adv;
    ModelState newState;

    // 1. Set the current state of the model based on the predicted state

    // 2. Remove old data that has become irrelevant

    Plan newPlan{.invalid = false};
    std::vector<i_t> completedOrders;
    for (i_t orI = 0; orI < m_mstate_predicted.orderLogs.size(); ++orI)
    {
        const OrderLog &orderLog = m_mstate_predicted.orderLogs[orI];

        // If order is already completed, just skip... TODO: create some logs
        if (orderLog.completionTime <= t_cur) // Order is completed
        {
            completedOrders.push_back(orI);
            // continue;
        }

        // // Copy the order
        // auto new_orI = newPlan.orders.size();
        // newPlan.orders.push_back(m_plan.orders[orI]);

        // // Copy the order-job mappings
        // auto [old_from, old_to] = m_plan.jobsFromToPerOrder[orI];
        // auto new_from = m_plan.jobs.size();
        // auto new_to = m_plan.jobs.size() + (old_to - old_from);
        // newPlan.jobsFromToPerOrder.push_back(std::make_pair(new_from, new_to));

        // // Copy the jobs
        // newPlan.jobs.insert(newPlan.jobs.end(), &(m_plan.jobs[old_from]), &(m_plan.jobs[old_to]));
        // for (size_t i = new_from; i < new_to; ++i)
        // {
        //     newPlan.jobs[i].order = new_orI;
        // }
    }
    // Copy schedule matrix
    std::vector<bool> orderRemovable(m_plan.orders.size(), true);
    for (i_t wsI = 0; wsI < m_model.workstations.size(); ++wsI)
    {
        const auto &wsSch = m_plan.sch_matrix[wsI];
        for (const JobOp &jobOp : wsSch)
        {
            if (m_mstate_predicted.jobOpLogs[jobOp.job][jobOp.op].endTime > t_cur)
                newPlan.sch_matrix[wsI].push_back(jobOp);
        }
    }
}

AmountLogs ModelHandler::calcLogs() const
{
    return m_amountLogger.calcAmountLogs(m_model, m_mstate_predicted, m_plan, m_t_cur, 10);
}
