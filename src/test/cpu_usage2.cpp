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

    virTypedParameter par_s, par_e;
    struct timeval real_time_s, real_time_e;

    int cpu_diff, real_diff;
    float usage;

    while(true){
        if(gettimeofday(&real_time_s, NULL) == -1) {
            printf("error get time of day\n");
            virDomainFree(vm_ptr);
            virConnectClose(conn);
            exit(1);
        } 
        virDomainGetCPUStats(vm_ptr, &par_s, 1, -1, 1, 0);
        sleep(1);
        if(gettimeofday(&real_time_e, NULL) == -1) {
            printf("error get time of day\n");
            virDomainFree(vm_ptr);
            virConnectClose(conn);
            exit(1);
        }

        virDomainGetCPUStats(vm_ptr, &par_e, 1, -1, 1, 0);
        cpu_diff = (par_e.value.ul - par_s.value.ul) / 1000;
        real_diff = 1000000 * (real_time_e.tv_sec - real_time_s.tv_sec) +
                    (real_time_e.tv_usec - real_time_s.tv_usec);
        usage = cpu_diff / (float)(real_diff);
        printf("cpu_diff:%d, real_diff:%d, cpu usage:%f\n", cpu_diff, real_diff, usage);

    }
    virDomainFree(vm_ptr);
    virConnectClose(conn);
    return 0;
}


