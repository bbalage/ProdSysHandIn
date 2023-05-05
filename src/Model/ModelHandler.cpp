#include "ModelHandler.hpp"

void ModelHandler::addOrders(std::vector<Order> orders)
{
    // Add new orders
    const ModelState &mstate = m_mstate_current;

    // Create plan
    Plan plan = m_planner.plan(m_model, mstate, m_plan, orders, m_t_cur);

    // Optimize the plan
    ModelState sim_mstate = m_simulator.simulate(m_model, mstate, plan, m_t_cur);
    Eval eval = analyse(m_model, sim_mstate);
    std::cout << "Before optimization:" << std::endl;
    print_eval(eval);

    ModelState out_mstate;
    m_optimizer.optimize(m_model, mstate, plan, out_mstate, eval, m_t_cur);

    std::cout << "After optimization:" << std::endl;
    print_eval(eval);

    // Accept the new plan and state
    m_mstate_predicted = out_mstate;
    m_plan = plan;
}

void ModelHandler::advanceTime(long t_adv)
{
    auto n_cur = m_t_cur + t_adv;
    std::vector<i_t> completedOrders;
}