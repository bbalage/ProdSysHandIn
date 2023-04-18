#ifndef PROD_SYS_HAND_IN_MODEL_MODEL_HPP
#define PROD_SYS_HAND_IN_MODEL_MODEL_HPP

#include <vector>
#include <random>

typedef size_t i_t;

struct ThingAndAmount
{
    i_t thing;
    uint amount;

    bool operator==(const ThingAndAmount &other) const { return thing == other.thing && amount == other.amount; }
};

struct Operation
{
    uint workstationType;
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
    std::vector<Operation> ops;
    // std::vector<TechPlan> techPlans;
    size_t amount;
};

struct Workstation
{
    uint type;
};

struct Order
{
    std::vector<ThingAndAmount> products;
    int priority;
    ulong due;
};

struct Model
{
    std::vector<Workstation> workstations;
    std::vector<Product> products;
    std::vector<Order> orders;
    uint workstationTypes;
};

Model createRandomModel();

#endif