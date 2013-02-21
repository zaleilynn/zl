#ifndef _LYNN_INCLUDE_SYSTEM_H_
#define _LYNN_INCLUDE_SYSTEM_H_

#include <stdint.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <sys/statfs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <string>
#include <algorithm>
#include <sys/param.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <errno.h>

using std::string;
class System {
public:

    static bool SetAllFD(){
        struct dirent *ent = NULL;
        DIR *dir = opendir("/dev/fd");
        if(dir == NULL){
            return false;
        }
        while(NULL != (ent = readdir(dir))) {
            if(ent->d_type != DT_REG && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                int32_t num = atoi(ent->d_name);
                fcntl(num, F_SETFD, FD_CLOEXEC);
            }
        }
        closedir(dir);
    }

    // @return:logic cpu numbers
    static int CpuNum() {
        int cpu_num = 0;
 
        FILE *fp = fopen("/proc/cpuinfo", "r");
        if (!fp) {
            return 1;
        }
 
        char buf[256] = {0};
        //  Look at each line, count line begin with
        while (fgets(buf, sizeof(buf)-1, fp)) {
            if (strncmp(buf, "processor", 9) == 0) {
                cpu_num++;
            }
        }
 
        // Done reading; close the file
        fclose(fp);
        return cpu_num ? cpu_num : 1;
    }

    // @return: total physical memory size
    static int TotalMemory() {
        int total = 0;
        char buf[1024];
        FILE* fp = fopen("/proc/meminfo", "r");
        if (!fp) {
            return -1;
        }
 
        while (fgets(buf, sizeof(buf)-1, fp)) {
            if (strncmp(buf, "MemTotal:", 9))
                continue;
            char* b = strstr(buf, "MemTotal: ");
            sscanf(b, "MemTotal: %d", &total);
        }
        fclose(fp);
        total /= 1024;
        return total;
    }

    //  @return: used memory
    static int UsedMemory() {
        int total = TotalMemory();
        int free = PhysicalMemory();
        if (total == -1 || free == -1 || free > total)
            return -1;
        return total - free;
    }

    // @return: free physical memory
    static int PhysicalMemory() {
        int free = 0;
        int buffer = 0;
        int cache = 0;
        char buf[1024];
        FILE* fp = fopen("/proc/meminfo", "r");
        if (!fp) {
            return -1;
        }
 
        while (fgets(buf, sizeof(buf)-1, fp)) {
            if (!strncmp(buf, "MemFree:", 8)){
                char* b = strstr(buf, "MemFree: ");
                sscanf(b, "MemFree: %d", &free);
                continue;
            } else if(!strncmp(buf, "Buffers:", 8)){
                char* b = strstr(buf, "Buffers: ");
                sscanf(b, "Bufffers: %d", &buffer);
                continue;
            } else if(!strncmp(buf, "Cached:", 7)){
                char* b = strstr(buf, "Cached: ");
                sscanf(b, "Cached: %d", &cache);
                break;
            }
        }
        fclose(fp);
        return (free + buffer + cache) / 1024;
    }


    // @return: return free swap size if success, or return -1
    static int VirtualMemory() {
        int free_swap = -1;
        // gather info from /proc/meminfo to get free swap
        FILE *fp = fopen("/proc/meminfo", "r");
        char buf[256] = {0};
        if (!fp) {
            return -1;
        }
 
        /* Look at each line, count line begin with  */
        while (fgets(buf, sizeof(buf)-1, fp)) {
            if (strncmp(buf, "SwapFree", 8) == 0) {
                if (sscanf(buf, "SwapFree: %d", &free_swap) < 1) {
                    fclose(fp);
                    return -1;
                }
                break;
            }
        }
 
        // Done reading; close the file
        fclose(fp);
        free_swap /= 1024;
        return free_swap;
    }

    // @return:return cpu load if success, or return -1.0
    static double LoadAvg() {
        double short_avg, medium_avg, long_avg;
        FILE* fp = fopen("/proc/loadavg", "r");
        if (!fp)
            return -1.0;
 
        // gather info from /proc/meminfo to get free swap
        if (!fp) {
            return -1.0;
        }
 
        fscanf(fp, "%lf %lf %lf", &short_avg, &medium_avg, &long_avg);
        fclose(fp);
        return medium_avg;
    }

