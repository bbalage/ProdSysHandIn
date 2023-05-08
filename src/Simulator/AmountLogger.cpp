#include "AmountLogger.hpp"

size_t AmountLogs::calcLogI(long t_time) const
{
    return (t_time - start) / freq;
}

AmountLogs AmountLogger::calcAmountLogs(
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
    for (size_t wsI = 0; wsI < mstate.wsOpLogs.size(); wsI)
    {
        const auto &wsOpLogs = mstate.wsOpLogs[wsI];
        const auto &it = std::lower_bound(wsOpLogs.begin(), wsOpLogs.end(), t_ref,
                                          [&mstate](const WSOpLog &wsOpLog, long t_ref)
                                          { return wsOpLog.endTime < t_ref; });
        relevantOpIs[wsI] = wsOpLogs.size() - (wsOpLogs.end() - it);
        sum += relevantOpIs[wsI];
    }

    // 2. Create an index for these operations, which orders them last to first completed
    std::vector<std::pair<size_t, size_t>> index(sum);
    for (size_t wsI = 0, counterI = 0; wsI < mstate.wsOpLogs.size(); ++wsI)
    {
        for (size_t opI = relevantOpIs[wsI]; opI < mstate.wsOpLogs[wsI].size(); ++opI)
        {
            index[counterI] = std::make_pair(wsI, opI);
        }
    }
    std::sort(index.begin(), index.end(), [&mstate](const auto &a, const auto &b)
              { return mstate.wsOpLogs[a.first][a.second].endTime < mstate.wsOpLogs[b.first][b.second].endTime; });

    // 3. Get the latest finishing time
    const auto &indexLast = index[index.size() - 1];
    long latestTime = mstate.wsOpLogs[indexLast.first][indexLast.second].endTime;

    // 4. Prepare the sizes of logs and set the last amount in logs
    const auto numOfLogsRequired = (latestTime - t_ref) / t_freq + 1;
    amLogs.matLogs.resize(model.materials.size());
    for (size_t matI = 0; matI < amLogs.matLogs.size(); ++matI)
    {
        amLogs.matLogs[matI].resize(numOfLogsRequired);
        amLogs.matLogs[matI][numOfLogsRequired - 1] = mstate.materialQuantities[matI];
    }

    amLogs.prodLogs.resize(model.products.size());
    for (size_t prodI = 0; prodI < amLogs.prodLogs.size(); ++prodI)
    {
        amLogs.prodLogs[prodI].resize(numOfLogsRequired);
        amLogs.prodLogs[prodI][numOfLogsRequired - 1] = mstate.productQuantities[prodI];
    }

    // 5. Iterate from the last and get the logs
    std::vector<size_t> backTrackProdIs(model.products.size(), numOfLogsRequired - 1);
    std::vector<size_t> backTrackMatIs(model.materials.size(), numOfLogsRequired - 1);
    for (auto it = index.end() - 1; it >= index.begin(); --it)
    {
        const auto &[wsI, opI] = (*it);
        const WSOpLog &wsOpLog = mstate.wsOpLogs[wsI][opI];
        const Job &job = plan.jobs[wsOpLog.job];
        const Operation &op = model.techPlans[job.techPlan].operations[wsOpLog.op];
        const i_t prodI = job.product;
        const size_t logI = amLogs.calcLogI(wsOpLog.endTime);

        for (size_t i = backTrackProdIs[job.product] - 1; i > logI; --i)
        {
            amLogs.prodLogs[i][prodI] = amLogs.prodLogs[i + 1][prodI];
        }
        amLogs.prodLogs[logI][prodI]--;

        for (const auto &mat : op.materials)
        {
            for (size_t i = backTrackMatIs[mat.thing] - 1; i > logI; --i)
            {
                amLogs.matLogs[i][mat.thing] = amLogs.matLogs[i + 1][mat.thing];
            }
            amLogs.matLogs[logI][mat.thing] += mat.amount;
        }
    }

    return amLogs;
}