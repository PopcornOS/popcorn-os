ifeq ($(CC),cl)
define pop-c
	@# MSVC path: compile to OBJ, then convert to raw binary
	@# Warning: does not play nice with non-inline functions.
	cl /nologo /I .. /GS- /Zi /W3 /Od /D UNICODE /D _UNICODE /c $(1) /Fo$(1).obj
	link.exe /NOLOGO /NODEFAULTLIB /ENTRY:pop_main /SUBSYSTEM:NATIVE /OUT:$(1).exe $(1).obj
	objcopy -O binary $(1).exe $(2)
	rm $(1).obj $(1).exe
endef
else
define pop-c
	@# GCC/Clang path: freestanding compile + LD to raw binary
	@# Warning: untested
	$(CC) -I.. -ffreestanding -fno-stack-protector -nostdlib \
	      -fshort-wchar -mno-red-zone -c $(1) -o $(1).o
	ld -nostdlib -e pop_main -Ttext=0x0 --oformat binary \
	       $(1).o -o $(2)
	rm $(1).o
endef
endif

.PHONY: default

default:
	$(call pop-c,main.c,../uefi/system/cstart.bin)