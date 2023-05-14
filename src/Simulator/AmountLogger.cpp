#include "AmountLogger.hpp"
#include <iostream>

size_t AmountLogs::calcLogI(long t_time) const
{
    return (t_time - start) / freq;
}

AmountLogs AmountLoggerSimple::calcAmountLogs(
    const Model &model,
    const ModelState &mstate,
    const Plan &plan,
    long t_ref,
    long t_freq) const
{
    AmountLogs amLogs;
    amLogs.freq = t_freq;
    amLogs.start = t_ref;

    // 1. Get how many completed operations we are talking about
    size_t sum = 0;
    std::vector<size_t> relevantOpIs(mstate.wsOpLogs.size());
    for (size_t wsI = 0; wsI < mstate.wsOpLogs.size(); ++wsI)
    {
        const auto &wsOpLogs = mstate.wsOpLogs[wsI];
        const auto &it = std::lower_bound(wsOpLogs.begin(), wsOpLogs.end(), t_ref,
                                          [&mstate](const WSOpLog &wsOpLog, long t_ref)
                                          { return wsOpLog.endTime < t_ref; });
        relevantOpIs[wsI] = it - wsOpLogs.begin();
        sum += wsOpLogs.size() - relevantOpIs[wsI];
    }

    // 2. Create an index for these operations, which orders them last to first completed
    std::vector<std::pair<size_t, size_t>> index(sum);
    for (size_t wsI = 0, counterI = 0; wsI < mstate.wsOpLogs.size(); ++wsI)
    {
        for (size_t opI = relevantOpIs[wsI]; opI < mstate.wsOpLogs[wsI].size(); ++opI)
        {
            index[counterI] = std::make_pair(wsI, opI);
            ++counterI;
        }
    }
    std::sort(index.begin(), index.end(), [&mstate](const auto &a, const auto &b)
              { return mstate.wsOpLogs[a.first][a.second].endTime < mstate.wsOpLogs[b.first][b.second].endTime; });

    // 3. Get the latest finishing time
    const auto &indexLast = index[index.size() - 1];
    long latestTime = mstate.wsOpLogs[indexLast.first][indexLast.second].endTime;

    // 4. Prepare the sizes of logs and set the last amount in logs
    const auto numOfLogsRequired = (latestTime - t_ref) / t_freq + 1;
    amLogs.matLogs.resize(numOfLogsRequired, std::vector<am_t>(model.materials.size()));
    for (size_t matI = 0; matI < model.materials.size(); ++matI)
    {
        amLogs.matLogs[numOfLogsRequired - 1][matI] = mstate.materialQuantities[matI];
    }

    amLogs.prodLogs.resize(numOfLogsRequired, std::vector<am_t>(model.products.size()));
    for (size_t prodI = 0; prodI < model.products.size(); ++prodI)
    {
        amLogs.prodLogs[numOfLogsRequired - 1][prodI] = mstate.productQuantities[prodI];
    }

    // 5. Iterate from the last and get the logs
    std::vector<long> backTrackProdIs(model.products.size(), numOfLogsRequired - 1);
    std::vector<long> backTrackMatIs(model.materials.size(), numOfLogsRequired - 1);
    for (auto it = index.end() - 1; it >= index.begin(); --it)
    {
        const auto &[wsI, opI] = (*it);
        const WSOpLog &wsOpLog = mstate.wsOpLogs[wsI][opI];
        const Job &job = plan.jobs[wsOpLog.job];
        const TechPlan &tp = model.techPlans[job.techPlan];
        const Operation &op = tp.operations[wsOpLog.op];
        const i_t prodI = job.product;
        const long logI = static_cast<long>(amLogs.calcLogI(wsOpLog.endTime));

        for (long i = backTrackProdIs[job.product] - 1; i >= logI; --i)
        {
            amLogs.prodLogs[i][prodI] = amLogs.prodLogs[i + 1][prodI];
        }
        if (wsOpLog.op == tp.operations.size() - 1)
            amLogs.prodLogs[logI][prodI]--;
        backTrackProdIs[job.product] = logI;

        for (const auto &mat : op.materials)
        {
            for (long i = backTrackMatIs[mat.thing] - 1; i >= logI; --i)
            {
                amLogs.matLogs[i][mat.thing] = amLogs.matLogs[i + 1][mat.thing];
            }
            amLogs.matLogs[logI][mat.thing] += mat.amount;
            backTrackMatIs[mat.thing] = logI;
        }
    }

    // 6. Fill in any remaining time at the beginning
    for (i_t prodI = 0; prodI < backTrackProdIs.size(); ++prodI)
    {
        for (long logI = 0; logI < backTrackProdIs[prodI]; ++logI)
        {
            amLogs.prodLogs[logI][prodI] = amLogs.prodLogs[backTrackProdIs[prodI]][prodI];
        }
    }

    for (i_t matI = 0; matI < backTrackMatIs.size(); ++matI)
    {
        for (long logI = 0; logI < backTrackMatIs[matI]; ++logI)
        {
            amLogs.matLogs[logI][matI] = amLogs.matLogs[backTrackMatIs[matI]][matI];
        }
    }

    return amLogs;
}