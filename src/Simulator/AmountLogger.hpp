#ifndef PROD_SYS_HAND_IN_EVALUATOR_AMOUNTLOGGER_HPP
#define PROD_SYS_HAND_IN_EVALUATOR_AMOUNTLOGGER_HPP

#include "../Model/Model.hpp"

struct AmountLogs
{
    long freq;
    std::vector<std::vector<am_t>> matLogs;
    std::vector<std::vector<am_t>> prodLogs;

    static AmountLogs createFromModelState(const Model &model, const ModelState &mstate, long t_ref, long t_freq)
    {
        AmountLogs amLogs;
        // TODO: Continue here...

        const auto &wsOpLogs = mstate.wsOpLogs;
        // Max search for the latest finishing time of an operation in the simulation
        long latestTime = 0;
        std::vector<size_t> wsIs(model.workstations.size());
        for (size_t wsI = 0; wsI < wsOpLogs.size(); ++wsI)
        {
            const auto &lastI = wsOpLogs[wsI].size() - 1;
            latestTime = std::max(latestTime, mstate.wsOpLogs[wsI][lastI].endTime);
            wsIs[wsI] = lastI;
        }

        amLogs.matLogs.resize(model.materials.size());
        for (size_t matI = 0; matI < amLogs.matLogs.size(); ++matI)
        {
            amLogs.matLogs[matI].resize((latestTime - t_ref) / t_freq + 1, mstate.materialQuantities[matI]);
        }

        amLogs.prodLogs.resize(model.products.size());
        for (size_t prodI = 0; prodI < amLogs.prodLogs.size(); ++prodI)
        {
            amLogs.prodLogs[prodI].resize((latestTime - t_ref) / t_freq + 1, mstate.productQuantities[prodI]);
        }

        for (size_t wsI = 0; wsI < wsOpLogs[wsI].size(); ++wsI)
        {
            for (size_t opI = wsOpLogs[wsI].size() - 1; opI != std::numeric_limits<size_t>::max(); --opI)
            {
            }
        }
    }
};

// class AmountLogger
// {
// public:
//     AmountLogger() {}
//     ~AmountLogger() {}
//     AmountLogger(const AmountLogger &other) = delete;
//     AmountLogger &operator=(const AmountLogger &other) = delete;
//     AmountLogger(AmountLogger &&other) = default;
//     AmountLogger &operator=(AmountLogger &&other) = default;

//     virtual AmountLogs initLogs(const Model &model, const ModelState &mstate) = 0;
//     virtual AmountLogs logEvent(const Model &model, const ModelState &mstate, const Operation &op, long t_event) = 0;
//     virtual AmountLogs getLogs() const = 0;
// };

// class AmountLoggerSimple : public AmountLogger
// {
// public:
//     AmountLoggerSimple(long amLogFreq = 10) : m_amLogFreq{amLogFreq} {}

//     AmountLogs initLogs(const Model &model, const ModelState &mstate) override;
//     AmountLogs logEvent(const Model &model, const ModelState &mstate, const Operation &op, long t_event) override;
//     AmountLogs getLogs() const override;

// private:
//     long m_amLogFreq;
//     AmountLogs m_amLogs;
// };

#endif