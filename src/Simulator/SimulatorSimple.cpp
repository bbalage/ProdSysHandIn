#include "Simulator.hpp"

void SimulatorSimple::simulate(Model &model, Planner &planner, double t_ref)
{
    Plan plan = planner.plan(model);
    const auto &sch = plan.sch_matrix;
    // const auto &dest = m_model->destination;
    // for (uint i = 0; i < sch.size(); ++i)
    // {
    //     auto &agent = m_model->agents[i];
    //     agent.distTravelled = 0;
    //     agent.freeAt = m_t_ref;
    //     for (uint j = 0; j < sch[i].size(); ++j)
    //     {
    //         auto &job = m_model->cargos[sch[i][j]];
    //         const auto dist = eucl_dist(agent.pos, job.pos);
    //         const auto dist_dest = eucl_dist(job.pos, dest.pos);
    //         const auto timeToReach = time_taken(dist, agent.speed);
    //         const auto timeToDeliver = time_taken(dist_dest, agent.speed);

    //         job.pickupTime = agent.freeAt + timeToReach;
    //         agent.distTravelled += dist + dist_dest;
    //         job.deliveryTime = job.pickupTime + timeToDeliver;
    //         agent.pos = dest.pos;
    //         agent.freeAt = job.deliveryTime;
    //     }
    // }
    return;
}