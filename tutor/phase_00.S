
./bomb:     file format elf64-x86-64


Disassembly of section .init:

Disassembly of section .plt:

Disassembly of section .plt.sec:

Disassembly of section .text:

0000000000401460 <phase_00>:
  401460:	endbr64
  401464:	push   %rbp
  401465:	mov    %rsp,%rbp
  401468:	push   %r12
  40146a:	mov    %rdi,%r12
  40146d:	push   %rbx
  40146e:	mov    %rsi,%rbx
  401471:	mov    $0x2,%esi
  401476:	sub    $0x10,%rsp
  40147a:	mov    %fs:0x28,%rax
  401483:	mov    %rax,-0x18(%rbp)
  401487:	xor    %eax,%eax
  401489:	call   4014f0 <valid_u32_fields>
  40148e:	test   %al,%al
  401490:	jne    4014b0 <phase_00+0x50>
  401492:	xor    %eax,%eax
  401494:	mov    -0x18(%rbp),%rdx
  401498:	sub    %fs:0x28,%rdx
  4014a1:	jne    4014e8 <phase_00+0x88>
  4014a3:	add    $0x10,%rsp
  4014a7:	pop    %rbx
  4014a8:	pop    %r12
  4014aa:	pop    %rbp
  4014ab:	ret
  4014ac:	nopl   0x0(%rax)
  4014b0:	xor    %eax,%eax
  4014b2:	lea    -0x1c(%rbp),%rcx
  4014b6:	lea    -0x20(%rbp),%rdx
  4014ba:	mov    $0x402041,%esi
  4014bf:	mov    %r12,%rdi
  4014c2:	call   401150 <__isoc99_sscanf@plt>
  4014c7:	cmp    $0x2,%eax
  4014ca:	jne    401492 <phase_00+0x32>
  4014cc:	shr    $0x5,%rbx
  4014d0:	movzbl %bl,%edi
  4014d3:	xor    $0x2a,%edi
  4014d6:	cmp    %edi,-0x20(%rbp)
  4014d9:	jne    401492 <phase_00+0x32>
  4014db:	call   401450 <tutorial_mix>
  4014e0:	cmp    %eax,-0x1c(%rbp)
  4014e3:	sete   %al
  4014e6:	jmp    401494 <phase_00+0x34>
  4014e8:	call   401120 <__stack_chk_fail@plt>

Disassembly of section .fini:
