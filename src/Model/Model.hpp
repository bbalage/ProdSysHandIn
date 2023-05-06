#ifndef PROD_SYS_HAND_IN_MODEL_MODEL_HPP
#define PROD_SYS_HAND_IN_MODEL_MODEL_HPP

#include <vector>
#include <map>
#include "../util/util.hpp"

// Index of any vector in the model
typedef size_t i_t;
// Workstation type
typedef uint wt_t;

struct ThingAndAmount
{
    i_t thing;
    uint amount;

    bool operator==(const ThingAndAmount &other) const { return thing == other.thing && amount == other.amount; }
};

struct Material
{
    // Name for example...
};

struct Operation
{
    wt_t workstationType;
    // <Product id, amount> pairs
    std::vector<ThingAndAmount> materials;
    // Time it takes to complete the operation
    long time;

    bool operator==(const Operation &other) const
    {
        return workstationType == other.workstationType && materials == other.materials;
    }
};

struct TechPlan
{
    std::vector<Operation> operations;
};

struct Product
{
    // Operations that if completed produce this product as a result.
    std::vector<i_t> techPlans;
};

struct WSOpLog
{
    long startTime;
    long endTime;
    i_t job;
    i_t op;
};

struct Workstation
{
    uint type;
};

struct OrderLog
{
    // std::vector<ThingAndAmount> completionAmount; // TODO: something like this
    long completionTime;
    long lateness;
};

struct Order
{
    std::vector<ThingAndAmount> products;
    uint priority;
    long due;
};

struct JobOpLog
{
    long startTime;
    long endTime;
    bool finished;
};

struct WSSchPair
{
    i_t ws;
    i_t sch;
};

struct Job
{
    i_t order;
    i_t product;
    i_t techPlan;
    // Mapping to the place of operations in the schedule matrix.
    // std::vector<WSSchPair> sch;
};

struct JobOp
{
    i_t job;
    i_t op;
};

struct Model
{
    wt_t workstationTypes;
    std::vector<Workstation> workstations;
    std::vector<Material> materials;
    std::vector<Product> products;
    std::vector<TechPlan> techPlans;

    std::map<wt_t, std::vector<i_t>> workstationTypeMap;
};

struct Plan
{
    // Matrix where rows denote the workstation and columns the job.
    std::vector<std::vector<JobOp>> sch_matrix;
    // Mutation 1: Change tech plans
    // std::vector<i_t> techPlans;
    // Mutation 2: Change workstations
    // Mutation 3: Change order on the workstations
    std::vector<Order> orders;
    std::vector<Job> jobs;
    std::vector<std::pair<i_t, i_t>> jobsFromToPerOrder;
    bool invalid;
};

struct ModelState
{
    std::vector<std::vector<WSOpLog>> wsOpLogs;
    std::vector<std::vector<JobOpLog>> jobOpLogs;
    std::vector<ThingAndAmount> materials;
    std::vector<OrderLog> orderLogs;

    ModelState copyBeforeTime(long t_time) const;
};

Model generateRandomModel();
std::vector<Order> generateOrders(uint numberOfOrders, uint numberOfProducts);

#endif