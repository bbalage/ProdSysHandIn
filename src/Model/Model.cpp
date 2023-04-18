#include "Model.hpp"

Model createRandomModel()
{
    Model model;
    auto randBtw = [](uint min, uint max)
    { return min + rand() % (max - min); };

    // Generate workstations
    constexpr uint minNumOfWorkstations = 5;
    constexpr uint maxNumOfWorkstations = 15;
    const uint numWorkstations = randBtw(minNumOfWorkstations, maxNumOfWorkstations);
    constexpr uint workstationsPerTypes = 3;
    model.workstationTypes = numWorkstations / workstationsPerTypes;
    for (uint i = 0; i < numWorkstations; ++i)
    {
        model.workstations.push_back(Workstation{
            .type = i % model.workstationTypes});
    }

    // Generate products
    constexpr uint minNumOfProducts = 30;
    constexpr uint maxNumOfProducts = 60;
    constexpr uint numOfRawMaterials = 5;
    const uint numOfProducts = randBtw(minNumOfProducts, maxNumOfProducts);
    for (uint i = 0; i < numOfProducts; ++i)
    {
        model.products.push_back(Product{
            .amount = i < minNumOfProducts / numOfRawMaterials ? std::numeric_limits<size_t>::max() : 0U});
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
    for (uint prodI = numOfRawMaterials; prodI < model.products.size(); ++prodI)
    {
        const uint numOfOperations = randBtw(minNumOfOpsPerTechPlan, maxNumOfOpsPerTechPlan);
        Product &product = model.products[prodI];
        for (uint opI = 0; opI < numOfOperations; ++opI)
        {
            Operation op{
                .workstationType = randBtw(0, model.workstationTypes),
                .time = randBtw(minTimeForOperation, maxTimeForOperation)};
            const uint numberOfMaterialsNeeded = randBtw(0, prodI - 1);
            for (uint matI = 0; matI < numberOfMaterialsNeeded; ++matI)
            {
                op.materials.push_back(ThingAndAmount{
                    .thing = randBtw(0, prodI - 1),
                    .amount = randBtw(minNumOfMatPerOp, maxNumOfMatPerOp)});
            }
            product.ops.push_back(std::move(op));
        }
    }

    // Generate orders
    constexpr uint minNumOfProductsPerOrder = 1;
    constexpr uint maxNumOfProductsPerOrder = 10;
    constexpr uint minAmountPerProduct = 1;
    constexpr uint maxAmountPerProduct = 20;
    constexpr uint minPriority = 1;
    constexpr uint maxPriority = 100;
    constexpr uint minDueMultiplier = 10;
    constexpr uint maxDueMultiplier = 100;
    constexpr uint numOfOrders = 5;

    for (uint orderI = 0; orderI < numOfOrders; ++orderI)
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
                .thing = randBtw(0, numOfProducts),
                .amount = amount});
        }
        order.due = amountCounter * randBtw(minDueMultiplier, maxDueMultiplier);
        model.orders.push_back(std::move(order));
    }

    return model;
}