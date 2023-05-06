#include "Optimizer.hpp"
#include <algorithm>

void OptimizerLocalSearch::optimize(const Model &model,
                                    const ModelState &in_modelState,
                                    Plan &inout_plan,
                                    ModelState &out_modelState,
                                    Eval &out_eval,
                                    long t_ref) const
{
    Plan plan_0 = inout_plan;    // start base
    Plan plan_best = inout_plan; // best solution of the search
    Plan plan_actual;            // actual
    Plan plan_extension;         // best neighbor

    ModelState mstate_actual;
    ModelState mstate_extension;

    Eval eval_best;
    Eval eval_actual;
    Eval eval_extension;

    ModelState mstate_0 = m_sim.simulate(model, in_modelState, plan_0, t_ref);
    ModelState mstate_best = mstate_0;
    eval_best = analyse(model, mstate_0);

    for (uint step = 1; step <= m_steps; ++step)
    {
        for (uint loop = 1; loop <= m_loops; ++loop)
        {
            plan_actual = mutate(mstate_0, plan_0, t_ref);
            mstate_actual = m_sim.simulate(model, in_modelState, plan_actual, t_ref);
            eval_actual = analyse(model, mstate_actual);
            if (loop == 1 || m_eval.targetF(eval_extension, eval_actual) < 0.0)
            {
                plan_extension = plan_actual;
                eval_extension = eval_actual;
                mstate_extension = mstate_actual;
            }
        }
        plan_0 = plan_extension;
        if (m_eval.targetF(eval_best, eval_extension) < 0.0)
        {
            plan_best = plan_extension;
            eval_best = eval_extension;
            mstate_best = mstate_extension;
        }
    }

    inout_plan = plan_best;
    out_modelState = mstate_best;
    out_eval = eval_best;
}

Plan mutate(const ModelState &mstate, const Plan &plan_0, long t_ref)
{
    Plan plan = plan_0;
    // Possible mutations:
    // 1. Change tech plan
    // 2. Change which operation runs on which machine in a group
    // 3. Change the order of operations on a given machine.

    // 3. is implemented here
    const auto wsI = randBtw(0, plan.sch_matrix.size() - 1);
    auto &jobOps = plan.sch_matrix[wsI];
    if (jobOps.size() < 2)
        return plan;
    const auto &it = std::lower_bound(jobOps.begin(), jobOps.end(), t_ref, [&mstate](const JobOp &jobOp, long t_ref)
                                      { return mstate.jobOpLogs[jobOp.job][jobOp.op].startTime <= t_ref; });
    if (it == jobOps.end())
        return plan;
    const uint firstI = it - jobOps.begin();
    if (jobOps.size() - firstI < 2)
        return plan;
    auto op1I = randBtw(firstI, jobOps.size() - 2);
    auto op2I = randBtw(firstI, jobOps.size() - 2);
    if (op2I == op1I)
        return plan;
    if (jobOps[op1I].job == jobOps[op2I].job)
        return plan;
    std::swap(jobOps[op1I], jobOps[op2I]);

    // Switch the schedule mappings so it reflects the actual switch
    // auto jOpI1 = wsOps[op1I].op;
    // auto jOpI2 = wsOps[op2I].op;
    // WSSchPair &schMap1 = plan.jobs[wsOps[op1I].job].sch[jOpI1];
    // WSSchPair &schMap2 = plan.jobs[wsOps[op2I].job].sch[jOpI2];
    // std::swap(schMap1, schMap2);
    return plan;
}