/*
   "ITMOx: I2CPx How to win coding competitions: secrets of champions"
   Week 2 > 2nd Week Problems > Queue with Minimum
   https://goo.gl/0VwqGs
*/

#include <assert.h>
#include <stdlib.h>
#include <string>
#include <deque>
#include <set>
#include <fstream>

//  http://stackoverflow.com/a/6154614/1032277
enum STR2INT_ERROR { CA_STR2INT_SUCCESS, CA_STR2INT_OVERFLOW, CA_STR2INT_UNDERFLOW, CA_STR2INT_INCONVERTIBLE };
static inline
STR2INT_ERROR str2long(long &i, char const *s, int base = 0)
{
    errno = 0;
    char *end = nullptr;
    const long l = strtol(s, &end, base);
    if ((errno == ERANGE && l == LONG_MAX) /*|| l > INT_MAX*/) {
        return CA_STR2INT_OVERFLOW;
    }
    if ((errno == ERANGE && l == LONG_MIN) /*|| l < INT_MIN*/) {
        return CA_STR2INT_UNDERFLOW;
    }
    if (*s == '\0' || *end != '\0') {
        return CA_STR2INT_INCONVERTIBLE;
    }
    i = l;
    return CA_STR2INT_SUCCESS;
}

static inline
STR2INT_ERROR str2int(int &i, char const *s, int base = 0)
{
    long l = 0;
    const auto res = str2long(l, s, base);
    if (CA_STR2INT_SUCCESS != res)
        return res;
    if (l > INT_MAX)
        return CA_STR2INT_OVERFLOW;
    if (l < INT_MIN)
        return CA_STR2INT_UNDERFLOW;
    i = l;
    return CA_STR2INT_SUCCESS;
}

static inline
char get_next(std::ifstream& in, std::ofstream& out, std::deque<int>& queue, std::multiset<int>& min)
{
    std::string line;
    if (!std::getline(in, line))
        return 0;

    if ('-' == line[0])
    {
        //out << queue.front() << '\n';
        min.erase(min.find(queue.front()));
        queue.pop_front();
    }
    else if ('+' == line[0])
    {
        int nb;
        (void)str2int(nb, line.c_str() + 2);
        queue.push_back(nb);
        min.insert(nb);
    }
    else if ('?' == line[0])
    {
        out << *min.begin() << '\n';
    }

    return line[0];
}

int stack()
{
    std::ifstream in("queuemin.in");
    if (!in)
        return -1;
    std::ofstream out("queuemin.out");
    if (!in)
        return -2;
    int N;
    in >> N;
    if (in.bad())
        return -3;
    std::string line;
    //  empty line
    if (!std::getline(in, line))
        return -3;

    std::deque<int> queue;
    static_assert(sizeof(int) == 4, "assure the needed capacity");
    std::multiset<int> min;

    for (auto i = 0; i < N; ++i)
    {
        if (!get_next(in, out, queue, min))
            return -4;
    }

    return 0;
}

//int main()
//{
//    (void)stack();
//}
