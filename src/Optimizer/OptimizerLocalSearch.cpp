#include "Optimizer.hpp"

void OptimizerLocalSearch::optimize(const Model &model,
                                    const ModelState &in_modelState,
                                    Plan &plan,
                                    long t_ref) const
{
    ModelState modelState = in_modelState;
    Plan plan_0 = plan;    // start base
    Plan plan_best = plan; // best solution of the search
    Plan plan_actual;      // actual
    Plan plan_extension;   // best neighbor

    Eval e_best;
    Eval e_actual;
    Eval e_extension;

    m_sim->simulate(model, modelState, plan, t_ref);
    e_best = analyse(model, modelState);

    for (uint step = 1; step <= m_steps; ++step)
    {
        for (uint loop = 1; loop <= m_loops; ++loop)
        {
            // neighbor(plan_0);
            m_sim->simulate(model, modelState, plan_actual, t_ref);
            e_actual = analyse(model, modelState);
            if (loop == 1)
            {
                plan_extension = plan_actual;
                e_extension = e_actual;
            }
            else if (m_eval->targetF(e_extension, e_actual) < 0.0)
            {
                plan_extension = plan_actual;
                e_extension = e_actual;
            }
        }
        plan_0 = plan_extension;
        if (m_eval->targetF(e_best, e_extension) < 0.0)
        {
            plan_best = plan_extension;
            e_best = e_extension;
        }
    }

    plan = plan_best;
}