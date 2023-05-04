#ifndef PROD_SYS_HAND_IN_PLANNER_PLANNER_HPP
#define PROD_SYS_HAND_IN_PLANNER_PLANNER_HPP

#include "../Model/Model.hpp"

struct Plan
{
    // Matrix where rows denote the workstation and columns the job.
    // Mutation 3: Change order on the workstations
    std::vector<std::vector<JobOp>> sch_matrix;
    // Mutation 1: Change tech plans
    std::vector<i_t> techPlans;
    // Mutation 2: Change workstations
    std::vector<Job> jobs;
    bool invalid;
};

class Planner
{
public:
    Planner() {}
    ~Planner() {}
    Planner(const Planner &other) = delete;
    Planner &operator=(const Planner &other) = delete;
    Planner(Planner &&other) = default;
    Planner &operator=(Planner &&other) = default;

    virtual Plan plan(const Model &model, const ModelState &modelState, const std::vector<Order> &newOrders, long t_ref) const = 0;
};

class PlannerSimple : public Planner
{
public:
    PlannerSimple(long t_frozen) : m_t_frozen{t_frozen} {}

    Plan plan(const Model &model, const ModelState &modelState, const std::vector<Order> &newOrders, long t_ref) const override;

private:
    long m_t_frozen;
};

#endif