#include "AmountLogger.hpp"

AmountLogs AmountLoggerSimple::initLogs(const Model &model, const ModelState &mstate)
{
    m_amLogs = AmountLogs{
        .freq = m_amLogFreq,
        .matLogs = std::vector<std::vector<am_t>>(model.materials.size()),
        .prodLogs = std::vector<std::vector<am_t>>(model.products.size()),
    };
}

AmountLogs AmountLoggerSimple::logEvent(const Model &model,
                                        const ModelState &mstate,
                                        const Operation &op,
                                        long t_event)
{
}

AmountLogs AmountLoggerSimple::getLogs() const
{
    return m_amLogs;
}

{
    AmountLogs amLogs;

    const auto &wsOpLogs = mstate.wsOpLogs;
    // Max search for the latest finishing time of an operation in the simulation
    long latestTime = 0;
    for (size_t wsI = 0; wsI < wsOpLogs.size(); ++wsI)
    {
        const auto &lastI = wsOpLogs[wsI].size() - 1;
        latestTime = std::max(latestTime, mstate.wsOpLogs[wsI][lastI].endTime);
    }

    amLogs.matLogs.resize(model.materials.size());
    for (size_t matI = 0; matI < amLogs.matLogs.size(); ++matI)
    {
        amLogs.matLogs[matI].resize((latestTime - t_ref) / freq + 1, mstate.materialQuantities[matI]);
    }

    amLogs.prodLogs.resize(model.products.size());
    for (size_t prodI = 0; prodI < amLogs.prodLogs.size(); ++prodI)
    {
        amLogs.prodLogs[prodI].resize((latestTime - t_ref) / freq + 1, mstate.productQuantities[prodI]);
    }

    for (size_t wsI = 0; wsI < wsOpLogs[wsI].size(); ++wsI)
    {
        for (size_t opI = wsOpLogs[wsI].size() - 1; opI != std::numeric_limits<size_t>::max(); --opI)
        {
        }
    }
}