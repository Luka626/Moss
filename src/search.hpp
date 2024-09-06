#ifndef SEARCH_HPP_
#define SEARCH_HPP_

#include "position.hpp"
class Search{
    public:
        Search(Position *position_ptr);
        Move simple_search();

        
    private:
        Position *pos;

};


#endif
