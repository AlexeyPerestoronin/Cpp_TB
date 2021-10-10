# Getting statistic example
It is the pseudo-code example of how to obtain statistic-data from an exchange platform  

```cpp
struct StatisticUnit {
    DateAndTime m_from;
    DataAndTime m_to;
    double m_start;
    double m_stop;
    double m_max;
    double m_min;
};

using DataAndTimePeriod = std::pair<DateAndTime, DateAndTime>;

// brief: returns in loop the next StatisticUnit of target period of time
// param: i_period - target period of time to statistic data request
// return: the next statistic-data
cppcoro::generator<StatisticUnit> StatisticUnits(const DataAndTimePeriod& i_period) {
    using FstreamUPtr = std::unique_ptr<std::fstream>;
    using StatisticUnitList = std::list<StatisticUnit>;
    
    auto GetSFileHandle = [&i_period]() -> FstreamUPtr {
        // TODO: there is the returned pointer to fstream-data must have a custom destructor 
        if (FstreamUPtr f_ptr{ LoadFileWithStatistic(i_period) }; f_ptr)
            return f_ptr;
        return CreateFileWithStatistic(i_period);
    };

    auto GetNextUnits = [f_ptr = GetSFileHandle()]() -> StatisticUnitList {
        return StatisticUnitList{}.emplace_back(GetNextUnitsFromFile(f_ptr));
    };

    for (auto statistic_units = StatisticUnitList{}.emplace_back(GetNextUnits());
        !statistic_units.empty();
        std::async(std::launcher::async, [&](){ statistic_units.emplace_back(GetNextUnits()); })) {
        co_yeld statistic_units.erace_front();
    }
}
```