    static double MemoryUsage() {
        int total = TotalMemory();
        int used = UsedMemory();
        if (total == -1 || used == -1 || used > total)
            return -1;
        return (double) used / (double) total;
    }

    // @return: cpu usage
    static double CpuUsage() {
        FILE *fp;
        char line[8192];
        static uint64_t prev_used = 0;
        static uint64_t prev_total = 0;
        static bool first = true;
 
        if ((fp = fopen("/proc/stat", "r")) == NULL) {
            return -1.0;
        }
 
        uint64_t cpu_user = 0;
        uint64_t cpu_nice = 0;
        uint64_t cpu_sys = 0;
        uint64_t cpu_idle = 0;
        uint64_t cpu_iowait = 0;
        uint64_t cpu_hardirq = 0;
        uint64_t cpu_softirq = 0;
        uint64_t used = 0;
        uint64_t total = 0;
        while (fgets(line, 8192, fp) != NULL) {
            if (!strncmp(line, "cpu ", 4)) {
                sscanf(line + 5, "%lu %lu %lu %lu %lu %lu %lu",
                        &cpu_user, &cpu_nice, &cpu_sys, &cpu_idle,
                        &cpu_iowait, &cpu_hardirq, &cpu_softirq);
                used = cpu_user + cpu_nice + cpu_sys +
                       cpu_iowait + cpu_hardirq + cpu_softirq;
                total = used + cpu_idle;
                break;
            }
        }
 
        if (first) {
            first = false;
            prev_used = used;
            prev_total = total;
            fclose(fp);
            return 0.0;
        }
 
        // 回避除以0的问题
        if (total == prev_total) {
            total = prev_total + 1;
        }
        double cpu_usage = static_cast<double>(used - prev_used)
                            / static_cast<double>(total - prev_total);
        prev_used = used;
        prev_total = total;
        fclose(fp);
        return cpu_usage;
    }
 
    static void GetDiskSpace(int32_t *total_disk, int32_t *avail_disk){
        char buf[1024];
        string cmd = "df --block-size=G";
        FILE *stream = popen(cmd.c_str(),"r"); 
        char location[100], mount_on[100];
        float percentage;
        int32_t capacity, used, available;
        if(stream == NULL){
            //出了某些错误
            *total_disk = *avail_disk = -1;
        }
 
        while(fgets(buf, sizeof(buf), stream)){
            char* b = strstr(buf," /\n");
            if(b == NULL) {
                continue;
            }             
            sscanf(buf, "%s %dG %dG %dG %f%% %s", location, &capacity, &used, &available, &percentage, mount_on);
        }
        *total_disk = capacity;
        *avail_disk = available;
        pclose(stream);
    }

    static int32_t GetFakeSpace(const string& img_dir) { 
        struct dirent* ent = NULL;
        DIR *dir = opendir(img_dir.c_str());
        int32_t rtn = 0;
        if(!dir){
            return 0;
        }
        while(NULL != (ent = readdir(dir))) {
            if(ent->d_type != DT_REG && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                string name = ent->d_name;
                //找到已经有的镜像
                if(name.find("_") != string::npos) {
                    string full = img_dir + name + "/" + name + ".img";
                    struct stat buf;
                    if(stat(full.c_str(), &buf) != -1) {
                        unsigned long size = (unsigned long)buf.st_size;
                        rtn += size / (1024 * 1024 * 1024);
                    }
                }
            }
        }
        closedir(dir);
        return rtn;
    }

