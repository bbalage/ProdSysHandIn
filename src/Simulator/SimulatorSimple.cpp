#include "Simulator.hpp"

void SimulatorSimple::simulate(const Model &model, ModelState &modelState, Plan &plan, long t_ref)
{
    const auto &sch = plan.sch_matrix;
    auto &jobs = plan.jobs;
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
            allJobsCompleted = false;
            const auto &jobOp = sch[wsI][nextJobs[wsI]];
            Job &job = jobs[jobOp.job];
            const auto &op = model.techPlans[job.techPlan].operations[jobOp.op];
            auto &wsops = modelState.wsOpLogs[wsI];
            long ref = wsops.size() == 0 ? t_ref : wsops[wsops.size() - 1].endTime;
            auto &opLog = job.opLogs[jobOp.op];

            // Job operation is launchable if all its previous operations had been finished
            // If the workstation can launch the job, then launch it
            // If not, go to the next workstation
            if (jobOp.op == 0)
            {
                opLog.startTime = ref;
            }
            else if (job.opLogs[jobOp.op - 1].finished)
            {
                opLog.startTime = std::max(job.opLogs[jobOp.op - 1].endTime, ref);
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
            Order &order = modelState.orders[job.order];
            order.completionTime = std::max(order.completionTime, opLog.endTime);

            couldLaunchAtLeastOneJob = true;
            nextJobs[wsI]++;
        }
        if (allJobsCompleted)
        {
            break;
        }
        if (!couldLaunchAtLeastOneJob)
        {
            plan.invalid = true;
            break;
        }
    }
    if (plan.invalid)
        return;

    for (Order &order : modelState.orders)
    {
        order.lateness = order.completionTime - order.due;
    }
    return;
}