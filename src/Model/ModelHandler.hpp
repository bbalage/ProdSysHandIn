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
        Optimizer &optimizer,
        Model model,
        long t_ref = 0L) : m_planner{planner},
                           m_optimizer{optimizer},
                           m_model{model},
                           m_t_cur{t_ref},
                           m_mstate{.wsOpLogs = std::vector<std::vector<WSOpLog>>(model.workstations.size())}
    {
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
    const auto &mstate() const { return m_mstate; }

private:
    Planner &m_planner;
    Optimizer &m_optimizer;

    Model m_model;
    long m_t_cur;

    ModelState m_mstate;
};

#endif