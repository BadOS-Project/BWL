#include "reqrec.hh"

extern std::vector<bwl::__page *> pages; //页列表
extern bwl::id_t current_pgid;           //当前页

extern std::vector<bwl::__frame *> frames; //窗口列表

extern bwl::Pipe server_recv;

namespace bwl
{
    /**
     * @brief req信号接受函数
     *
     */
    void reqrec(int)
    {
        req_pack *rp = new req_pack;
        server_recv.read((char *)rp, sizeof(req_pack));
    }
};
