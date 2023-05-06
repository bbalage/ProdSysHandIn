#include "Simulator.hpp"
#include <stdexcept>

// TODO: This function has got pretty crazy. Refactor!
ModelState SimulatorSimple::simulate(const Model &model,
                                     const ModelState &in_modelState,
                                     const Plan &plan,
                                     long t_ref)
{
    const auto &sch = plan.sch_matrix;
    auto &jobs = plan.jobs;

    ModelState modelState = in_modelState.copyBeforeTime(t_ref);

    std::vector<i_t> nextJobs(sch.size(), 0);
    while (true)
    {
        bool couldLaunchAtLeastOneJob = false;
        bool allJobsCompleted = true;
        for (size_t wsI = 0; wsI < sch.size(); ++wsI)
        {
            // If there is not any queued jobs for this workstation, continue with next one.
            if (nextJobs[wsI] >= sch[wsI].size())
                continue;
            couldLaunchAtLeastOneJob = true;
            nextJobs[wsI]++;
            allJobsCompleted = false;

            const auto &jobOp = sch[wsI][nextJobs[wsI]];
            const Job &job = jobs[jobOp.job];
            JobOpLog &opLog = modelState.jobOpLogs[jobOp.job][jobOp.op];
            if (opLog.finished)
                continue;

            const auto &op = model.techPlans[job.techPlan].operations[jobOp.op];
            auto &wsops = modelState.wsOpLogs[wsI];
            long ref = wsops.size() == 0 ? t_ref : wsops[wsops.size() - 1].endTime;

            // Job operation is launchable if all its previous operations had been finished
            // If the workstation can launch the job, then launch it
            // If not, go to the next workstation
            if (jobOp.op == 0)
            {
                opLog.startTime = ref;
            }
            else if (modelState.jobOpLogs[jobOp.job][jobOp.op - 1].finished)
            {
                opLog.startTime = std::max(modelState.jobOpLogs[jobOp.job][jobOp.op - 1].endTime, ref);
            }
            else
                continue;
            opLog.endTime = opLog.startTime + op.time;
            opLog.finished = true;
            wsops.push_back(WSOpLog{
                .startTime = opLog.startTime,
                .endTime = opLog.endTime,
                .job = jobOp.job,
                .op = jobOp.op});

            // Update the order's completion time. If one of the order's operations finish after the
            // current completion time, then the order's completion time should be set to the operation's
            // completion time.
            OrderLog &orderLog = modelState.orderLogs[job.order];
            orderLog.completionTime = std::max(orderLog.completionTime, opLog.endTime);
        }
        if (allJobsCompleted)
        {
            break;
        }
        if (!couldLaunchAtLeastOneJob)
        {
            throw std::invalid_argument("Plan cannot be executed.");
        }
    }

    for (i_t orI = 0; orI < modelState.orderLogs.size(); ++orI)
    {
        OrderLog &orderLog = modelState.orderLogs[orI];
        const Order &order = plan.orders[orI];
        orderLog.lateness = orderLog.completionTime - order.due;
    }
    return modelState;
}