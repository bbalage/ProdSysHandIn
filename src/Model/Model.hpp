#ifndef PROD_SYS_HAND_IN_MODEL_MODEL_HPP
#define PROD_SYS_HAND_IN_MODEL_MODEL_HPP

#include <vector>

typedef size_t i_t;

struct Product
{
    std::vector<i_t> techPlans;
};

struct Workstation
{
    int type;
};

struct Operation
{
    int workstationType;
    // <Product id, amount> pairs
    std::vector<std::pair<i_t, uint>> materials;
    // Time it takes to complete the operation
    ulong time;
};

struct TechPlan
{
    std::vector<std::vector<i_t>> operations;
};

struct Order
{
    i_t product;
    int priority;
    ulong dueDelivery;
};

struct Model
{
    std::vector<Workstation> workstations;
    std::vector<Product> products;
    std::vector<TechPlan> techPlans;
    std::vector<Order> orders;
};

#endif