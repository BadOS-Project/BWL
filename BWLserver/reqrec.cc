#include "reqrec.hh"

extern std::map<bwl::id_t, bwl::__page *> pages; //页列表
uint64_t pgcount;
extern bwl::id_t current_pgid;                   //当前页

extern std::map<bwl::id_t, bwl::__frame *> frames; //窗口列表
uint64_t frcount;

extern bwl::Pipe server_recv;

namespace bwl
{

    void do_create_page(req_pack *rp)
    {
        __page *pg = createPage(pgcount++);
        pg->space = rp->arguments.a_create_page.bgspace;
        pages[pg->pgid] = pg;
    }

    void do_delete_page(req_pack *rp)
    {
        deletePage(pages.at(rp->arguments.a_del_page.pgid));
        pages.erase(rp->arguments.a_del_page.pgid);
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
        case requests::create_page:
            do_create_page(rp);
            break;
        case requests::del_page:
            do_delete_page(rp);
            break;
        default:
            break;
        }
    }
};
