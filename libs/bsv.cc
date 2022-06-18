#include "../includes/bsv.hh"
#include "../dbg/logs.hh"

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <unistd.h>

namespace bwl
{

    /**
     * @brief 创建页
     *
     * @param pgid 使用的pgid
     * @return __page*
     */
#define CREATE_PAGE_FAULT (__page *)(-2)
#define TRUNCATE_SHM_FAULT (__page *)(-3)
    __page *createPage(id_t pgid)
    {
        int shmf = shm_open((PAGEDIR + std::to_string(pgid) + "shm").c_str(), O_RDWR | O_EXCL, 0);
        if (shmf == -1)
        {
            bwl::err("creating page failed.");
            return CREATE_PAGE_FAULT;
        }
        if (ftruncate(shmf, sizeof(__page)))
        {
            bwl::err("truncate shm size failed.");
            return TRUNCATE_SHM_FAULT;
        }
        __page *page = (__page *)mmap(NULL, sizeof(__page), PROT_READ | PROT_WRITE, MAP_SHARED, shmf, 0);

        close(shmf);

        return page;
    }
};
