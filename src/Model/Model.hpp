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
    uint amount;
};

struct Operation
{
    wt_t workstationType;
    // <Product id, amount> pairs
    std::vector<ThingAndAmount> materials;
    // Time it takes to complete the operation
    ulong time;

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

struct ProductInstance
{
    i_t product;
};

struct Workstation
{
    uint type;
};

struct Order
{
    std::vector<ThingAndAmount> products;
    int priority;
    long due;

    std::vector<ThingAndAmount> completionAmount;
    long completionTime;
    long lateness;
};

struct Job
{
    i_t order;
    i_t product;
    i_t techPlan;
};

struct Model
{
    wt_t workstationTypes;
    std::vector<Workstation> workstations;
    std::vector<Material> materials;
    std::vector<Product> products;
    std::vector<TechPlan> techPlans;
    std::vector<Order> orders;
    std::vector<ProductInstance> productInstances;

    std::map<wt_t, std::vector<i_t>> workstationTypeMap;
};

Model createRandomModel();

#endif