    // @return: cpu usage
    static uint64_t CpuTime() {
        FILE *fp;
        char line[8192];
        if ((fp = fopen("/proc/stat", "r")) == NULL) {
            return -1;
        }
 
        uint64_t cpu_user = 0;
        uint64_t cpu_nice = 0;
        uint64_t cpu_sys = 0;
        uint64_t cpu_idle = 0;
        uint64_t cpu_iowait = 0;
        uint64_t cpu_hardirq = 0;
        uint64_t cpu_softirq = 0;
        uint64_t total = 0;
        while (fgets(line, 8192, fp) != NULL) {
            if (!strncmp(line, "cpu ", 4)) {
                sscanf(line + 5, "%lu %lu %lu %lu %lu %lu %lu",
                        &cpu_user, &cpu_nice, &cpu_sys, &cpu_idle,
                        &cpu_iowait, &cpu_hardirq, &cpu_softirq);
                total = cpu_user + cpu_nice + cpu_sys +
                       cpu_iowait + cpu_hardirq + cpu_softirq + cpu_idle;
                break;
            }
        }
 
        fclose(fp);
        return total;
    }

    static void RemoveDir(const char* path) {
        DIR* dp = opendir(path);
        if (dp) {
            dirent* ep = NULL;
            while((ep = readdir(dp)) != NULL) {
                // get rid of .. and .
                if (strcmp("..", ep->d_name) != 0 &&
                    strcmp(".", ep->d_name) != 0) {
                    char file[256] = {0};
                    snprintf(file, sizeof(file), "%s/%s", path, ep->d_name);
                    struct stat info;
                    stat(file, &info);
                    // directory or file
                    if (S_ISDIR(info.st_mode))
                        RemoveDir(file);
                    else
                        unlink(file);
                }
            }
            rmdir(path);
        }
        closedir(dp);
    }

    void GetCurrentTime(char* time_str, int len) {
        time_t cur_time;
        time(&cur_time);
        tm* p = gmtime(&cur_time);
        snprintf(time_str, len, "%d%d%d-%d%d%d", (1900 + p->tm_year),
                 (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
    }

    static void GetNetFlow(const char* interface, int64_t *bytes_in, int64_t *bytes_out){
        FILE *fp;
        char buf[256];
        char name[30];
        long rb, rpkt, r_err, r_drop, r_fifo, r_frame, r_compr, r_mcast;
        long tb, tpkg, t_err, t_drop, t_fifo, t_frame, t_compr, t_mcast;
 
        //先清0
        *bytes_in = *bytes_out = 0;
 
        fp = fopen("/proc/net/dev", "r");
        if(!fp) {
            return;
        }
 
        snprintf(name, 30, "%s:", interface);
 
        while(fgets(buf, sizeof(buf) - 1, fp)){
            char* p = buf;
            //去掉空格
            while(*p == ' ') {
                p++;
            }
            if(!strncmp(p, name, strlen(name))) {
                //忽略了返回值为空的情况
                char* b = strstr(p, name);
                sscanf(b + strlen(name), "%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld", &rb, &rpkt, &r_err, &r_drop, &r_fifo, &r_frame, &r_compr, &r_mcast, &tb, &tpkg, &t_err, &t_drop, &t_fifo, &t_frame, &t_compr, &t_mcast);
                *bytes_in = rb;
                *bytes_out = tb;
            }
        }
        fclose(fp);
    }

    static string GetIP(string interface){
        int32_t sock;
        struct sockaddr_in sin;
        struct ifreq ifr; 
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if(sock == -1) {
             return "";
        } 
        strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ);
        ifr.ifr_name[IFNAMSIZ - 1] = 0;
        if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
            return "";
        }
        memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
        return string(inet_ntoa(sin.sin_addr));
    }
};

#endif

#if 0
int get_ip(char* str) {
    addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_flags = AI_CANONNAME;
    hint.ai_socktype = SOCK_STREAM;
    char hostname[100] =  {0};
    gethostname(hostname, sizeof(hostname));
    int ret;
    addrinfo* res;
    if ((ret = getaddrinfo(hostname, NULL, &hint, &res)) < 0) {
        LOG(ERROR) << "get ip error: " << gai_strerror(ret);
        return ret;
    }
    addrinfo* curr = res;
    if (curr != NULL) {
        // get the fisrt address as ip
        char* ip = inet_ntoa(((sockaddr_in* )(res->ai_addr))->sin_addr);
        strcpy(str, ip);
        while (curr != NULL) {
            addrinfo* tmp = curr->ai_next;
            freeaddrinfo(curr);
            curr = tmp;
        }
        return 0;
    } else {
        return -1;
    }
}
#endif

