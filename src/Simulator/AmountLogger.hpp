#ifndef PROD_SYS_HAND_IN_EVALUATOR_AMOUNTLOGGER_HPP
#define PROD_SYS_HAND_IN_EVALUATOR_AMOUNTLOGGER_HPP

#include "../Model/Model.hpp"

struct AmountLogs
{
    long freq;
    long start;
    std::vector<std::vector<am_t>> matLogs;
    std::vector<std::vector<am_t>> prodLogs;

    // TODO: Properly set intervals: [) type; this is not that type yet
    size_t calcLogI(long t_time) const;
};

class AmountLogger
{
public:
    AmountLogger() {}
    virtual ~AmountLogger() = default;
    AmountLogger(const AmountLogger &other) = delete;
    AmountLogger &operator=(const AmountLogger &other) = delete;
    AmountLogger(AmountLogger &&other) = default;
    AmountLogger &operator=(AmountLogger &&other) = default;

    virtual AmountLogs calcAmountLogs(
        const Model &model,
        const ModelState &mstate,
        const Plan &plan,
        long t_ref,
        long t_freq) const = 0;
};

class AmountLoggerSimple : public AmountLogger
{
public:
    AmountLoggerSimple() {}
    ~AmountLoggerSimple() {}

    AmountLogs calcAmountLogs(
        const Model &model,
        const ModelState &mstate,
        const Plan &plan,
        long t_ref,
        long t_freq) const override;
};

#endif