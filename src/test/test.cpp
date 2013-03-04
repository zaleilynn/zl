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
    virDomainPtr vm_ptr = virDomainLookupByName(conn, "template");
    if(vm_ptr == NULL) {
        printf("error finding domain\n");
        virConnectClose(conn);
        exit(1);
    }
    virTypedParameter par[4];
    int n = 5;
    virDomainGetMemoryParameters(vm_ptr, par, &n, 0);
    printf("%d\n", n);
    for(int i = 0; i < n ; i++) {
        printf("%s %d %llu\n", par[i].field, par[i].type, par[i].value.ul);
    }
    virDomainFree(vm_ptr);
    virConnectClose(conn);
    return 0;
}
