#ifndef CD_HPP
#define CD_HPP

#include <map>
#include <vector>
#include <string>

class CD
{
    public:
        CD();

    private:
        std::map <std::string, std::vector <std::string> >
                data;
};

#endif
