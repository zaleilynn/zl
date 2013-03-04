#include <stdio.h>
#include <sys/time.h>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
#include "include/proxy.h"
#include "include/rpc.h"

int main(int argc, char ** argv) {
    virConnectPtr conn = virConnectOpen("qemu:///system");
    if(conn == NULL) {
        printf("error connecting qemu driver\n");
        exit(1);
    }
    virDomainPtr vm_ptr = virDomainLookupByName(conn, "1");
    if(vm_ptr == NULL) {
        printf("error finding domain\n");
        virConnectClose(conn);
        exit(1);
    }
    virDomainInfo info_s,info_e;
    struct timeval real_time_s, real_time_e;
     int cpu_diff, real_diff;
    float usage;

    while(true){ 
        if(virDomainGetInfo(vm_ptr, &info_s) !=0) {
            printf("error get domain info\n");  
            virDomainFree(vm_ptr);
            virConnectClose(conn); 
            exit(1);
        }
        
        if(gettimeofday(&real_time_s, NULL) == -1) {
            printf("error get time of day\n");
            virDomainFree(vm_ptr);
            virConnectClose(conn);
            exit(1);
        }
 
        sleep(1);
       
        if(virDomainGetInfo(vm_ptr, &info_e) !=0) {
            printf("error get domain info\n");
            virDomainFree(vm_ptr);
            virConnectClose(conn);
            exit(1);
        }
 
        if(gettimeofday(&real_time_e, NULL) == -1) {
            printf("error get time of day\n");
            virDomainFree(vm_ptr);
            virConnectClose(conn);
            exit(1);
        }
 
        //转换成微秒
        cpu_diff = (info_e.cpuTime - info_s.cpuTime) / 1000;
        //转换成微秒
        real_diff = 1000000 * (real_time_e.tv_sec - real_time_s.tv_sec) +  
                    (real_time_e.tv_usec - real_time_s.tv_usec);
        usage = cpu_diff / (float) (real_diff);
        printf("cpu_diff:%d, real_diff:%d, cpu usage:%f\n", cpu_diff, real_diff, usage);
    }
    virDomainFree(vm_ptr);
    virConnectClose(conn);
    return 0;
}


