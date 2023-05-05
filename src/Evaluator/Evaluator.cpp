#include "Evaluator.hpp"

Eval analyse(const Model &model, const ModelState &modelState)
{
    Eval eval{
        .Cmax = std::numeric_limits<long>::min(),
        .Csum = 0,
        .Lmax = std::numeric_limits<long>::min(),
        .Tsum = 0,
        .Usum = 0};
    for (const OrderLog &orderLog : modelState.orderLogs)
    {
        eval.Cmax = std::max(eval.Cmax, orderLog.completionTime);
        eval.Csum += orderLog.completionTime;
        eval.Lmax = std::max(eval.Lmax, orderLog.lateness);
        auto T = std::max(orderLog.lateness, 0L);
        eval.Tsum += T;
        eval.Usum += T > 0 ? 1 : 0;
    }

    return eval;
}

double Evaluator::targetF(const Eval &x, const Eval &y) const
{
    auto f = [](double x, double y, double w)
    {
        double D;
        if (std::max(x, y) == 0)
            D = 0;
        else
            D = (y - x) / std::max(x, y);
        return w * D;
    };
    double F_ret = 0.0;
    F_ret += f(static_cast<double>(x.Cmax), static_cast<double>(y.Cmax), static_cast<double>(m_evalWeights.wCmax));
    F_ret += f(static_cast<double>(x.Csum), static_cast<double>(y.Csum), static_cast<double>(m_evalWeights.wCsum));
    F_ret += f(static_cast<double>(x.Lmax), static_cast<double>(y.Lmax), static_cast<double>(m_evalWeights.wLmax));
    F_ret += f(static_cast<double>(x.Tsum), static_cast<double>(y.Tsum), static_cast<double>(m_evalWeights.wTsum));
    F_ret += f(static_cast<double>(x.Usum), static_cast<double>(y.Usum), static_cast<double>(m_evalWeights.wUsum));

    return F_ret;
}