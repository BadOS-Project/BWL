#include "reqrec.hh"

#include <cstring>
#include <iostream>

extern std::map<bwl::id_t, bwl::__page *> pages; //页列表
uint64_t pgcount;
extern bwl::id_t current_pgid; //当前页

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
        pg->owners[0] = rp->pid;
    }

    void do_delete_page(req_pack *rp)
    {
        deletePage(pages.at(rp->arguments.a_del_page.pgid));
        pages.erase(rp->arguments.a_del_page.pgid);
        memset(pages.at(rp->arguments.a_del_page.pgid)->owners, 0, MAX_OWNERS * sizeof(pid_t));
    }

    void do_create_frame(req_pack *rp)
    {
        std::string frname = rp->arguments.a_create_frame.name;
        __frame *fr = createFrame(frcount++,                                // fid设为已创建的frame数量
                                  pages[rp->arguments.a_create_frame.pgid], //所属的页
                                  frname,                                   //窗口名
                                  rp->arguments.a_create_frame.size[0],     //
                                  rp->arguments.a_create_frame.size[1],     //窗口大小
                                  rp->arguments.a_create_frame.pos[0],      //
                                  rp->arguments.a_create_frame.pos[1]       //窗口位置
        );
        fr->space = rp->arguments.a_create_frame.space;
        frames[fr->fid] = fr;
        fr->owner = rp->pid;
    }

    void do_delete_frame(req_pack *rp)
    {
        deleteFrame(frames[rp->arguments.a_del_frame.fid]);
        frames.erase(rp->arguments.a_del_frame.fid);
        frames.at(rp->arguments.a_del_frame.fid)->owner = 0;
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
        id_t id;
        switch (rp->request)
        {
        case requests::create_page:
            do_create_page(rp);
            id = pgcount-1;
            break;
        case requests::del_page:
            do_delete_page(rp);
            break;
        case requests::create_frame:
            do_create_frame(rp);
            id=frcount-1;
            break;
        case requests::del_frame:
            do_delete_frame(rp);
            break;
        default:
            break;
        }
        Pipe reply(BWLDIR + "/reply/" + std::to_string(rp->pid) + ".pipe", Pipe::out);
        reply.write((char *)&id, sizeof(id_t));
        reply.close();
    }
};
