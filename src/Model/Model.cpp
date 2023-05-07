#include "Model.hpp"
#include <stdexcept>

Model generateRandomModel()
{
    Model model;

    // Generate workstations
    constexpr uint minNumOfWorkstations = 5;
    constexpr uint maxNumOfWorkstations = 15;
    const uint numWorkstations = randBtw(minNumOfWorkstations, maxNumOfWorkstations);
    constexpr uint workstationsPerTypes = 3;
    model.workstationTypes = numWorkstations / workstationsPerTypes;
    for (uint i = 0; i < numWorkstations; ++i)
    {
        wt_t wt_type = i % model.workstationTypes;
        if (model.workstationTypeMap.contains(wt_type))
            model.workstationTypeMap[wt_type].push_back(model.workstations.size());
        else
            model.workstationTypeMap[wt_type] = std::vector<i_t>(1, model.workstations.size());

        model.workstations.push_back(Workstation{
            .type = wt_type});
    }

    // Generate raw materials
    constexpr uint numOfRawMaterials = 5;
    for (uint i = 0; i < numOfRawMaterials; ++i)
    {
        model.materials.push_back(Material{});
    }

    // Generate products
    constexpr uint minNumOfProducts = 30;
    constexpr uint maxNumOfProducts = 60;
    const uint numOfProducts = randBtw(minNumOfProducts, maxNumOfProducts);
    for (uint i = 0; i < numOfProducts; ++i)
    {
        model.products.push_back(Product{});
    }

    // Generate tech plans
    constexpr uint minNumOfTechPlansPerProduct = 1;
    constexpr uint maxNumOfTechPlansPerProduct = 4;
    constexpr uint minNumOfOpsPerTechPlan = 1;
    constexpr uint maxNumOfOpsPerTechPlan = 5;
    constexpr uint minTimeForOperation = 1;
    constexpr uint maxTimeForOperation = 100;
    constexpr uint minNumOfMatPerOp = 1;
    constexpr uint maxNumOfMatPerOp = 20;

    const uint numberOfTechPlans = randBtw(minNumOfTechPlansPerProduct, maxNumOfTechPlansPerProduct);
    for (uint prodI = 0; prodI < model.products.size(); ++prodI)
    {
        const uint numOfTechPlans = randBtw(minNumOfTechPlansPerProduct, maxNumOfTechPlansPerProduct);
        for (uint techI = 0; techI < numOfTechPlans; ++techI)
        {
            TechPlan tp;
            Product &product = model.products[prodI];
            const uint numOfOperations = randBtw(minNumOfOpsPerTechPlan, maxNumOfOpsPerTechPlan);
            for (uint opI = 0; opI < numOfOperations; ++opI)
            {
                Operation op{
                    .workstationType = randBtw(0, model.workstationTypes),
                    .time = randBtw(minTimeForOperation, maxTimeForOperation)};
                const uint numberOfMaterialsNeeded = randBtw(1, numOfRawMaterials);
                for (uint matI = 0; matI < numberOfMaterialsNeeded; ++matI)
                {
                    op.materials.push_back(ThingAndAmount{
                        .thing = randBtw(0, numOfRawMaterials),
                        .amount = randBtw(minNumOfMatPerOp, maxNumOfMatPerOp)});
                }
                if (std::find(tp.operations.begin(), tp.operations.end(), op) != tp.operations.end())
                    continue;
                tp.operations.push_back(std::move(op));
            }
            product.techPlans.push_back(model.techPlans.size());
            model.techPlans.push_back(tp);
        }
    }

    return model;
}

std::vector<Order> generateOrders(uint numberOfOrders, uint numberOfProducts)
{
    // Generate orders
    constexpr uint minNumOfProductsPerOrder = 1;
    constexpr uint maxNumOfProductsPerOrder = 10;
    constexpr uint minAmountPerProduct = 1;
    constexpr uint maxAmountPerProduct = 20;
    constexpr uint minPriority = 1;
    constexpr uint maxPriority = 100;
    constexpr uint minDueMultiplier = 10;
    constexpr uint maxDueMultiplier = 100;
    std::vector<Order> orders;

    for (uint orderI = 0; orderI < numberOfOrders; ++orderI)
    {
        Order order{
            .priority = randBtw(minPriority, maxPriority)};
        const uint numOfProductsPerOrder = randBtw(minNumOfProductsPerOrder, maxNumOfProductsPerOrder);
        uint amountCounter = 0;
        for (uint prodI = 0; prodI < numOfProductsPerOrder; ++prodI)
        {
            auto amount = randBtw(minAmountPerProduct, maxAmountPerProduct);
            amountCounter += amount;
            order.products.push_back(ThingAndAmount{
                .thing = randBtw(0, numberOfProducts),
                .amount = amount});
        }
        order.due = amountCounter * randBtw(minDueMultiplier, maxDueMultiplier);
        orders.push_back(std::move(order));
    }

    return orders;
}

ModelState ModelState::copyBeforeTime(long t_time) const
{
    ModelState new_mstate{
        .materials = materials,
        .orderLogs = std::vector<OrderLog>(orderLogs.size(), OrderLog{.completionTime = 0,
                                                                      .lateness = 0}),
        .nextJobsPerWS = std::vector<i_t>(wsOpLogs.size(), 0)};
    new_mstate.jobOpLogs.resize(jobOpLogs.size());
    for (i_t jobI = 0; jobI < jobOpLogs.size(); ++jobI)
        new_mstate.jobOpLogs[jobI].resize(
            jobOpLogs[jobI].size(), JobOpLog{
                                        .startTime = 0,
                                        .endTime = 0,
                                        .finished = false});
    for (size_t jI = 0; jI < jobOpLogs.size(); ++jI)
    {
        const auto &jobLogs = jobOpLogs[jI];
        for (size_t opI = 0; opI < jobLogs.size(); ++opI)
        {
            const JobOpLog &jobOpLog = jobLogs[opI];
            if (jobOpLog.finished && jobOpLog.startTime <= t_time)
            {
                new_mstate.jobOpLogs[jI][opI] = jobOpLog;
                new_mstate.jobOpLogs[jI][opI].finished = true;
            }
        }
    }

    new_mstate.wsOpLogs.resize(wsOpLogs.size());
    for (size_t wsI = 0; wsI < wsOpLogs.size(); ++wsI)
    {
        const auto &wsLogs = wsOpLogs[wsI];
        for (const WSOpLog &wsOpLog : wsLogs)
        {
            if (wsOpLog.finished && wsOpLog.startTime <= t_time)
            {
                new_mstate.wsOpLogs[wsI].push_back(wsOpLog);
                new_mstate.nextJobsPerWS[wsI]++;
            }
        }
    }
    return new_mstate;
}