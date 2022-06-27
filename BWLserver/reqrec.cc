#include "reqrec.hh"

extern std::vector<bwl::__page *> pages; //页列表
extern bwl::id_t current_pgid;           //当前页

extern std::vector<bwl::__frame *> frames; //窗口列表

extern bwl::Pipe server_recv;

namespace bwl
{

    void do_create_page(req_pack *rp)
    {
    }

    /**
     * @brief req信号接受函数
     *
     */
    void reqrec(int)
    {
        req_pack *rp = new req_pack;
        server_recv.read((char *)rp, sizeof(req_pack));
        if (rp->magic != REQMAGIC)
            return;
        switch (rp->request)
        {
        case request::create_page:
            do_create_page(rp);
            break;

        default:
            break;
        }
    }
};
