
enum StreetName {
    preflop = 0,
    flop    = 1,
    turn    = 2,
    river   = 3,
    showdown= 4
};

std::ostream& operator<<(std::ostream& os, const StreetName& street_name)
{
    switch(street_name)
    {
        case preflop   : os << "PREFLOP";    break;
        case flop      : os << "FLOP";       break;
        case turn      : os << "TURN";       break;
        case river     : os << "RIVER";      break;
        case showdown  : os << "SHOWDOWN";   break;
        default        : os << "Unknown street" << street_name ;
    }
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const vector<T>& v)
{
    for (auto& i: v)
        os << i << " ";
    return os;
}
