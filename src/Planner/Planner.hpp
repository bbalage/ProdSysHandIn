#ifndef PROD_SYS_HAND_IN_PLANNER_PLANNER_HPP
#define PROD_SYS_HAND_IN_PLANNER_PLANNER_HPP

#include "../Model/Model.hpp"

struct Plan
{
    // Matrix where rows denote the job and columns the workstation.
    std::vector<std::vector<i_t>> sch_matrix;
    std::vector<Job> jobs;
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

    virtual Plan plan(const Model &model) const = 0;
};

class PlannerSimple : public Planner
{
public:
    Plan plan(const Model &model) const override;
};

#endif