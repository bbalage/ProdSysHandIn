#include "ModelHandler.hpp"

void ModelHandler::addOrders(std::vector<Order> orders)
{
    // Create plan
    Plan plan = m_planner.plan(m_model, m_mstate, orders, m_t_cur);
    // Optimize
    m_mstate.orders = orders;

    // ModelState n_mstate = m_simulator.simulate(m_model, m_mstate, plan);
}

void ModelHandler::advanceTime(long t_adv)
{
}