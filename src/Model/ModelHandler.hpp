#ifndef PROD_SYS_HAND_IN_MODEL_MODELHANDLER_HPP
#define PROD_SYS_HAND_IN_MODEL_MODELHANDLER_HPP

#include <memory>

#include "../Planner/Planner.hpp"
#include "../Simulator/Simulator.hpp"
#include "../Evaluator/Evaluator.hpp"
#include "../util/output.hpp"
#include "../Optimizer/Optimizer.hpp"
#include "Model.hpp"

class ModelHandler
{
public:
    ModelHandler(
        Planner &planner,
        Simulator &simulator,
        Optimizer &optimizer,
        Model model,
        long t_ref = 0L) : m_planner{planner},
                           m_simulator{simulator},
                           m_optimizer{optimizer},
                           m_model{model},
                           m_t_cur{t_ref},
                           m_mstate_current{.wsOpLogs = std::vector<std::vector<WSOpLog>>(model.workstations.size()),
                                            .jobOpLogs = std::vector<std::vector<JobOpLog>>(0),
                                            .nextJobsPerWS = std::vector<i_t>(model.workstations.size(), 0)},
                           m_mstate_predicted{.wsOpLogs = std::vector<std::vector<WSOpLog>>(model.workstations.size()),
                                              .jobOpLogs = std::vector<std::vector<JobOpLog>>(0),
                                              .nextJobsPerWS = std::vector<i_t>(model.workstations.size(), 0)}
    {
        m_plan.sch_matrix.resize(model.workstations.size(), std::vector<JobOp>());
    }
    ~ModelHandler() {}
    ModelHandler(const ModelHandler &other) = delete;
    ModelHandler &operator=(const ModelHandler &other) = delete;
    ModelHandler(ModelHandler &&other) = default;
    ModelHandler &operator=(ModelHandler &&other) = default;

    void addOrders(std::vector<Order> orders);
    void advanceTime(long t_adv);

    const auto &t_cur() const { return m_t_cur; }
    const auto &model() const { return m_model; }
    const auto &mstate_current() const { return m_mstate_current; }
    const auto &mstate_predicted() const { return m_mstate_predicted; }

private:
    Planner &m_planner;
    Simulator &m_simulator;
    Optimizer &m_optimizer;

    Model m_model;
    long m_t_cur;

    ModelState m_mstate_current;
    ModelState m_mstate_predicted;
    Plan m_plan{.invalid = false};
};

#endif