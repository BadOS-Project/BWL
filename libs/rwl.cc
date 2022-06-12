#define __rwl_server

#include "../includes/rwl.hh"

#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>

namespace rwl
{
    /**
     * @brief 通过进程名获取pid
     *
     * @param task_name
     * @return int
     */
    int get_pid_by_name(const char *task_name)
    {
        int pid_value = -1;
        DIR *dir;
        struct dirent *ptr;
        FILE *fp;
        char filepath[512] = {0};
        char cur_task_name[512] = {0};
        char buf[1024] = {0};

        dir = opendir("/proc");
        if (NULL != dir)
        {
            while ((ptr = readdir(dir)) != NULL)
            {
                if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                    continue;

                if (DT_DIR != ptr->d_type)
                    continue;

                sprintf(filepath, "/proc/%s/status", ptr->d_name);

                fp = fopen(filepath, "r");
                if (NULL != fp)
                {
                    if (fgets(buf, 1023, fp) == NULL)
                    {
                        fclose(fp);
                        continue;
                    }

                    sscanf(buf, "%*s %s", cur_task_name);
                    if (!strcmp(task_name, cur_task_name))
                    {
                        printf("PID:%s\n", ptr->d_name);
                        pid_value = atoi(ptr->d_name);
                    }

                    fclose(fp);
                }
            }
            closedir(dir);
        }
        else
        {
            return -1;
        }

        return pid_value;
    }

    void Frame::__Frame()
    {
        this->pid = getpid();
    }

    Frame::Frame()
    {
        this->__Frame();
    }

    Frame::Frame(std::string name)
    {
        this->name = name;
        this->__Frame();
    }

    Frame::Frame(uint64_t size[2])
    {
        memcpy(this->size, size, 2 * sizeof(uint64_t));
        pos[0] = 0;
        pos[1] = 0;
        this->__Frame();
    }

    Frame::Frame(int64_t position[2], uint64_t size[2])
    {
        memcpy(this->pos, position, 2 * sizeof(int64_t));
        memcpy(this->size, size, 2 * sizeof(uint64_t));
        this->__Frame();
    }

    Frame::Frame(std::string name, uint64_t size[2])
    {
        this->name = name;
        memcpy(this->size, size, 2 * sizeof(uint64_t));
        pos[0] = 0;
        pos[1] = 0;
        this->__Frame();
    }

    Frame::Frame(std::string name, int64_t position[2], uint64_t size[2])
    {
        this->name = name;
        memcpy(this->pos, position, 2 * sizeof(int64_t));
        memcpy(this->size, size, 2 * sizeof(uint64_t));
        this->__Frame();
    }

    void Frame::setName(std::string name)
    {
        this->name = name;
    }

    std::string Frame::getName()
    {
        return name;
    }

    int64_t Frame::getX()
    {
        return pos[0];
    }

    int64_t Frame::getY()
    {
        return pos[1];
    }

    int64_t Frame::getWidth()
    {
        return usersize[0];
    }

    int64_t Frame::getHeight()
    {
        return usersize[1];
    }

    int Frame::getMainBufferInfo(void *&buffer, int64_t *&position, uint64_t *&size)
    {
        buffer = this->mbuf;
        position = this->pos;
        size = this->size;
    }

    int Frame::getUserBufferInfo(void *&buffer, int64_t *&position, uint64_t *&size)
    {
        buffer = this->userbuf;
        position = this->ofset;
        size = this->usersize;
    }

    void Frame::setX(int64_t x)
    {
        this->pos[0] = x;
    }
    void Frame::setY(int64_t y)
    {
        this->pos[1] = y;
    }
    int Frame::setWidth(uint64_t width)
    {
        int increment = width - usersize[0];
        size[0] += increment;
        usersize[0] += increment;
        mbuf = realloc_buff(mbuf, size[0] * size[1] * pixel_depth);
        userbuf = realloc_buff(userbuf, usersize[0] * usersize[1] * pixel_depth);
    }
    int Frame::setHeight(uint64_t height)
    {
        int increment = height - usersize[1];
        size[1] += increment;
        usersize[1] += increment;
        mbuf = realloc_buff(mbuf, size[0] * size[1] * pixel_depth);
        userbuf = realloc_buff(userbuf, usersize[0] * usersize[1] * pixel_depth);
    }

};
