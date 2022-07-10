#ifndef __bwl_hh_
#define __bwl_hh_

#include "bsv.hh"

namespace bwl
{

    class Page
    {
    private:
        __page *page;

    public:
        Page();
        ~Page();
    };

};

#endif
