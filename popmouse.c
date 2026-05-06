#include <efi.h>
#include <efilib.h>
#include "popcorn.h"

extern EFI_SYSTEM_TABLE *gST;
extern EFI_BOOT_SERVICES *gBS;
extern EFI_RUNTIME_SERVICES *gRT;
extern EFI_HANDLE *ImageHandle;

int pop_API popmk_init(popm_MouseServices* msvc) {
    EFI_SIMPLE_POINTER_PROTOCOL* spp;
    EFI_GUID sppGuid = EFI_SIMPLE_POINTER_PROTOCOL_GUID;
    EFI_HANDLE* handles;
    EFI_STATUS Status;
    UINTN handleCount;

    Status = gBS->LocateHandleBuffer(ByProtocol, &sppGuid, NULL, &handleCount, &handles);
    if (EFI_ERROR(Status)) return pop_EUEFIPR;
    Status = gBS->HandleProtocol(handles[0], &sppGuid, (VOID **)&spp);
    if (EFI_ERROR(Status)) return pop_EUEFIPR;
    
    spp->Reset(spp, FALSE);
    
    msvc->shndl = (void*)spp;
    msvc->sensitivity = 0.1;
    
    return pop_SUCCESS;
}

int pop_API popmk_query(popm_MouseServices* msvc) {
    EFI_SIMPLE_POINTER_PROTOCOL* spp = (EFI_SIMPLE_POINTER_PROTOCOL*)msvc->shndl;
    EFI_STATUS Status;
    UINTN index;
    
    //gBS->WaitForEvent(1, &spp->WaitForInput, &index);
    
    EFI_SIMPLE_POINTER_STATE state;
    Status = spp->GetState(spp, &state);
    
    if (EFI_ERROR(Status)) { return pop_EUEFIPR; }
    
    double scale = ((double)(msvc->svc->sgfx->w + msvc->svc->sgfx->h) / 2.0) * msvc->sensitivity;
    
    if (!EFI_ERROR(Status)) {
        msvc->x += (unsigned int)(state.RelativeMovementX * scale);
        msvc->y += (unsigned int)(state.RelativeMovementY * scale);
        msvc->z += (unsigned int)(state.RelativeMovementZ * scale);

        // Clamp to framebuffer bounds
        if (msvc->x < 0) msvc->x = 0;
        if (msvc->y < 0) msvc->y = 0;
        if (msvc->x >= msvc->svc->sgfx->w) msvc->x = msvc->svc->sgfx->w - 1;
        if (msvc->y >= msvc->svc->sgfx->h) msvc->y = msvc->svc->sgfx->h - 1;

        msvc->left  = state.LeftButton;
        msvc->right = state.RightButton;
        return pop_SUCCESS;
    } else {
        return pop_EUEFIPR;
    }
}