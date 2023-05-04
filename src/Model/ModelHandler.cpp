#include "ModelHandler.hpp"

void ModelHandler::addOrders(std::vector<Order> orders)
{
    // Add new orders
    ModelState mstate = m_mstate;
    mstate.orders.insert(mstate.orders.end(), orders.begin(), orders.end());

    // Create plan
    Plan plan = m_planner.plan(m_model, mstate, m_mstate.orders.size(), m_t_cur);

    // Optimize the plan
    ModelState sim_mstate = m_simulator.simulate(m_model, mstate, plan, m_t_cur);
    Eval eval = analyse(m_model, sim_mstate);
    std::cout << "Before optimization:" << std::endl;
    print_eval(eval);

    m_optimizer.optimize(m_model, mstate, plan, mstate, eval, m_t_cur);

    std::cout << "After optimization:" << std::endl;
    print_eval(eval);

    // Accept the new plan and state
    m_mstate = mstate;
    m_mstate.plan = plan;
}

void ModelHandler::advanceTime(long t_adv)
{
    auto n_cur = m_t_cur + t_adv;
}