#ifndef PROD_SYS_HAND_IN_PLANNER_PLANNER_HPP
#define PROD_SYS_HAND_IN_PLANNER_PLANNER_HPP

#include "../Model/Model.hpp"

class Planner
{
public:
    Planner() {}
    virtual ~Planner() {}
    Planner(const Planner &other) = delete;
    Planner &operator=(const Planner &other) = delete;
    Planner(Planner &&other) = default;
    Planner &operator=(Planner &&other) = default;

    virtual Plan plan(const Model &model,
                      const Plan &oldPlan,
                      const std::vector<Order> &newOrders,
                      ModelState &modelState,
                      long t_ref) const = 0;
};

class PlannerSimple : public Planner
{
public:
    PlannerSimple(long t_frozen) : m_t_frozen{t_frozen} {}

    Plan plan(const Model &model,
              const Plan &oldPlan,
              const std::vector<Order> &newOrders,
              ModelState &modelState,
              long t_ref) const override;

private:
    long m_t_frozen;
};

#endif