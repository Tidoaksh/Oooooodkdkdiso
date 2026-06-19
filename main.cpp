#include <orbis/libkernel.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define PAGE_SIZE 0x1000

void Notify(const char* text)
{
    OrbisNotificationRequest req;
    memset(&req, 0, sizeof(req));

    req.type = 0;
    req.unk3 = 0;
    req.useIconImageUri = 1;
    req.targetId = -1;

    strcpy(
        req.iconUri,
        "cxml://psnotification/tex_g_trophy_gold"
    );

    snprintf(
        req.message,
        sizeof(req.message),
        "%s",
        text
    );

    sceKernelSendNotificationRequest(
        0,
        &req,
        sizeof(req),
        0
    );
}

uint64_t GetGameBaseAddress()
{
    OrbisKernelModuleInfo info;
    memset(&info, 0, sizeof(info));

    info.size = sizeof(info);

    if (sceKernelGetModuleInfo(0x2, &info) != 0)
        return 0;

    return (uint64_t)info.segmentInfo[0].address;
}

void DumpPointers(uint64_t base)
{
    FILE* f = fopen("/data/net_dump.txt", "w");

    if (!f)
    {
        Notify("Error creando dump");
        return;
    }

    fprintf(f, "=== netConnectionManager dump ===\n");
    fprintf(f, "Base: 0x%lX\n\n", base);

    for (int i = 0; i < 128; i++)
    {
        uint64_t addr = base + (i * 8);
        uint64_t value = *(uint64_t*)addr;

        fprintf(
            f,
            "[+0x%03X] -> 0x%016lX\n",
            i * 8,
            value
        );
    }

    fclose(f);
}

extern "C" int module_start(size_t argc, const void* args)
{
    Notify("PRX Loaded");

    uint64_t base = GetGameBaseAddress();

    if (!base)
    {
        Notify("Base not found");
        return -1;
    }

    uint64_t netConnectionManager =
        base + 0x00096360;

    DumpPointers(netConnectionManager);

    Notify("Dump Complete");

    return 0;
}

extern "C" int module_stop(size_t argc, const void* args)
{
    Notify("PRX Unloaded");
    return 0;
}
