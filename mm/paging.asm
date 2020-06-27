global enable_paging
enable_paging:
	mov eax, [esp+4]
	mov cr3, eax

	mov ecx, cr4
	and ecx, 0x00000010
	mov cr4, ecx

	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	ret
