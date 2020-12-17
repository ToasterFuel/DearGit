#include <algorithm>

class Utils
{
public:
    static int Clamp(int min, int max, int value)
    {
        return std::min(max, std::max(min, value));
    }
};