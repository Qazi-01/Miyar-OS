
build/kernel.elf:     file format elf32-i386


Disassembly of section .text:

00100000 <_start-0x10>:
  100000:	02 b0 ad 1b 03 00    	add    0x31bad(%eax),%dh
  100006:	00 00                	add    %al,(%eax)
  100008:	fb                   	sti
  100009:	4f                   	dec    %edi
  10000a:	52                   	push   %edx
  10000b:	e4 66                	in     $0x66,%al
  10000d:	90                   	nop
  10000e:	66 90                	xchg   %ax,%ax

00100010 <_start>:
  100010:	bc 00 70 10 00       	mov    $0x107000,%esp
  100015:	53                   	push   %ebx
  100016:	50                   	push   %eax
  100017:	e8 04 00 00 00       	call   100020 <kernel_main>
  10001c:	fa                   	cli

0010001d <_start.hang>:
  10001d:	f4                   	hlt
  10001e:	eb fd                	jmp    10001d <_start.hang>

00100020 <kernel_main>:
  100020:	55                   	push   %ebp
  100021:	89 e5                	mov    %esp,%ebp
  100023:	81 ec 98 00 00 00    	sub    $0x98,%esp
  100029:	e8 fa 00 00 00       	call   100128 <serial_init>
  10002e:	e8 72 03 00 00       	call   1003a5 <terminal_init>
  100033:	e8 df 0e 00 00       	call   100f17 <pic_init>
  100038:	e8 89 07 00 00       	call   1007c6 <idt_init>
  10003d:	fb                   	sti
  10003e:	e8 b6 04 00 00       	call   1004f9 <keyboard_init>
  100043:	83 ec 0c             	sub    $0xc,%esp
  100046:	68 ca 0f 10 00       	push   $0x100fca
  10004b:	e8 c2 03 00 00       	call   100412 <terminal_write>
  100050:	83 c4 10             	add    $0x10,%esp
  100053:	83 ec 0c             	sub    $0xc,%esp
  100056:	68 d4 0f 10 00       	push   $0x100fd4
  10005b:	e8 b2 03 00 00       	call   100412 <terminal_write>
  100060:	83 c4 10             	add    $0x10,%esp
  100063:	83 ec 0c             	sub    $0xc,%esp
  100066:	68 ed 0f 10 00       	push   $0x100fed
  10006b:	e8 a2 03 00 00       	call   100412 <terminal_write>
  100070:	83 c4 10             	add    $0x10,%esp
  100073:	e8 e6 03 00 00       	call   10045e <terminal_prompt>
  100078:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%ebp)
  10007f:	e8 1e 04 00 00       	call   1004a2 <keyboard_getchar>
  100084:	88 45 f3             	mov    %al,-0xd(%ebp)
  100087:	80 7d f3 00          	cmpb   $0x0,-0xd(%ebp)
  10008b:	74 76                	je     100103 <kernel_main+0xe3>
  10008d:	80 7d f3 0a          	cmpb   $0xa,-0xd(%ebp)
  100091:	75 2e                	jne    1000c1 <kernel_main+0xa1>
  100093:	8d 95 73 ff ff ff    	lea    -0x8d(%ebp),%edx
  100099:	8b 45 f4             	mov    -0xc(%ebp),%eax
  10009c:	01 d0                	add    %edx,%eax
  10009e:	c6 00 00             	movb   $0x0,(%eax)
  1000a1:	83 ec 0c             	sub    $0xc,%esp
  1000a4:	8d 85 73 ff ff ff    	lea    -0x8d(%ebp),%eax
  1000aa:	50                   	push   %eax
  1000ab:	e8 f3 04 00 00       	call   1005a3 <shell_execute>
  1000b0:	83 c4 10             	add    $0x10,%esp
  1000b3:	e8 a6 03 00 00       	call   10045e <terminal_prompt>
  1000b8:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%ebp)
  1000bf:	eb 43                	jmp    100104 <kernel_main+0xe4>
  1000c1:	83 7d f4 7e          	cmpl   $0x7e,-0xc(%ebp)
  1000c5:	7f b8                	jg     10007f <kernel_main+0x5f>
  1000c7:	8b 45 f4             	mov    -0xc(%ebp),%eax
  1000ca:	8d 50 01             	lea    0x1(%eax),%edx
  1000cd:	89 55 f4             	mov    %edx,-0xc(%ebp)
  1000d0:	0f b6 55 f3          	movzbl -0xd(%ebp),%edx
  1000d4:	88 94 05 73 ff ff ff 	mov    %dl,-0x8d(%ebp,%eax,1)
  1000db:	0f b6 45 f3          	movzbl -0xd(%ebp),%eax
  1000df:	88 85 71 ff ff ff    	mov    %al,-0x8f(%ebp)
  1000e5:	c6 85 72 ff ff ff 00 	movb   $0x0,-0x8e(%ebp)
  1000ec:	83 ec 0c             	sub    $0xc,%esp
  1000ef:	8d 85 71 ff ff ff    	lea    -0x8f(%ebp),%eax
  1000f5:	50                   	push   %eax
  1000f6:	e8 17 03 00 00       	call   100412 <terminal_write>
  1000fb:	83 c4 10             	add    $0x10,%esp
  1000fe:	e9 7c ff ff ff       	jmp    10007f <kernel_main+0x5f>
  100103:	90                   	nop
  100104:	e9 76 ff ff ff       	jmp    10007f <kernel_main+0x5f>

00100109 <outb>:
  100109:	55                   	push   %ebp
  10010a:	89 e5                	mov    %esp,%ebp
  10010c:	83 ec 08             	sub    $0x8,%esp
  10010f:	8b 55 08             	mov    0x8(%ebp),%edx
  100112:	8b 45 0c             	mov    0xc(%ebp),%eax
  100115:	66 89 55 fc          	mov    %dx,-0x4(%ebp)
  100119:	88 45 f8             	mov    %al,-0x8(%ebp)
  10011c:	0f b6 45 f8          	movzbl -0x8(%ebp),%eax
  100120:	0f b7 55 fc          	movzwl -0x4(%ebp),%edx
  100124:	ee                   	out    %al,(%dx)
  100125:	90                   	nop
  100126:	c9                   	leave
  100127:	c3                   	ret

00100128 <serial_init>:
  100128:	55                   	push   %ebp
  100129:	89 e5                	mov    %esp,%ebp
  10012b:	6a 00                	push   $0x0
  10012d:	68 f9 03 00 00       	push   $0x3f9
  100132:	e8 d2 ff ff ff       	call   100109 <outb>
  100137:	83 c4 08             	add    $0x8,%esp
  10013a:	68 80 00 00 00       	push   $0x80
  10013f:	68 fb 03 00 00       	push   $0x3fb
  100144:	e8 c0 ff ff ff       	call   100109 <outb>
  100149:	83 c4 08             	add    $0x8,%esp
  10014c:	6a 01                	push   $0x1
  10014e:	68 f8 03 00 00       	push   $0x3f8
  100153:	e8 b1 ff ff ff       	call   100109 <outb>
  100158:	83 c4 08             	add    $0x8,%esp
  10015b:	6a 00                	push   $0x0
  10015d:	68 f9 03 00 00       	push   $0x3f9
  100162:	e8 a2 ff ff ff       	call   100109 <outb>
  100167:	83 c4 08             	add    $0x8,%esp
  10016a:	6a 03                	push   $0x3
  10016c:	68 fb 03 00 00       	push   $0x3fb
  100171:	e8 93 ff ff ff       	call   100109 <outb>
  100176:	83 c4 08             	add    $0x8,%esp
  100179:	68 c7 00 00 00       	push   $0xc7
  10017e:	68 fa 03 00 00       	push   $0x3fa
  100183:	e8 81 ff ff ff       	call   100109 <outb>
  100188:	83 c4 08             	add    $0x8,%esp
  10018b:	6a 0b                	push   $0xb
  10018d:	68 fc 03 00 00       	push   $0x3fc
  100192:	e8 72 ff ff ff       	call   100109 <outb>
  100197:	83 c4 08             	add    $0x8,%esp
  10019a:	90                   	nop
  10019b:	c9                   	leave
  10019c:	c3                   	ret

0010019d <serial_putc>:
  10019d:	55                   	push   %ebp
  10019e:	89 e5                	mov    %esp,%ebp
  1001a0:	83 ec 04             	sub    $0x4,%esp
  1001a3:	8b 45 08             	mov    0x8(%ebp),%eax
  1001a6:	88 45 fc             	mov    %al,-0x4(%ebp)
  1001a9:	0f b6 45 fc          	movzbl -0x4(%ebp),%eax
  1001ad:	0f b6 c0             	movzbl %al,%eax
  1001b0:	50                   	push   %eax
  1001b1:	68 f8 03 00 00       	push   $0x3f8
  1001b6:	e8 4e ff ff ff       	call   100109 <outb>
  1001bb:	83 c4 08             	add    $0x8,%esp
  1001be:	90                   	nop
  1001bf:	c9                   	leave
  1001c0:	c3                   	ret

001001c1 <kputs>:
  1001c1:	55                   	push   %ebp
  1001c2:	89 e5                	mov    %esp,%ebp
  1001c4:	eb 18                	jmp    1001de <kputs+0x1d>
  1001c6:	8b 45 08             	mov    0x8(%ebp),%eax
  1001c9:	8d 50 01             	lea    0x1(%eax),%edx
  1001cc:	89 55 08             	mov    %edx,0x8(%ebp)
  1001cf:	0f b6 00             	movzbl (%eax),%eax
  1001d2:	0f be c0             	movsbl %al,%eax
  1001d5:	50                   	push   %eax
  1001d6:	e8 c2 ff ff ff       	call   10019d <serial_putc>
  1001db:	83 c4 04             	add    $0x4,%esp
  1001de:	8b 45 08             	mov    0x8(%ebp),%eax
  1001e1:	0f b6 00             	movzbl (%eax),%eax
  1001e4:	84 c0                	test   %al,%al
  1001e6:	75 de                	jne    1001c6 <kputs+0x5>
  1001e8:	90                   	nop
  1001e9:	90                   	nop
  1001ea:	c9                   	leave
  1001eb:	c3                   	ret

001001ec <vga_entry>:
  1001ec:	55                   	push   %ebp
  1001ed:	89 e5                	mov    %esp,%ebp
  1001ef:	83 ec 08             	sub    $0x8,%esp
  1001f2:	8b 55 08             	mov    0x8(%ebp),%edx
  1001f5:	8b 45 0c             	mov    0xc(%ebp),%eax
  1001f8:	88 55 fc             	mov    %dl,-0x4(%ebp)
  1001fb:	88 45 f8             	mov    %al,-0x8(%ebp)
  1001fe:	66 0f be 55 fc       	movsbw -0x4(%ebp),%dx
  100203:	0f b6 45 f8          	movzbl -0x8(%ebp),%eax
  100207:	c1 e0 08             	shl    $0x8,%eax
  10020a:	09 d0                	or     %edx,%eax
  10020c:	c9                   	leave
  10020d:	c3                   	ret

0010020e <vga_set_color>:
  10020e:	55                   	push   %ebp
  10020f:	89 e5                	mov    %esp,%ebp
  100211:	83 ec 08             	sub    $0x8,%esp
  100214:	8b 55 08             	mov    0x8(%ebp),%edx
  100217:	8b 45 0c             	mov    0xc(%ebp),%eax
  10021a:	88 55 fc             	mov    %dl,-0x4(%ebp)
  10021d:	88 45 f8             	mov    %al,-0x8(%ebp)
  100220:	0f b6 45 f8          	movzbl -0x8(%ebp),%eax
  100224:	c1 e0 04             	shl    $0x4,%eax
  100227:	89 c2                	mov    %eax,%edx
  100229:	0f b6 45 fc          	movzbl -0x4(%ebp),%eax
  10022d:	09 d0                	or     %edx,%eax
  10022f:	a2 04 20 10 00       	mov    %al,0x102004
  100234:	90                   	nop
  100235:	c9                   	leave
  100236:	c3                   	ret

00100237 <vga_clear>:
  100237:	55                   	push   %ebp
  100238:	89 e5                	mov    %esp,%ebp
  10023a:	53                   	push   %ebx
  10023b:	83 ec 10             	sub    $0x10,%esp
  10023e:	c7 45 f8 00 00 00 00 	movl   $0x0,-0x8(%ebp)
  100245:	eb 2a                	jmp    100271 <vga_clear+0x3a>
  100247:	0f b6 05 04 20 10 00 	movzbl 0x102004,%eax
  10024e:	0f b6 c0             	movzbl %al,%eax
  100251:	8b 15 00 20 10 00    	mov    0x102000,%edx
  100257:	8b 4d f8             	mov    -0x8(%ebp),%ecx
  10025a:	01 c9                	add    %ecx,%ecx
  10025c:	8d 1c 0a             	lea    (%edx,%ecx,1),%ebx
  10025f:	50                   	push   %eax
  100260:	6a 20                	push   $0x20
  100262:	e8 85 ff ff ff       	call   1001ec <vga_entry>
  100267:	83 c4 08             	add    $0x8,%esp
  10026a:	66 89 03             	mov    %ax,(%ebx)
  10026d:	83 45 f8 01          	addl   $0x1,-0x8(%ebp)
  100271:	81 7d f8 cf 07 00 00 	cmpl   $0x7cf,-0x8(%ebp)
  100278:	7e cd                	jle    100247 <vga_clear+0x10>
  10027a:	c7 05 00 70 10 00 00 	movl   $0x0,0x107000
  100281:	00 00 00 
  100284:	c7 05 04 70 10 00 00 	movl   $0x0,0x107004
  10028b:	00 00 00 
  10028e:	90                   	nop
  10028f:	8b 5d fc             	mov    -0x4(%ebp),%ebx
  100292:	c9                   	leave
  100293:	c3                   	ret

00100294 <vga_putc>:
  100294:	55                   	push   %ebp
  100295:	89 e5                	mov    %esp,%ebp
  100297:	56                   	push   %esi
  100298:	53                   	push   %ebx
  100299:	83 ec 04             	sub    $0x4,%esp
  10029c:	8b 45 08             	mov    0x8(%ebp),%eax
  10029f:	88 45 f4             	mov    %al,-0xc(%ebp)
  1002a2:	80 7d f4 0a          	cmpb   $0xa,-0xc(%ebp)
  1002a6:	75 34                	jne    1002dc <vga_putc+0x48>
  1002a8:	c7 05 04 70 10 00 00 	movl   $0x0,0x107004
  1002af:	00 00 00 
  1002b2:	a1 00 70 10 00       	mov    0x107000,%eax
  1002b7:	83 c0 01             	add    $0x1,%eax
  1002ba:	a3 00 70 10 00       	mov    %eax,0x107000
  1002bf:	a1 00 70 10 00       	mov    0x107000,%eax
  1002c4:	83 f8 18             	cmp    $0x18,%eax
  1002c7:	0f 8e a5 00 00 00    	jle    100372 <vga_putc+0xde>
  1002cd:	c7 05 00 70 10 00 18 	movl   $0x18,0x107000
  1002d4:	00 00 00 
  1002d7:	e9 96 00 00 00       	jmp    100372 <vga_putc+0xde>
  1002dc:	a1 00 70 10 00       	mov    0x107000,%eax
  1002e1:	83 f8 18             	cmp    $0x18,%eax
  1002e4:	7e 0a                	jle    1002f0 <vga_putc+0x5c>
  1002e6:	c7 05 00 70 10 00 18 	movl   $0x18,0x107000
  1002ed:	00 00 00 
  1002f0:	0f b6 05 04 20 10 00 	movzbl 0x102004,%eax
  1002f7:	0f b6 d8             	movzbl %al,%ebx
  1002fa:	0f be 4d f4          	movsbl -0xc(%ebp),%ecx
  1002fe:	8b 35 00 20 10 00    	mov    0x102000,%esi
  100304:	8b 15 00 70 10 00    	mov    0x107000,%edx
  10030a:	89 d0                	mov    %edx,%eax
  10030c:	c1 e0 02             	shl    $0x2,%eax
  10030f:	01 d0                	add    %edx,%eax
  100311:	c1 e0 04             	shl    $0x4,%eax
  100314:	89 c2                	mov    %eax,%edx
  100316:	a1 04 70 10 00       	mov    0x107004,%eax
  10031b:	01 d0                	add    %edx,%eax
  10031d:	01 c0                	add    %eax,%eax
  10031f:	01 c6                	add    %eax,%esi
  100321:	53                   	push   %ebx
  100322:	51                   	push   %ecx
  100323:	e8 c4 fe ff ff       	call   1001ec <vga_entry>
  100328:	83 c4 08             	add    $0x8,%esp
  10032b:	66 89 06             	mov    %ax,(%esi)
  10032e:	a1 04 70 10 00       	mov    0x107004,%eax
  100333:	83 c0 01             	add    $0x1,%eax
  100336:	a3 04 70 10 00       	mov    %eax,0x107004
  10033b:	a1 04 70 10 00       	mov    0x107004,%eax
  100340:	83 f8 4f             	cmp    $0x4f,%eax
  100343:	7e 2e                	jle    100373 <vga_putc+0xdf>
  100345:	c7 05 04 70 10 00 00 	movl   $0x0,0x107004
  10034c:	00 00 00 
  10034f:	a1 00 70 10 00       	mov    0x107000,%eax
  100354:	83 c0 01             	add    $0x1,%eax
  100357:	a3 00 70 10 00       	mov    %eax,0x107000
  10035c:	a1 00 70 10 00       	mov    0x107000,%eax
  100361:	83 f8 18             	cmp    $0x18,%eax
  100364:	7e 0d                	jle    100373 <vga_putc+0xdf>
  100366:	c7 05 00 70 10 00 18 	movl   $0x18,0x107000
  10036d:	00 00 00 
  100370:	eb 01                	jmp    100373 <vga_putc+0xdf>
  100372:	90                   	nop
  100373:	8d 65 f8             	lea    -0x8(%ebp),%esp
  100376:	5b                   	pop    %ebx
  100377:	5e                   	pop    %esi
  100378:	5d                   	pop    %ebp
  100379:	c3                   	ret

0010037a <vga_write>:
  10037a:	55                   	push   %ebp
  10037b:	89 e5                	mov    %esp,%ebp
  10037d:	eb 18                	jmp    100397 <vga_write+0x1d>
  10037f:	8b 45 08             	mov    0x8(%ebp),%eax
  100382:	8d 50 01             	lea    0x1(%eax),%edx
  100385:	89 55 08             	mov    %edx,0x8(%ebp)
  100388:	0f b6 00             	movzbl (%eax),%eax
  10038b:	0f be c0             	movsbl %al,%eax
  10038e:	50                   	push   %eax
  10038f:	e8 00 ff ff ff       	call   100294 <vga_putc>
  100394:	83 c4 04             	add    $0x4,%esp
  100397:	8b 45 08             	mov    0x8(%ebp),%eax
  10039a:	0f b6 00             	movzbl (%eax),%eax
  10039d:	84 c0                	test   %al,%al
  10039f:	75 de                	jne    10037f <vga_write+0x5>
  1003a1:	90                   	nop
  1003a2:	90                   	nop
  1003a3:	c9                   	leave
  1003a4:	c3                   	ret

001003a5 <terminal_init>:
  1003a5:	55                   	push   %ebp
  1003a6:	89 e5                	mov    %esp,%ebp
  1003a8:	83 ec 08             	sub    $0x8,%esp
  1003ab:	83 ec 08             	sub    $0x8,%esp
  1003ae:	6a 0f                	push   $0xf
  1003b0:	6a 00                	push   $0x0
  1003b2:	e8 57 fe ff ff       	call   10020e <vga_set_color>
  1003b7:	83 c4 10             	add    $0x10,%esp
  1003ba:	e8 78 fe ff ff       	call   100237 <vga_clear>
  1003bf:	83 ec 0c             	sub    $0xc,%esp
  1003c2:	68 ef 0f 10 00       	push   $0x100fef
  1003c7:	e8 6b 00 00 00       	call   100437 <terminal_writeIn>
  1003cc:	83 c4 10             	add    $0x10,%esp
  1003cf:	83 ec 0c             	sub    $0xc,%esp
  1003d2:	68 05 10 10 00       	push   $0x101005
  1003d7:	e8 5b 00 00 00       	call   100437 <terminal_writeIn>
  1003dc:	83 c4 10             	add    $0x10,%esp
  1003df:	83 ec 0c             	sub    $0xc,%esp
  1003e2:	68 13 10 10 00       	push   $0x101013
  1003e7:	e8 4b 00 00 00       	call   100437 <terminal_writeIn>
  1003ec:	83 c4 10             	add    $0x10,%esp
  1003ef:	83 ec 0c             	sub    $0xc,%esp
  1003f2:	68 28 10 10 00       	push   $0x101028
  1003f7:	e8 3b 00 00 00       	call   100437 <terminal_writeIn>
  1003fc:	83 c4 10             	add    $0x10,%esp
  1003ff:	83 ec 0c             	sub    $0xc,%esp
  100402:	68 45 10 10 00       	push   $0x101045
  100407:	e8 2b 00 00 00       	call   100437 <terminal_writeIn>
  10040c:	83 c4 10             	add    $0x10,%esp
  10040f:	90                   	nop
  100410:	c9                   	leave
  100411:	c3                   	ret

00100412 <terminal_write>:
  100412:	55                   	push   %ebp
  100413:	89 e5                	mov    %esp,%ebp
  100415:	83 ec 08             	sub    $0x8,%esp
  100418:	83 ec 0c             	sub    $0xc,%esp
  10041b:	ff 75 08             	push   0x8(%ebp)
  10041e:	e8 57 ff ff ff       	call   10037a <vga_write>
  100423:	83 c4 10             	add    $0x10,%esp
  100426:	83 ec 0c             	sub    $0xc,%esp
  100429:	ff 75 08             	push   0x8(%ebp)
  10042c:	e8 90 fd ff ff       	call   1001c1 <kputs>
  100431:	83 c4 10             	add    $0x10,%esp
  100434:	90                   	nop
  100435:	c9                   	leave
  100436:	c3                   	ret

00100437 <terminal_writeIn>:
  100437:	55                   	push   %ebp
  100438:	89 e5                	mov    %esp,%ebp
  10043a:	83 ec 08             	sub    $0x8,%esp
  10043d:	83 ec 0c             	sub    $0xc,%esp
  100440:	ff 75 08             	push   0x8(%ebp)
  100443:	e8 ca ff ff ff       	call   100412 <terminal_write>
  100448:	83 c4 10             	add    $0x10,%esp
  10044b:	83 ec 0c             	sub    $0xc,%esp
  10044e:	68 45 10 10 00       	push   $0x101045
  100453:	e8 ba ff ff ff       	call   100412 <terminal_write>
  100458:	83 c4 10             	add    $0x10,%esp
  10045b:	90                   	nop
  10045c:	c9                   	leave
  10045d:	c3                   	ret

0010045e <terminal_prompt>:
  10045e:	55                   	push   %ebp
  10045f:	89 e5                	mov    %esp,%ebp
  100461:	83 ec 08             	sub    $0x8,%esp
  100464:	83 ec 0c             	sub    $0xc,%esp
  100467:	68 47 10 10 00       	push   $0x101047
  10046c:	e8 a1 ff ff ff       	call   100412 <terminal_write>
  100471:	83 c4 10             	add    $0x10,%esp
  100474:	90                   	nop
  100475:	c9                   	leave
  100476:	c3                   	ret

00100477 <terminal_clear>:
  100477:	55                   	push   %ebp
  100478:	89 e5                	mov    %esp,%ebp
  10047a:	83 ec 08             	sub    $0x8,%esp
  10047d:	e8 b5 fd ff ff       	call   100237 <vga_clear>
  100482:	90                   	nop
  100483:	c9                   	leave
  100484:	c3                   	ret

00100485 <inb>:
  100485:	55                   	push   %ebp
  100486:	89 e5                	mov    %esp,%ebp
  100488:	83 ec 14             	sub    $0x14,%esp
  10048b:	8b 45 08             	mov    0x8(%ebp),%eax
  10048e:	66 89 45 ec          	mov    %ax,-0x14(%ebp)
  100492:	0f b7 45 ec          	movzwl -0x14(%ebp),%eax
  100496:	89 c2                	mov    %eax,%edx
  100498:	ec                   	in     (%dx),%al
  100499:	88 45 ff             	mov    %al,-0x1(%ebp)
  10049c:	0f b6 45 ff          	movzbl -0x1(%ebp),%eax
  1004a0:	c9                   	leave
  1004a1:	c3                   	ret

001004a2 <keyboard_getchar>:
  1004a2:	55                   	push   %ebp
  1004a3:	89 e5                	mov    %esp,%ebp
  1004a5:	83 ec 10             	sub    $0x10,%esp
  1004a8:	c6 45 ff 00          	movb   $0x0,-0x1(%ebp)
  1004ac:	90                   	nop
  1004ad:	6a 64                	push   $0x64
  1004af:	e8 d1 ff ff ff       	call   100485 <inb>
  1004b4:	83 c4 04             	add    $0x4,%esp
  1004b7:	0f b6 c0             	movzbl %al,%eax
  1004ba:	83 e0 01             	and    $0x1,%eax
  1004bd:	85 c0                	test   %eax,%eax
  1004bf:	74 ec                	je     1004ad <keyboard_getchar+0xb>
  1004c1:	6a 60                	push   $0x60
  1004c3:	e8 bd ff ff ff       	call   100485 <inb>
  1004c8:	83 c4 04             	add    $0x4,%esp
  1004cb:	88 45 ff             	mov    %al,-0x1(%ebp)
  1004ce:	0f b6 45 ff          	movzbl -0x1(%ebp),%eax
  1004d2:	84 c0                	test   %al,%al
  1004d4:	79 07                	jns    1004dd <keyboard_getchar+0x3b>
  1004d6:	b8 00 00 00 00       	mov    $0x0,%eax
  1004db:	eb 1a                	jmp    1004f7 <keyboard_getchar+0x55>
  1004dd:	0f b6 45 ff          	movzbl -0x1(%ebp),%eax
  1004e1:	84 c0                	test   %al,%al
  1004e3:	78 0d                	js     1004f2 <keyboard_getchar+0x50>
  1004e5:	0f b6 45 ff          	movzbl -0x1(%ebp),%eax
  1004e9:	0f b6 80 60 10 10 00 	movzbl 0x101060(%eax),%eax
  1004f0:	eb 05                	jmp    1004f7 <keyboard_getchar+0x55>
  1004f2:	b8 00 00 00 00       	mov    $0x0,%eax
  1004f7:	c9                   	leave
  1004f8:	c3                   	ret

001004f9 <keyboard_init>:
  1004f9:	55                   	push   %ebp
  1004fa:	89 e5                	mov    %esp,%ebp
  1004fc:	eb 0a                	jmp    100508 <keyboard_init+0xf>
  1004fe:	6a 60                	push   $0x60
  100500:	e8 80 ff ff ff       	call   100485 <inb>
  100505:	83 c4 04             	add    $0x4,%esp
  100508:	6a 64                	push   $0x64
  10050a:	e8 76 ff ff ff       	call   100485 <inb>
  10050f:	83 c4 04             	add    $0x4,%esp
  100512:	0f b6 c0             	movzbl %al,%eax
  100515:	83 e0 01             	and    $0x1,%eax
  100518:	85 c0                	test   %eax,%eax
  10051a:	75 e2                	jne    1004fe <keyboard_init+0x5>
  10051c:	90                   	nop
  10051d:	90                   	nop
  10051e:	c9                   	leave
  10051f:	c3                   	ret

00100520 <streq>:
  100520:	55                   	push   %ebp
  100521:	89 e5                	mov    %esp,%ebp
  100523:	eb 1f                	jmp    100544 <streq+0x24>
  100525:	8b 45 08             	mov    0x8(%ebp),%eax
  100528:	0f b6 10             	movzbl (%eax),%edx
  10052b:	8b 45 0c             	mov    0xc(%ebp),%eax
  10052e:	0f b6 00             	movzbl (%eax),%eax
  100531:	38 c2                	cmp    %al,%dl
  100533:	74 07                	je     10053c <streq+0x1c>
  100535:	b8 00 00 00 00       	mov    $0x0,%eax
  10053a:	eb 30                	jmp    10056c <streq+0x4c>
  10053c:	83 45 08 01          	addl   $0x1,0x8(%ebp)
  100540:	83 45 0c 01          	addl   $0x1,0xc(%ebp)
  100544:	8b 45 08             	mov    0x8(%ebp),%eax
  100547:	0f b6 00             	movzbl (%eax),%eax
  10054a:	84 c0                	test   %al,%al
  10054c:	74 0a                	je     100558 <streq+0x38>
  10054e:	8b 45 0c             	mov    0xc(%ebp),%eax
  100551:	0f b6 00             	movzbl (%eax),%eax
  100554:	84 c0                	test   %al,%al
  100556:	75 cd                	jne    100525 <streq+0x5>
  100558:	8b 45 08             	mov    0x8(%ebp),%eax
  10055b:	0f b6 10             	movzbl (%eax),%edx
  10055e:	8b 45 0c             	mov    0xc(%ebp),%eax
  100561:	0f b6 00             	movzbl (%eax),%eax
  100564:	38 c2                	cmp    %al,%dl
  100566:	0f 94 c0             	sete   %al
  100569:	0f b6 c0             	movzbl %al,%eax
  10056c:	5d                   	pop    %ebp
  10056d:	c3                   	ret

0010056e <starts_with>:
  10056e:	55                   	push   %ebp
  10056f:	89 e5                	mov    %esp,%ebp
  100571:	eb 1f                	jmp    100592 <starts_with+0x24>
  100573:	8b 45 08             	mov    0x8(%ebp),%eax
  100576:	0f b6 10             	movzbl (%eax),%edx
  100579:	8b 45 0c             	mov    0xc(%ebp),%eax
  10057c:	0f b6 00             	movzbl (%eax),%eax
  10057f:	38 c2                	cmp    %al,%dl
  100581:	74 07                	je     10058a <starts_with+0x1c>
  100583:	b8 00 00 00 00       	mov    $0x0,%eax
  100588:	eb 17                	jmp    1005a1 <starts_with+0x33>
  10058a:	83 45 08 01          	addl   $0x1,0x8(%ebp)
  10058e:	83 45 0c 01          	addl   $0x1,0xc(%ebp)
  100592:	8b 45 0c             	mov    0xc(%ebp),%eax
  100595:	0f b6 00             	movzbl (%eax),%eax
  100598:	84 c0                	test   %al,%al
  10059a:	75 d7                	jne    100573 <starts_with+0x5>
  10059c:	b8 01 00 00 00       	mov    $0x1,%eax
  1005a1:	5d                   	pop    %ebp
  1005a2:	c3                   	ret

001005a3 <shell_execute>:
  1005a3:	55                   	push   %ebp
  1005a4:	89 e5                	mov    %esp,%ebp
  1005a6:	83 ec 08             	sub    $0x8,%esp
  1005a9:	8b 45 08             	mov    0x8(%ebp),%eax
  1005ac:	0f b6 00             	movzbl (%eax),%eax
  1005af:	84 c0                	test   %al,%al
  1005b1:	0f 84 96 01 00 00    	je     10074d <shell_execute+0x1aa>
  1005b7:	83 ec 0c             	sub    $0xc,%esp
  1005ba:	68 e0 10 10 00       	push   $0x1010e0
  1005bf:	e8 73 fe ff ff       	call   100437 <terminal_writeIn>
  1005c4:	83 c4 10             	add    $0x10,%esp
  1005c7:	83 ec 08             	sub    $0x8,%esp
  1005ca:	68 e1 10 10 00       	push   $0x1010e1
  1005cf:	ff 75 08             	push   0x8(%ebp)
  1005d2:	e8 49 ff ff ff       	call   100520 <streq>
  1005d7:	83 c4 10             	add    $0x10,%esp
  1005da:	85 c0                	test   %eax,%eax
  1005dc:	74 75                	je     100653 <shell_execute+0xb0>
  1005de:	83 ec 0c             	sub    $0xc,%esp
  1005e1:	68 e6 10 10 00       	push   $0x1010e6
  1005e6:	e8 4c fe ff ff       	call   100437 <terminal_writeIn>
  1005eb:	83 c4 10             	add    $0x10,%esp
  1005ee:	83 ec 0c             	sub    $0xc,%esp
  1005f1:	68 e1 10 10 00       	push   $0x1010e1
  1005f6:	e8 3c fe ff ff       	call   100437 <terminal_writeIn>
  1005fb:	83 c4 10             	add    $0x10,%esp
  1005fe:	83 ec 0c             	sub    $0xc,%esp
  100601:	68 fb 10 10 00       	push   $0x1010fb
  100606:	e8 2c fe ff ff       	call   100437 <terminal_writeIn>
  10060b:	83 c4 10             	add    $0x10,%esp
  10060e:	83 ec 0c             	sub    $0xc,%esp
  100611:	68 01 11 10 00       	push   $0x101101
  100616:	e8 1c fe ff ff       	call   100437 <terminal_writeIn>
  10061b:	83 c4 10             	add    $0x10,%esp
  10061e:	83 ec 0c             	sub    $0xc,%esp
  100621:	68 09 11 10 00       	push   $0x101109
  100626:	e8 0c fe ff ff       	call   100437 <terminal_writeIn>
  10062b:	83 c4 10             	add    $0x10,%esp
  10062e:	83 ec 0c             	sub    $0xc,%esp
  100631:	68 18 11 10 00       	push   $0x101118
  100636:	e8 fc fd ff ff       	call   100437 <terminal_writeIn>
  10063b:	83 c4 10             	add    $0x10,%esp
  10063e:	83 ec 0c             	sub    $0xc,%esp
  100641:	68 1f 11 10 00       	push   $0x10111f
  100646:	e8 ec fd ff ff       	call   100437 <terminal_writeIn>
  10064b:	83 c4 10             	add    $0x10,%esp
  10064e:	e9 fb 00 00 00       	jmp    10074e <shell_execute+0x1ab>
  100653:	83 ec 08             	sub    $0x8,%esp
  100656:	68 fb 10 10 00       	push   $0x1010fb
  10065b:	ff 75 08             	push   0x8(%ebp)
  10065e:	e8 bd fe ff ff       	call   100520 <streq>
  100663:	83 c4 10             	add    $0x10,%esp
  100666:	85 c0                	test   %eax,%eax
  100668:	74 25                	je     10068f <shell_execute+0xec>
  10066a:	83 ec 0c             	sub    $0xc,%esp
  10066d:	68 25 11 10 00       	push   $0x101125
  100672:	e8 c0 fd ff ff       	call   100437 <terminal_writeIn>
  100677:	83 c4 10             	add    $0x10,%esp
  10067a:	83 ec 0c             	sub    $0xc,%esp
  10067d:	68 30 11 10 00       	push   $0x101130
  100682:	e8 b0 fd ff ff       	call   100437 <terminal_writeIn>
  100687:	83 c4 10             	add    $0x10,%esp
  10068a:	e9 bf 00 00 00       	jmp    10074e <shell_execute+0x1ab>
  10068f:	83 ec 08             	sub    $0x8,%esp
  100692:	68 01 11 10 00       	push   $0x101101
  100697:	ff 75 08             	push   0x8(%ebp)
  10069a:	e8 81 fe ff ff       	call   100520 <streq>
  10069f:	83 c4 10             	add    $0x10,%esp
  1006a2:	85 c0                	test   %eax,%eax
  1006a4:	74 15                	je     1006bb <shell_execute+0x118>
  1006a6:	83 ec 0c             	sub    $0xc,%esp
  1006a9:	68 56 11 10 00       	push   $0x101156
  1006ae:	e8 84 fd ff ff       	call   100437 <terminal_writeIn>
  1006b3:	83 c4 10             	add    $0x10,%esp
  1006b6:	e9 93 00 00 00       	jmp    10074e <shell_execute+0x1ab>
  1006bb:	83 ec 08             	sub    $0x8,%esp
  1006be:	68 63 11 10 00       	push   $0x101163
  1006c3:	ff 75 08             	push   0x8(%ebp)
  1006c6:	e8 a3 fe ff ff       	call   10056e <starts_with>
  1006cb:	83 c4 10             	add    $0x10,%esp
  1006ce:	85 c0                	test   %eax,%eax
  1006d0:	74 14                	je     1006e6 <shell_execute+0x143>
  1006d2:	8b 45 08             	mov    0x8(%ebp),%eax
  1006d5:	83 c0 05             	add    $0x5,%eax
  1006d8:	83 ec 0c             	sub    $0xc,%esp
  1006db:	50                   	push   %eax
  1006dc:	e8 56 fd ff ff       	call   100437 <terminal_writeIn>
  1006e1:	83 c4 10             	add    $0x10,%esp
  1006e4:	eb 68                	jmp    10074e <shell_execute+0x1ab>
  1006e6:	83 ec 08             	sub    $0x8,%esp
  1006e9:	68 1f 11 10 00       	push   $0x10111f
  1006ee:	ff 75 08             	push   0x8(%ebp)
  1006f1:	e8 2a fe ff ff       	call   100520 <streq>
  1006f6:	83 c4 10             	add    $0x10,%esp
  1006f9:	85 c0                	test   %eax,%eax
  1006fb:	74 07                	je     100704 <shell_execute+0x161>
  1006fd:	e8 75 fd ff ff       	call   100477 <terminal_clear>
  100702:	eb 4a                	jmp    10074e <shell_execute+0x1ab>
  100704:	83 ec 08             	sub    $0x8,%esp
  100707:	68 18 11 10 00       	push   $0x101118
  10070c:	ff 75 08             	push   0x8(%ebp)
  10070f:	e8 0c fe ff ff       	call   100520 <streq>
  100714:	83 c4 10             	add    $0x10,%esp
  100717:	85 c0                	test   %eax,%eax
  100719:	74 12                	je     10072d <shell_execute+0x18a>
  10071b:	83 ec 0c             	sub    $0xc,%esp
  10071e:	68 6c 11 10 00       	push   $0x10116c
  100723:	e8 0f fd ff ff       	call   100437 <terminal_writeIn>
  100728:	83 c4 10             	add    $0x10,%esp
  10072b:	eb 21                	jmp    10074e <shell_execute+0x1ab>
  10072d:	83 ec 0c             	sub    $0xc,%esp
  100730:	68 8f 11 10 00       	push   $0x10118f
  100735:	e8 d8 fc ff ff       	call   100412 <terminal_write>
  10073a:	83 c4 10             	add    $0x10,%esp
  10073d:	83 ec 0c             	sub    $0xc,%esp
  100740:	ff 75 08             	push   0x8(%ebp)
  100743:	e8 ef fc ff ff       	call   100437 <terminal_writeIn>
  100748:	83 c4 10             	add    $0x10,%esp
  10074b:	eb 01                	jmp    10074e <shell_execute+0x1ab>
  10074d:	90                   	nop
  10074e:	c9                   	leave
  10074f:	c3                   	ret

00100750 <idt_set_gate>:
  100750:	55                   	push   %ebp
  100751:	89 e5                	mov    %esp,%ebp
  100753:	83 ec 0c             	sub    $0xc,%esp
  100756:	8b 4d 08             	mov    0x8(%ebp),%ecx
  100759:	8b 55 10             	mov    0x10(%ebp),%edx
  10075c:	8b 45 14             	mov    0x14(%ebp),%eax
  10075f:	88 4d fc             	mov    %cl,-0x4(%ebp)
  100762:	66 89 55 f8          	mov    %dx,-0x8(%ebp)
  100766:	88 45 f4             	mov    %al,-0xc(%ebp)
  100769:	0f b6 45 fc          	movzbl -0x4(%ebp),%eax
  10076d:	8b 55 0c             	mov    0xc(%ebp),%edx
  100770:	66 89 14 c5 20 70 10 	mov    %dx,0x107020(,%eax,8)
  100777:	00 
  100778:	0f b6 45 fc          	movzbl -0x4(%ebp),%eax
  10077c:	0f b7 55 f8          	movzwl -0x8(%ebp),%edx
  100780:	66 89 14 c5 22 70 10 	mov    %dx,0x107022(,%eax,8)
  100787:	00 
  100788:	0f b6 45 fc          	movzbl -0x4(%ebp),%eax
  10078c:	c6 04 c5 24 70 10 00 	movb   $0x0,0x107024(,%eax,8)
  100793:	00 
  100794:	0f b6 45 fc          	movzbl -0x4(%ebp),%eax
  100798:	0f b6 55 f4          	movzbl -0xc(%ebp),%edx
  10079c:	88 14 c5 25 70 10 00 	mov    %dl,0x107025(,%eax,8)
  1007a3:	8b 45 0c             	mov    0xc(%ebp),%eax
  1007a6:	c1 e8 10             	shr    $0x10,%eax
  1007a9:	89 c2                	mov    %eax,%edx
  1007ab:	0f b6 45 fc          	movzbl -0x4(%ebp),%eax
  1007af:	66 89 14 c5 26 70 10 	mov    %dx,0x107026(,%eax,8)
  1007b6:	00 
  1007b7:	90                   	nop
  1007b8:	c9                   	leave
  1007b9:	c3                   	ret

001007ba <idt_load>:
  1007ba:	55                   	push   %ebp
  1007bb:	89 e5                	mov    %esp,%ebp
  1007bd:	8b 45 08             	mov    0x8(%ebp),%eax
  1007c0:	0f 01 18             	lidtl  (%eax)
  1007c3:	90                   	nop
  1007c4:	5d                   	pop    %ebp
  1007c5:	c3                   	ret

001007c6 <idt_init>:
  1007c6:	55                   	push   %ebp
  1007c7:	89 e5                	mov    %esp,%ebp
  1007c9:	83 ec 10             	sub    $0x10,%esp
  1007cc:	66 c7 45 fa ff 07    	movw   $0x7ff,-0x6(%ebp)
  1007d2:	b8 20 70 10 00       	mov    $0x107020,%eax
  1007d7:	89 45 fc             	mov    %eax,-0x4(%ebp)
  1007da:	b8 8e 0c 10 00       	mov    $0x100c8e,%eax
  1007df:	68 8e 00 00 00       	push   $0x8e
  1007e4:	6a 08                	push   $0x8
  1007e6:	50                   	push   %eax
  1007e7:	6a 00                	push   $0x0
  1007e9:	e8 62 ff ff ff       	call   100750 <idt_set_gate>
  1007ee:	83 c4 10             	add    $0x10,%esp
  1007f1:	b8 95 0c 10 00       	mov    $0x100c95,%eax
  1007f6:	68 8e 00 00 00       	push   $0x8e
  1007fb:	6a 08                	push   $0x8
  1007fd:	50                   	push   %eax
  1007fe:	6a 01                	push   $0x1
  100800:	e8 4b ff ff ff       	call   100750 <idt_set_gate>
  100805:	83 c4 10             	add    $0x10,%esp
  100808:	b8 9c 0c 10 00       	mov    $0x100c9c,%eax
  10080d:	68 8e 00 00 00       	push   $0x8e
  100812:	6a 08                	push   $0x8
  100814:	50                   	push   %eax
  100815:	6a 02                	push   $0x2
  100817:	e8 34 ff ff ff       	call   100750 <idt_set_gate>
  10081c:	83 c4 10             	add    $0x10,%esp
  10081f:	b8 a3 0c 10 00       	mov    $0x100ca3,%eax
  100824:	68 8e 00 00 00       	push   $0x8e
  100829:	6a 08                	push   $0x8
  10082b:	50                   	push   %eax
  10082c:	6a 03                	push   $0x3
  10082e:	e8 1d ff ff ff       	call   100750 <idt_set_gate>
  100833:	83 c4 10             	add    $0x10,%esp
  100836:	b8 aa 0c 10 00       	mov    $0x100caa,%eax
  10083b:	68 8e 00 00 00       	push   $0x8e
  100840:	6a 08                	push   $0x8
  100842:	50                   	push   %eax
  100843:	6a 04                	push   $0x4
  100845:	e8 06 ff ff ff       	call   100750 <idt_set_gate>
  10084a:	83 c4 10             	add    $0x10,%esp
  10084d:	b8 b1 0c 10 00       	mov    $0x100cb1,%eax
  100852:	68 8e 00 00 00       	push   $0x8e
  100857:	6a 08                	push   $0x8
  100859:	50                   	push   %eax
  10085a:	6a 05                	push   $0x5
  10085c:	e8 ef fe ff ff       	call   100750 <idt_set_gate>
  100861:	83 c4 10             	add    $0x10,%esp
  100864:	b8 b8 0c 10 00       	mov    $0x100cb8,%eax
  100869:	68 8e 00 00 00       	push   $0x8e
  10086e:	6a 08                	push   $0x8
  100870:	50                   	push   %eax
  100871:	6a 06                	push   $0x6
  100873:	e8 d8 fe ff ff       	call   100750 <idt_set_gate>
  100878:	83 c4 10             	add    $0x10,%esp
  10087b:	b8 bf 0c 10 00       	mov    $0x100cbf,%eax
  100880:	68 8e 00 00 00       	push   $0x8e
  100885:	6a 08                	push   $0x8
  100887:	50                   	push   %eax
  100888:	6a 07                	push   $0x7
  10088a:	e8 c1 fe ff ff       	call   100750 <idt_set_gate>
  10088f:	83 c4 10             	add    $0x10,%esp
  100892:	b8 c9 0c 10 00       	mov    $0x100cc9,%eax
  100897:	68 8e 00 00 00       	push   $0x8e
  10089c:	6a 08                	push   $0x8
  10089e:	50                   	push   %eax
  10089f:	6a 08                	push   $0x8
  1008a1:	e8 aa fe ff ff       	call   100750 <idt_set_gate>
  1008a6:	83 c4 10             	add    $0x10,%esp
  1008a9:	b8 d1 0c 10 00       	mov    $0x100cd1,%eax
  1008ae:	68 8e 00 00 00       	push   $0x8e
  1008b3:	6a 08                	push   $0x8
  1008b5:	50                   	push   %eax
  1008b6:	6a 09                	push   $0x9
  1008b8:	e8 93 fe ff ff       	call   100750 <idt_set_gate>
  1008bd:	83 c4 10             	add    $0x10,%esp
  1008c0:	b8 db 0c 10 00       	mov    $0x100cdb,%eax
  1008c5:	68 8e 00 00 00       	push   $0x8e
  1008ca:	6a 08                	push   $0x8
  1008cc:	50                   	push   %eax
  1008cd:	6a 0a                	push   $0xa
  1008cf:	e8 7c fe ff ff       	call   100750 <idt_set_gate>
  1008d4:	83 c4 10             	add    $0x10,%esp
  1008d7:	b8 e3 0c 10 00       	mov    $0x100ce3,%eax
  1008dc:	68 8e 00 00 00       	push   $0x8e
  1008e1:	6a 08                	push   $0x8
  1008e3:	50                   	push   %eax
  1008e4:	6a 0b                	push   $0xb
  1008e6:	e8 65 fe ff ff       	call   100750 <idt_set_gate>
  1008eb:	83 c4 10             	add    $0x10,%esp
  1008ee:	b8 eb 0c 10 00       	mov    $0x100ceb,%eax
  1008f3:	68 8e 00 00 00       	push   $0x8e
  1008f8:	6a 08                	push   $0x8
  1008fa:	50                   	push   %eax
  1008fb:	6a 0c                	push   $0xc
  1008fd:	e8 4e fe ff ff       	call   100750 <idt_set_gate>
  100902:	83 c4 10             	add    $0x10,%esp
  100905:	b8 f3 0c 10 00       	mov    $0x100cf3,%eax
  10090a:	68 8e 00 00 00       	push   $0x8e
  10090f:	6a 08                	push   $0x8
  100911:	50                   	push   %eax
  100912:	6a 0d                	push   $0xd
  100914:	e8 37 fe ff ff       	call   100750 <idt_set_gate>
  100919:	83 c4 10             	add    $0x10,%esp
  10091c:	b8 fb 0c 10 00       	mov    $0x100cfb,%eax
  100921:	68 8e 00 00 00       	push   $0x8e
  100926:	6a 08                	push   $0x8
  100928:	50                   	push   %eax
  100929:	6a 0e                	push   $0xe
  10092b:	e8 20 fe ff ff       	call   100750 <idt_set_gate>
  100930:	83 c4 10             	add    $0x10,%esp
  100933:	b8 03 0d 10 00       	mov    $0x100d03,%eax
  100938:	68 8e 00 00 00       	push   $0x8e
  10093d:	6a 08                	push   $0x8
  10093f:	50                   	push   %eax
  100940:	6a 0f                	push   $0xf
  100942:	e8 09 fe ff ff       	call   100750 <idt_set_gate>
  100947:	83 c4 10             	add    $0x10,%esp
  10094a:	b8 0d 0d 10 00       	mov    $0x100d0d,%eax
  10094f:	68 8e 00 00 00       	push   $0x8e
  100954:	6a 08                	push   $0x8
  100956:	50                   	push   %eax
  100957:	6a 10                	push   $0x10
  100959:	e8 f2 fd ff ff       	call   100750 <idt_set_gate>
  10095e:	83 c4 10             	add    $0x10,%esp
  100961:	b8 17 0d 10 00       	mov    $0x100d17,%eax
  100966:	68 8e 00 00 00       	push   $0x8e
  10096b:	6a 08                	push   $0x8
  10096d:	50                   	push   %eax
  10096e:	6a 11                	push   $0x11
  100970:	e8 db fd ff ff       	call   100750 <idt_set_gate>
  100975:	83 c4 10             	add    $0x10,%esp
  100978:	b8 1f 0d 10 00       	mov    $0x100d1f,%eax
  10097d:	68 8e 00 00 00       	push   $0x8e
  100982:	6a 08                	push   $0x8
  100984:	50                   	push   %eax
  100985:	6a 12                	push   $0x12
  100987:	e8 c4 fd ff ff       	call   100750 <idt_set_gate>
  10098c:	83 c4 10             	add    $0x10,%esp
  10098f:	b8 29 0d 10 00       	mov    $0x100d29,%eax
  100994:	68 8e 00 00 00       	push   $0x8e
  100999:	6a 08                	push   $0x8
  10099b:	50                   	push   %eax
  10099c:	6a 13                	push   $0x13
  10099e:	e8 ad fd ff ff       	call   100750 <idt_set_gate>
  1009a3:	83 c4 10             	add    $0x10,%esp
  1009a6:	b8 33 0d 10 00       	mov    $0x100d33,%eax
  1009ab:	68 8e 00 00 00       	push   $0x8e
  1009b0:	6a 08                	push   $0x8
  1009b2:	50                   	push   %eax
  1009b3:	6a 14                	push   $0x14
  1009b5:	e8 96 fd ff ff       	call   100750 <idt_set_gate>
  1009ba:	83 c4 10             	add    $0x10,%esp
  1009bd:	b8 3d 0d 10 00       	mov    $0x100d3d,%eax
  1009c2:	68 8e 00 00 00       	push   $0x8e
  1009c7:	6a 08                	push   $0x8
  1009c9:	50                   	push   %eax
  1009ca:	6a 15                	push   $0x15
  1009cc:	e8 7f fd ff ff       	call   100750 <idt_set_gate>
  1009d1:	83 c4 10             	add    $0x10,%esp
  1009d4:	b8 47 0d 10 00       	mov    $0x100d47,%eax
  1009d9:	68 8e 00 00 00       	push   $0x8e
  1009de:	6a 08                	push   $0x8
  1009e0:	50                   	push   %eax
  1009e1:	6a 16                	push   $0x16
  1009e3:	e8 68 fd ff ff       	call   100750 <idt_set_gate>
  1009e8:	83 c4 10             	add    $0x10,%esp
  1009eb:	b8 51 0d 10 00       	mov    $0x100d51,%eax
  1009f0:	68 8e 00 00 00       	push   $0x8e
  1009f5:	6a 08                	push   $0x8
  1009f7:	50                   	push   %eax
  1009f8:	6a 17                	push   $0x17
  1009fa:	e8 51 fd ff ff       	call   100750 <idt_set_gate>
  1009ff:	83 c4 10             	add    $0x10,%esp
  100a02:	b8 5b 0d 10 00       	mov    $0x100d5b,%eax
  100a07:	68 8e 00 00 00       	push   $0x8e
  100a0c:	6a 08                	push   $0x8
  100a0e:	50                   	push   %eax
  100a0f:	6a 18                	push   $0x18
  100a11:	e8 3a fd ff ff       	call   100750 <idt_set_gate>
  100a16:	83 c4 10             	add    $0x10,%esp
  100a19:	b8 65 0d 10 00       	mov    $0x100d65,%eax
  100a1e:	68 8e 00 00 00       	push   $0x8e
  100a23:	6a 08                	push   $0x8
  100a25:	50                   	push   %eax
  100a26:	6a 19                	push   $0x19
  100a28:	e8 23 fd ff ff       	call   100750 <idt_set_gate>
  100a2d:	83 c4 10             	add    $0x10,%esp
  100a30:	b8 6f 0d 10 00       	mov    $0x100d6f,%eax
  100a35:	68 8e 00 00 00       	push   $0x8e
  100a3a:	6a 08                	push   $0x8
  100a3c:	50                   	push   %eax
  100a3d:	6a 1a                	push   $0x1a
  100a3f:	e8 0c fd ff ff       	call   100750 <idt_set_gate>
  100a44:	83 c4 10             	add    $0x10,%esp
  100a47:	b8 79 0d 10 00       	mov    $0x100d79,%eax
  100a4c:	68 8e 00 00 00       	push   $0x8e
  100a51:	6a 08                	push   $0x8
  100a53:	50                   	push   %eax
  100a54:	6a 1b                	push   $0x1b
  100a56:	e8 f5 fc ff ff       	call   100750 <idt_set_gate>
  100a5b:	83 c4 10             	add    $0x10,%esp
  100a5e:	b8 83 0d 10 00       	mov    $0x100d83,%eax
  100a63:	68 8e 00 00 00       	push   $0x8e
  100a68:	6a 08                	push   $0x8
  100a6a:	50                   	push   %eax
  100a6b:	6a 1c                	push   $0x1c
  100a6d:	e8 de fc ff ff       	call   100750 <idt_set_gate>
  100a72:	83 c4 10             	add    $0x10,%esp
  100a75:	b8 8d 0d 10 00       	mov    $0x100d8d,%eax
  100a7a:	68 8e 00 00 00       	push   $0x8e
  100a7f:	6a 08                	push   $0x8
  100a81:	50                   	push   %eax
  100a82:	6a 1d                	push   $0x1d
  100a84:	e8 c7 fc ff ff       	call   100750 <idt_set_gate>
  100a89:	83 c4 10             	add    $0x10,%esp
  100a8c:	b8 97 0d 10 00       	mov    $0x100d97,%eax
  100a91:	68 8e 00 00 00       	push   $0x8e
  100a96:	6a 08                	push   $0x8
  100a98:	50                   	push   %eax
  100a99:	6a 1e                	push   $0x1e
  100a9b:	e8 b0 fc ff ff       	call   100750 <idt_set_gate>
  100aa0:	83 c4 10             	add    $0x10,%esp
  100aa3:	b8 a1 0d 10 00       	mov    $0x100da1,%eax
  100aa8:	68 8e 00 00 00       	push   $0x8e
  100aad:	6a 08                	push   $0x8
  100aaf:	50                   	push   %eax
  100ab0:	6a 1f                	push   $0x1f
  100ab2:	e8 99 fc ff ff       	call   100750 <idt_set_gate>
  100ab7:	83 c4 10             	add    $0x10,%esp
  100aba:	b8 ab 0d 10 00       	mov    $0x100dab,%eax
  100abf:	68 8e 00 00 00       	push   $0x8e
  100ac4:	6a 08                	push   $0x8
  100ac6:	50                   	push   %eax
  100ac7:	6a 20                	push   $0x20
  100ac9:	e8 82 fc ff ff       	call   100750 <idt_set_gate>
  100ace:	83 c4 10             	add    $0x10,%esp
  100ad1:	b8 b5 0d 10 00       	mov    $0x100db5,%eax
  100ad6:	68 8e 00 00 00       	push   $0x8e
  100adb:	6a 08                	push   $0x8
  100add:	50                   	push   %eax
  100ade:	6a 21                	push   $0x21
  100ae0:	e8 6b fc ff ff       	call   100750 <idt_set_gate>
  100ae5:	83 c4 10             	add    $0x10,%esp
  100ae8:	b8 bf 0d 10 00       	mov    $0x100dbf,%eax
  100aed:	68 8e 00 00 00       	push   $0x8e
  100af2:	6a 08                	push   $0x8
  100af4:	50                   	push   %eax
  100af5:	6a 22                	push   $0x22
  100af7:	e8 54 fc ff ff       	call   100750 <idt_set_gate>
  100afc:	83 c4 10             	add    $0x10,%esp
  100aff:	b8 c9 0d 10 00       	mov    $0x100dc9,%eax
  100b04:	68 8e 00 00 00       	push   $0x8e
  100b09:	6a 08                	push   $0x8
  100b0b:	50                   	push   %eax
  100b0c:	6a 23                	push   $0x23
  100b0e:	e8 3d fc ff ff       	call   100750 <idt_set_gate>
  100b13:	83 c4 10             	add    $0x10,%esp
  100b16:	b8 d3 0d 10 00       	mov    $0x100dd3,%eax
  100b1b:	68 8e 00 00 00       	push   $0x8e
  100b20:	6a 08                	push   $0x8
  100b22:	50                   	push   %eax
  100b23:	6a 24                	push   $0x24
  100b25:	e8 26 fc ff ff       	call   100750 <idt_set_gate>
  100b2a:	83 c4 10             	add    $0x10,%esp
  100b2d:	b8 dd 0d 10 00       	mov    $0x100ddd,%eax
  100b32:	68 8e 00 00 00       	push   $0x8e
  100b37:	6a 08                	push   $0x8
  100b39:	50                   	push   %eax
  100b3a:	6a 25                	push   $0x25
  100b3c:	e8 0f fc ff ff       	call   100750 <idt_set_gate>
  100b41:	83 c4 10             	add    $0x10,%esp
  100b44:	b8 e7 0d 10 00       	mov    $0x100de7,%eax
  100b49:	68 8e 00 00 00       	push   $0x8e
  100b4e:	6a 08                	push   $0x8
  100b50:	50                   	push   %eax
  100b51:	6a 26                	push   $0x26
  100b53:	e8 f8 fb ff ff       	call   100750 <idt_set_gate>
  100b58:	83 c4 10             	add    $0x10,%esp
  100b5b:	b8 f1 0d 10 00       	mov    $0x100df1,%eax
  100b60:	68 8e 00 00 00       	push   $0x8e
  100b65:	6a 08                	push   $0x8
  100b67:	50                   	push   %eax
  100b68:	6a 27                	push   $0x27
  100b6a:	e8 e1 fb ff ff       	call   100750 <idt_set_gate>
  100b6f:	83 c4 10             	add    $0x10,%esp
  100b72:	b8 fb 0d 10 00       	mov    $0x100dfb,%eax
  100b77:	68 8e 00 00 00       	push   $0x8e
  100b7c:	6a 08                	push   $0x8
  100b7e:	50                   	push   %eax
  100b7f:	6a 28                	push   $0x28
  100b81:	e8 ca fb ff ff       	call   100750 <idt_set_gate>
  100b86:	83 c4 10             	add    $0x10,%esp
  100b89:	b8 05 0e 10 00       	mov    $0x100e05,%eax
  100b8e:	68 8e 00 00 00       	push   $0x8e
  100b93:	6a 08                	push   $0x8
  100b95:	50                   	push   %eax
  100b96:	6a 29                	push   $0x29
  100b98:	e8 b3 fb ff ff       	call   100750 <idt_set_gate>
  100b9d:	83 c4 10             	add    $0x10,%esp
  100ba0:	b8 0f 0e 10 00       	mov    $0x100e0f,%eax
  100ba5:	68 8e 00 00 00       	push   $0x8e
  100baa:	6a 08                	push   $0x8
  100bac:	50                   	push   %eax
  100bad:	6a 2a                	push   $0x2a
  100baf:	e8 9c fb ff ff       	call   100750 <idt_set_gate>
  100bb4:	83 c4 10             	add    $0x10,%esp
  100bb7:	b8 19 0e 10 00       	mov    $0x100e19,%eax
  100bbc:	68 8e 00 00 00       	push   $0x8e
  100bc1:	6a 08                	push   $0x8
  100bc3:	50                   	push   %eax
  100bc4:	6a 2b                	push   $0x2b
  100bc6:	e8 85 fb ff ff       	call   100750 <idt_set_gate>
  100bcb:	83 c4 10             	add    $0x10,%esp
  100bce:	b8 23 0e 10 00       	mov    $0x100e23,%eax
  100bd3:	68 8e 00 00 00       	push   $0x8e
  100bd8:	6a 08                	push   $0x8
  100bda:	50                   	push   %eax
  100bdb:	6a 2c                	push   $0x2c
  100bdd:	e8 6e fb ff ff       	call   100750 <idt_set_gate>
  100be2:	83 c4 10             	add    $0x10,%esp
  100be5:	b8 2d 0e 10 00       	mov    $0x100e2d,%eax
  100bea:	68 8e 00 00 00       	push   $0x8e
  100bef:	6a 08                	push   $0x8
  100bf1:	50                   	push   %eax
  100bf2:	6a 2d                	push   $0x2d
  100bf4:	e8 57 fb ff ff       	call   100750 <idt_set_gate>
  100bf9:	83 c4 10             	add    $0x10,%esp
  100bfc:	b8 37 0e 10 00       	mov    $0x100e37,%eax
  100c01:	68 8e 00 00 00       	push   $0x8e
  100c06:	6a 08                	push   $0x8
  100c08:	50                   	push   %eax
  100c09:	6a 2e                	push   $0x2e
  100c0b:	e8 40 fb ff ff       	call   100750 <idt_set_gate>
  100c10:	83 c4 10             	add    $0x10,%esp
  100c13:	b8 41 0e 10 00       	mov    $0x100e41,%eax
  100c18:	68 8e 00 00 00       	push   $0x8e
  100c1d:	6a 08                	push   $0x8
  100c1f:	50                   	push   %eax
  100c20:	6a 2f                	push   $0x2f
  100c22:	e8 29 fb ff ff       	call   100750 <idt_set_gate>
  100c27:	83 c4 10             	add    $0x10,%esp
  100c2a:	8d 45 fa             	lea    -0x6(%ebp),%eax
  100c2d:	50                   	push   %eax
  100c2e:	e8 87 fb ff ff       	call   1007ba <idt_load>
  100c33:	83 c4 04             	add    $0x4,%esp
  100c36:	90                   	nop
  100c37:	c9                   	leave
  100c38:	c3                   	ret
  100c39:	66 90                	xchg   %ax,%ax
  100c3b:	66 90                	xchg   %ax,%ax
  100c3d:	66 90                	xchg   %ax,%ax
  100c3f:	90                   	nop

00100c40 <isr_common>:
  100c40:	60                   	pusha
  100c41:	1e                   	push   %ds
  100c42:	06                   	push   %es
  100c43:	0f a0                	push   %fs
  100c45:	0f a8                	push   %gs
  100c47:	66 b8 10 00          	mov    $0x10,%ax
  100c4b:	8e d8                	mov    %eax,%ds
  100c4d:	8e c0                	mov    %eax,%es
  100c4f:	8e e0                	mov    %eax,%fs
  100c51:	8e e8                	mov    %eax,%gs
  100c53:	54                   	push   %esp
  100c54:	e8 69 02 00 00       	call   100ec2 <exception_handler>
  100c59:	83 c4 04             	add    $0x4,%esp
  100c5c:	0f a9                	pop    %gs
  100c5e:	0f a1                	pop    %fs
  100c60:	07                   	pop    %es
  100c61:	1f                   	pop    %ds
  100c62:	61                   	popa
  100c63:	83 c4 08             	add    $0x8,%esp
  100c66:	cf                   	iret

00100c67 <irq_common>:
  100c67:	60                   	pusha
  100c68:	1e                   	push   %ds
  100c69:	06                   	push   %es
  100c6a:	0f a0                	push   %fs
  100c6c:	0f a8                	push   %gs
  100c6e:	66 b8 10 00          	mov    $0x10,%ax
  100c72:	8e d8                	mov    %eax,%ds
  100c74:	8e c0                	mov    %eax,%es
  100c76:	8e e0                	mov    %eax,%fs
  100c78:	8e e8                	mov    %eax,%gs
  100c7a:	54                   	push   %esp
  100c7b:	e8 24 03 00 00       	call   100fa4 <irq_handler>
  100c80:	83 c4 04             	add    $0x4,%esp
  100c83:	0f a9                	pop    %gs
  100c85:	0f a1                	pop    %fs
  100c87:	07                   	pop    %es
  100c88:	1f                   	pop    %ds
  100c89:	61                   	popa
  100c8a:	83 c4 08             	add    $0x8,%esp
  100c8d:	cf                   	iret

00100c8e <isr_stub_0>:
  100c8e:	fa                   	cli
  100c8f:	6a 00                	push   $0x0
  100c91:	6a 00                	push   $0x0
  100c93:	eb ab                	jmp    100c40 <isr_common>

00100c95 <isr_stub_1>:
  100c95:	fa                   	cli
  100c96:	6a 00                	push   $0x0
  100c98:	6a 01                	push   $0x1
  100c9a:	eb a4                	jmp    100c40 <isr_common>

00100c9c <isr_stub_2>:
  100c9c:	fa                   	cli
  100c9d:	6a 00                	push   $0x0
  100c9f:	6a 02                	push   $0x2
  100ca1:	eb 9d                	jmp    100c40 <isr_common>

00100ca3 <isr_stub_3>:
  100ca3:	fa                   	cli
  100ca4:	6a 00                	push   $0x0
  100ca6:	6a 03                	push   $0x3
  100ca8:	eb 96                	jmp    100c40 <isr_common>

00100caa <isr_stub_4>:
  100caa:	fa                   	cli
  100cab:	6a 00                	push   $0x0
  100cad:	6a 04                	push   $0x4
  100caf:	eb 8f                	jmp    100c40 <isr_common>

00100cb1 <isr_stub_5>:
  100cb1:	fa                   	cli
  100cb2:	6a 00                	push   $0x0
  100cb4:	6a 05                	push   $0x5
  100cb6:	eb 88                	jmp    100c40 <isr_common>

00100cb8 <isr_stub_6>:
  100cb8:	fa                   	cli
  100cb9:	6a 00                	push   $0x0
  100cbb:	6a 06                	push   $0x6
  100cbd:	eb 81                	jmp    100c40 <isr_common>

00100cbf <isr_stub_7>:
  100cbf:	fa                   	cli
  100cc0:	6a 00                	push   $0x0
  100cc2:	6a 07                	push   $0x7
  100cc4:	e9 77 ff ff ff       	jmp    100c40 <isr_common>

00100cc9 <isr_stub_8>:
  100cc9:	fa                   	cli
  100cca:	6a 08                	push   $0x8
  100ccc:	e9 6f ff ff ff       	jmp    100c40 <isr_common>

00100cd1 <isr_stub_9>:
  100cd1:	fa                   	cli
  100cd2:	6a 00                	push   $0x0
  100cd4:	6a 09                	push   $0x9
  100cd6:	e9 65 ff ff ff       	jmp    100c40 <isr_common>

00100cdb <isr_stub_10>:
  100cdb:	fa                   	cli
  100cdc:	6a 0a                	push   $0xa
  100cde:	e9 5d ff ff ff       	jmp    100c40 <isr_common>

00100ce3 <isr_stub_11>:
  100ce3:	fa                   	cli
  100ce4:	6a 0b                	push   $0xb
  100ce6:	e9 55 ff ff ff       	jmp    100c40 <isr_common>

00100ceb <isr_stub_12>:
  100ceb:	fa                   	cli
  100cec:	6a 0c                	push   $0xc
  100cee:	e9 4d ff ff ff       	jmp    100c40 <isr_common>

00100cf3 <isr_stub_13>:
  100cf3:	fa                   	cli
  100cf4:	6a 0d                	push   $0xd
  100cf6:	e9 45 ff ff ff       	jmp    100c40 <isr_common>

00100cfb <isr_stub_14>:
  100cfb:	fa                   	cli
  100cfc:	6a 0e                	push   $0xe
  100cfe:	e9 3d ff ff ff       	jmp    100c40 <isr_common>

00100d03 <isr_stub_15>:
  100d03:	fa                   	cli
  100d04:	6a 00                	push   $0x0
  100d06:	6a 0f                	push   $0xf
  100d08:	e9 33 ff ff ff       	jmp    100c40 <isr_common>

00100d0d <isr_stub_16>:
  100d0d:	fa                   	cli
  100d0e:	6a 00                	push   $0x0
  100d10:	6a 10                	push   $0x10
  100d12:	e9 29 ff ff ff       	jmp    100c40 <isr_common>

00100d17 <isr_stub_17>:
  100d17:	fa                   	cli
  100d18:	6a 11                	push   $0x11
  100d1a:	e9 21 ff ff ff       	jmp    100c40 <isr_common>

00100d1f <isr_stub_18>:
  100d1f:	fa                   	cli
  100d20:	6a 00                	push   $0x0
  100d22:	6a 12                	push   $0x12
  100d24:	e9 17 ff ff ff       	jmp    100c40 <isr_common>

00100d29 <isr_stub_19>:
  100d29:	fa                   	cli
  100d2a:	6a 00                	push   $0x0
  100d2c:	6a 13                	push   $0x13
  100d2e:	e9 0d ff ff ff       	jmp    100c40 <isr_common>

00100d33 <isr_stub_20>:
  100d33:	fa                   	cli
  100d34:	6a 00                	push   $0x0
  100d36:	6a 14                	push   $0x14
  100d38:	e9 03 ff ff ff       	jmp    100c40 <isr_common>

00100d3d <isr_stub_21>:
  100d3d:	fa                   	cli
  100d3e:	6a 00                	push   $0x0
  100d40:	6a 15                	push   $0x15
  100d42:	e9 f9 fe ff ff       	jmp    100c40 <isr_common>

00100d47 <isr_stub_22>:
  100d47:	fa                   	cli
  100d48:	6a 00                	push   $0x0
  100d4a:	6a 16                	push   $0x16
  100d4c:	e9 ef fe ff ff       	jmp    100c40 <isr_common>

00100d51 <isr_stub_23>:
  100d51:	fa                   	cli
  100d52:	6a 00                	push   $0x0
  100d54:	6a 17                	push   $0x17
  100d56:	e9 e5 fe ff ff       	jmp    100c40 <isr_common>

00100d5b <isr_stub_24>:
  100d5b:	fa                   	cli
  100d5c:	6a 00                	push   $0x0
  100d5e:	6a 18                	push   $0x18
  100d60:	e9 db fe ff ff       	jmp    100c40 <isr_common>

00100d65 <isr_stub_25>:
  100d65:	fa                   	cli
  100d66:	6a 00                	push   $0x0
  100d68:	6a 19                	push   $0x19
  100d6a:	e9 d1 fe ff ff       	jmp    100c40 <isr_common>

00100d6f <isr_stub_26>:
  100d6f:	fa                   	cli
  100d70:	6a 00                	push   $0x0
  100d72:	6a 1a                	push   $0x1a
  100d74:	e9 c7 fe ff ff       	jmp    100c40 <isr_common>

00100d79 <isr_stub_27>:
  100d79:	fa                   	cli
  100d7a:	6a 00                	push   $0x0
  100d7c:	6a 1b                	push   $0x1b
  100d7e:	e9 bd fe ff ff       	jmp    100c40 <isr_common>

00100d83 <isr_stub_28>:
  100d83:	fa                   	cli
  100d84:	6a 00                	push   $0x0
  100d86:	6a 1c                	push   $0x1c
  100d88:	e9 b3 fe ff ff       	jmp    100c40 <isr_common>

00100d8d <isr_stub_29>:
  100d8d:	fa                   	cli
  100d8e:	6a 00                	push   $0x0
  100d90:	6a 1d                	push   $0x1d
  100d92:	e9 a9 fe ff ff       	jmp    100c40 <isr_common>

00100d97 <isr_stub_30>:
  100d97:	fa                   	cli
  100d98:	6a 00                	push   $0x0
  100d9a:	6a 1e                	push   $0x1e
  100d9c:	e9 9f fe ff ff       	jmp    100c40 <isr_common>

00100da1 <isr_stub_31>:
  100da1:	fa                   	cli
  100da2:	6a 00                	push   $0x0
  100da4:	6a 1f                	push   $0x1f
  100da6:	e9 95 fe ff ff       	jmp    100c40 <isr_common>

00100dab <irq_stub_0>:
  100dab:	fa                   	cli
  100dac:	6a 00                	push   $0x0
  100dae:	6a 20                	push   $0x20
  100db0:	e9 b2 fe ff ff       	jmp    100c67 <irq_common>

00100db5 <irq_stub_1>:
  100db5:	fa                   	cli
  100db6:	6a 00                	push   $0x0
  100db8:	6a 21                	push   $0x21
  100dba:	e9 a8 fe ff ff       	jmp    100c67 <irq_common>

00100dbf <irq_stub_2>:
  100dbf:	fa                   	cli
  100dc0:	6a 00                	push   $0x0
  100dc2:	6a 22                	push   $0x22
  100dc4:	e9 9e fe ff ff       	jmp    100c67 <irq_common>

00100dc9 <irq_stub_3>:
  100dc9:	fa                   	cli
  100dca:	6a 00                	push   $0x0
  100dcc:	6a 23                	push   $0x23
  100dce:	e9 94 fe ff ff       	jmp    100c67 <irq_common>

00100dd3 <irq_stub_4>:
  100dd3:	fa                   	cli
  100dd4:	6a 00                	push   $0x0
  100dd6:	6a 24                	push   $0x24
  100dd8:	e9 8a fe ff ff       	jmp    100c67 <irq_common>

00100ddd <irq_stub_5>:
  100ddd:	fa                   	cli
  100dde:	6a 00                	push   $0x0
  100de0:	6a 25                	push   $0x25
  100de2:	e9 80 fe ff ff       	jmp    100c67 <irq_common>

00100de7 <irq_stub_6>:
  100de7:	fa                   	cli
  100de8:	6a 00                	push   $0x0
  100dea:	6a 26                	push   $0x26
  100dec:	e9 76 fe ff ff       	jmp    100c67 <irq_common>

00100df1 <irq_stub_7>:
  100df1:	fa                   	cli
  100df2:	6a 00                	push   $0x0
  100df4:	6a 27                	push   $0x27
  100df6:	e9 6c fe ff ff       	jmp    100c67 <irq_common>

00100dfb <irq_stub_8>:
  100dfb:	fa                   	cli
  100dfc:	6a 00                	push   $0x0
  100dfe:	6a 28                	push   $0x28
  100e00:	e9 62 fe ff ff       	jmp    100c67 <irq_common>

00100e05 <irq_stub_9>:
  100e05:	fa                   	cli
  100e06:	6a 00                	push   $0x0
  100e08:	6a 29                	push   $0x29
  100e0a:	e9 58 fe ff ff       	jmp    100c67 <irq_common>

00100e0f <irq_stub_10>:
  100e0f:	fa                   	cli
  100e10:	6a 00                	push   $0x0
  100e12:	6a 2a                	push   $0x2a
  100e14:	e9 4e fe ff ff       	jmp    100c67 <irq_common>

00100e19 <irq_stub_11>:
  100e19:	fa                   	cli
  100e1a:	6a 00                	push   $0x0
  100e1c:	6a 2b                	push   $0x2b
  100e1e:	e9 44 fe ff ff       	jmp    100c67 <irq_common>

00100e23 <irq_stub_12>:
  100e23:	fa                   	cli
  100e24:	6a 00                	push   $0x0
  100e26:	6a 2c                	push   $0x2c
  100e28:	e9 3a fe ff ff       	jmp    100c67 <irq_common>

00100e2d <irq_stub_13>:
  100e2d:	fa                   	cli
  100e2e:	6a 00                	push   $0x0
  100e30:	6a 2d                	push   $0x2d
  100e32:	e9 30 fe ff ff       	jmp    100c67 <irq_common>

00100e37 <irq_stub_14>:
  100e37:	fa                   	cli
  100e38:	6a 00                	push   $0x0
  100e3a:	6a 2e                	push   $0x2e
  100e3c:	e9 26 fe ff ff       	jmp    100c67 <irq_common>

00100e41 <irq_stub_15>:
  100e41:	fa                   	cli
  100e42:	6a 00                	push   $0x0
  100e44:	6a 2f                	push   $0x2f
  100e46:	e9 1c fe ff ff       	jmp    100c67 <irq_common>

00100e4b <panic>:
  100e4b:	55                   	push   %ebp
  100e4c:	89 e5                	mov    %esp,%ebp
  100e4e:	83 ec 08             	sub    $0x8,%esp
  100e51:	83 ec 0c             	sub    $0xc,%esp
  100e54:	68 a1 11 10 00       	push   $0x1011a1
  100e59:	e8 d9 f5 ff ff       	call   100437 <terminal_writeIn>
  100e5e:	83 c4 10             	add    $0x10,%esp
  100e61:	83 ec 0c             	sub    $0xc,%esp
  100e64:	68 a2 11 10 00       	push   $0x1011a2
  100e69:	e8 c9 f5 ff ff       	call   100437 <terminal_writeIn>
  100e6e:	83 c4 10             	add    $0x10,%esp
  100e71:	83 ec 0c             	sub    $0xc,%esp
  100e74:	68 bb 11 10 00       	push   $0x1011bb
  100e79:	e8 b9 f5 ff ff       	call   100437 <terminal_writeIn>
  100e7e:	83 c4 10             	add    $0x10,%esp
  100e81:	83 ec 0c             	sub    $0xc,%esp
  100e84:	68 a2 11 10 00       	push   $0x1011a2
  100e89:	e8 a9 f5 ff ff       	call   100437 <terminal_writeIn>
  100e8e:	83 c4 10             	add    $0x10,%esp
  100e91:	83 ec 0c             	sub    $0xc,%esp
  100e94:	ff 75 08             	push   0x8(%ebp)
  100e97:	e8 9b f5 ff ff       	call   100437 <terminal_writeIn>
  100e9c:	83 c4 10             	add    $0x10,%esp
  100e9f:	83 ec 0c             	sub    $0xc,%esp
  100ea2:	68 a1 11 10 00       	push   $0x1011a1
  100ea7:	e8 8b f5 ff ff       	call   100437 <terminal_writeIn>
  100eac:	83 c4 10             	add    $0x10,%esp
  100eaf:	83 ec 0c             	sub    $0xc,%esp
  100eb2:	68 c8 11 10 00       	push   $0x1011c8
  100eb7:	e8 7b f5 ff ff       	call   100437 <terminal_writeIn>
  100ebc:	83 c4 10             	add    $0x10,%esp
  100ebf:	f4                   	hlt
  100ec0:	eb fd                	jmp    100ebf <panic+0x74>

00100ec2 <exception_handler>:
  100ec2:	55                   	push   %ebp
  100ec3:	89 e5                	mov    %esp,%ebp
  100ec5:	f4                   	hlt
  100ec6:	eb fd                	jmp    100ec5 <exception_handler+0x3>

00100ec8 <outb>:
  100ec8:	55                   	push   %ebp
  100ec9:	89 e5                	mov    %esp,%ebp
  100ecb:	83 ec 08             	sub    $0x8,%esp
  100ece:	8b 55 08             	mov    0x8(%ebp),%edx
  100ed1:	8b 45 0c             	mov    0xc(%ebp),%eax
  100ed4:	66 89 55 fc          	mov    %dx,-0x4(%ebp)
  100ed8:	88 45 f8             	mov    %al,-0x8(%ebp)
  100edb:	0f b6 45 f8          	movzbl -0x8(%ebp),%eax
  100edf:	0f b7 55 fc          	movzwl -0x4(%ebp),%edx
  100ee3:	ee                   	out    %al,(%dx)
  100ee4:	90                   	nop
  100ee5:	c9                   	leave
  100ee6:	c3                   	ret

00100ee7 <pic_send_eoi>:
  100ee7:	55                   	push   %ebp
  100ee8:	89 e5                	mov    %esp,%ebp
  100eea:	83 ec 04             	sub    $0x4,%esp
  100eed:	8b 45 08             	mov    0x8(%ebp),%eax
  100ef0:	88 45 fc             	mov    %al,-0x4(%ebp)
  100ef3:	80 7d fc 07          	cmpb   $0x7,-0x4(%ebp)
  100ef7:	76 0f                	jbe    100f08 <pic_send_eoi+0x21>
  100ef9:	6a 20                	push   $0x20
  100efb:	68 a0 00 00 00       	push   $0xa0
  100f00:	e8 c3 ff ff ff       	call   100ec8 <outb>
  100f05:	83 c4 08             	add    $0x8,%esp
  100f08:	6a 20                	push   $0x20
  100f0a:	6a 20                	push   $0x20
  100f0c:	e8 b7 ff ff ff       	call   100ec8 <outb>
  100f11:	83 c4 08             	add    $0x8,%esp
  100f14:	90                   	nop
  100f15:	c9                   	leave
  100f16:	c3                   	ret

00100f17 <pic_init>:
  100f17:	55                   	push   %ebp
  100f18:	89 e5                	mov    %esp,%ebp
  100f1a:	6a 11                	push   $0x11
  100f1c:	6a 20                	push   $0x20
  100f1e:	e8 a5 ff ff ff       	call   100ec8 <outb>
  100f23:	83 c4 08             	add    $0x8,%esp
  100f26:	6a 11                	push   $0x11
  100f28:	68 a0 00 00 00       	push   $0xa0
  100f2d:	e8 96 ff ff ff       	call   100ec8 <outb>
  100f32:	83 c4 08             	add    $0x8,%esp
  100f35:	6a 20                	push   $0x20
  100f37:	6a 21                	push   $0x21
  100f39:	e8 8a ff ff ff       	call   100ec8 <outb>
  100f3e:	83 c4 08             	add    $0x8,%esp
  100f41:	6a 28                	push   $0x28
  100f43:	68 a1 00 00 00       	push   $0xa1
  100f48:	e8 7b ff ff ff       	call   100ec8 <outb>
  100f4d:	83 c4 08             	add    $0x8,%esp
  100f50:	6a 04                	push   $0x4
  100f52:	6a 21                	push   $0x21
  100f54:	e8 6f ff ff ff       	call   100ec8 <outb>
  100f59:	83 c4 08             	add    $0x8,%esp
  100f5c:	6a 02                	push   $0x2
  100f5e:	68 a1 00 00 00       	push   $0xa1
  100f63:	e8 60 ff ff ff       	call   100ec8 <outb>
  100f68:	83 c4 08             	add    $0x8,%esp
  100f6b:	6a 01                	push   $0x1
  100f6d:	6a 21                	push   $0x21
  100f6f:	e8 54 ff ff ff       	call   100ec8 <outb>
  100f74:	83 c4 08             	add    $0x8,%esp
  100f77:	6a 01                	push   $0x1
  100f79:	68 a1 00 00 00       	push   $0xa1
  100f7e:	e8 45 ff ff ff       	call   100ec8 <outb>
  100f83:	83 c4 08             	add    $0x8,%esp
  100f86:	6a 00                	push   $0x0
  100f88:	6a 21                	push   $0x21
  100f8a:	e8 39 ff ff ff       	call   100ec8 <outb>
  100f8f:	83 c4 08             	add    $0x8,%esp
  100f92:	6a 00                	push   $0x0
  100f94:	68 a1 00 00 00       	push   $0xa1
  100f99:	e8 2a ff ff ff       	call   100ec8 <outb>
  100f9e:	83 c4 08             	add    $0x8,%esp
  100fa1:	90                   	nop
  100fa2:	c9                   	leave
  100fa3:	c3                   	ret

00100fa4 <irq_handler>:
  100fa4:	55                   	push   %ebp
  100fa5:	89 e5                	mov    %esp,%ebp
  100fa7:	83 ec 08             	sub    $0x8,%esp
  100faa:	83 ec 0c             	sub    $0xc,%esp
  100fad:	68 d7 11 10 00       	push   $0x1011d7
  100fb2:	e8 5b f4 ff ff       	call   100412 <terminal_write>
  100fb7:	83 c4 10             	add    $0x10,%esp
  100fba:	83 ec 0c             	sub    $0xc,%esp
  100fbd:	6a 00                	push   $0x0
  100fbf:	e8 23 ff ff ff       	call   100ee7 <pic_send_eoi>
  100fc4:	83 c4 10             	add    $0x10,%esp
  100fc7:	90                   	nop
  100fc8:	c9                   	leave
  100fc9:	c3                   	ret
  100fca:	57                   	push   %edi
  100fcb:	65 6c                	gs insb (%dx),%es:(%edi)
  100fcd:	63 6f 6d             	arpl   %ebp,0x6d(%edi)
  100fd0:	65 21 0a             	and    %ecx,%gs:(%edx)
  100fd3:	00 53 79             	add    %dl,0x79(%ebx)
  100fd6:	73 74                	jae    10104c <irq_handler+0xa8>
  100fd8:	65 6d                	gs insl (%dx),%es:(%edi)
  100fda:	20 62 6f             	and    %ah,0x6f(%edx)
  100fdd:	6f                   	outsl  %ds:(%esi),(%dx)
  100fde:	74 20                	je     101000 <irq_handler+0x5c>
  100fe0:	73 75                	jae    101057 <irq_handler+0xb3>
  100fe2:	63 63 65             	arpl   %esp,0x65(%ebx)
  100fe5:	73 73                	jae    10105a <irq_handler+0xb6>
  100fe7:	66 75 6c             	data16 jne 101056 <irq_handler+0xb2>
  100fea:	2e 0a 00             	or     %cs:(%eax),%al
  100fed:	0a 00                	or     (%eax),%al
  100fef:	3d 3d 3d 3d 3d       	cmp    $0x3d3d3d3d,%eax
  100ff4:	3d 3d 3d 3d 3d       	cmp    $0x3d3d3d3d,%eax
  100ff9:	3d 3d 3d 3d 3d       	cmp    $0x3d3d3d3d,%eax
  100ffe:	3d 3d 3d 3d 3d       	cmp    $0x3d3d3d3d,%eax
  101003:	0a 00                	or     (%eax),%al
  101005:	4d                   	dec    %ebp
  101006:	69 79 61 72 4f 53 20 	imul   $0x20534f72,0x61(%ecx),%edi
  10100d:	76 30                	jbe    10103f <irq_handler+0x9b>
  10100f:	2e 31 0a             	xor    %ecx,%cs:(%edx)
  101012:	00 3d 3d 3d 3d 3d    	add    %bh,0x3d3d3d3d
  101018:	3d 3d 3d 3d 3d       	cmp    $0x3d3d3d3d,%eax
  10101d:	3d 3d 3d 3d 3d       	cmp    $0x3d3d3d3d,%eax
  101022:	3d 3d 3d 3d 3d       	cmp    $0x3d3d3d3d,%eax
  101027:	00 49 6e             	add    %cl,0x6e(%ecx)
  10102a:	69 74 69 61 6c 69 7a 	imul   $0x697a696c,0x61(%ecx,%ebp,2),%esi
  101031:	69 
  101032:	6e                   	outsb  %ds:(%esi),(%dx)
  101033:	67 20 54 65          	and    %dl,0x65(%si)
  101037:	72 6d                	jb     1010a6 <scancode_table+0x46>
  101039:	69 6e 61 6c 2e 2e 2e 	imul   $0x2e2e2e6c,0x61(%esi),%ebp
  101040:	20 4f 4b             	and    %cl,0x4b(%edi)
  101043:	0a 00                	or     (%eax),%al
  101045:	0a 00                	or     (%eax),%al
  101047:	3e 20 00             	and    %al,%ds:(%eax)
  10104a:	66 90                	xchg   %ax,%ax
  10104c:	66 90                	xchg   %ax,%ax
  10104e:	66 90                	xchg   %ax,%ax
  101050:	66 90                	xchg   %ax,%ax
  101052:	66 90                	xchg   %ax,%ax
  101054:	66 90                	xchg   %ax,%ax
  101056:	66 90                	xchg   %ax,%ax
  101058:	66 90                	xchg   %ax,%ax
  10105a:	66 90                	xchg   %ax,%ax
  10105c:	66 90                	xchg   %ax,%ax
  10105e:	66 90                	xchg   %ax,%ax

00101060 <scancode_table>:
  101060:	00 1b 31 32 33 34 35 36 37 38 39 30 2d 3d 08 09     ..1234567890-=..
  101070:	71 77 65 72 74 79 75 69 6f 70 5b 5d 0a 00 61 73     qwertyuiop[]..as
  101080:	64 66 67 68 6a 6b 6c 3b 27 60 00 5c 7a 78 63 76     dfghjkl;'`.\zxcv
  101090:	62 6e 6d 2c 2e 2f 00 2a 00 20 00 00 00 00 00 00     bnm,./.*. ......
	...
  1010e0:	00 68 65 6c 70 00 41 76 61 69 6c 61 62 6c 65 20     .help.Available 
  1010f0:	63 6f 6d 6d 61 6e 64 73 3a 20 00 61 62 6f 75 74     commands: .about
  101100:	00 76 65 72 73 69 6f 6e 00 65 63 68 6f 20 3c 6d     .version.echo <m
  101110:	65 73 73 61 67 65 3e 00 75 70 74 69 6d 65 00 63     essage>.uptime.c
  101120:	6c 65 61 72 00 4d 69 79 61 72 4f 53 00 00 00 00     lear.MiyarOS....
  101130:	41 20 68 6f 62 62 79 20 6f 70 65 72 61 74 69 6e     A hobby operatin
  101140:	67 20 73 79 73 74 65 6d 20 77 72 69 74 74 65 6e     g system written
  101150:	20 69 6e 20 43 00 4d 69 79 61 72 4f 53 20 76 30      in C.MiyarOS v0
  101160:	2e 31 00 65 63 68 6f 20 00 00 00 00 55 70 74 69     .1.echo ....Upti
  101170:	6d 65 3a 20 54 69 6d 65 72 20 6e 6f 74 20 69 6d     me: Timer not im
  101180:	70 6c 65 6d 65 6e 74 65 64 20 79 65 74 2e 00 55     plemented yet..U
  101190:	6e 6b 6e 6f 77 6e 20 63 6f 6d 6d 61 6e 64 3a 20     nknown command: 
  1011a0:	00 00 3d 3d 3d 3d 3d 3d 3d 3d 3d 3d 3d 3d 3d 3d     ..==============
  1011b0:	3d 3d 3d 3d 3d 3d 3d 3d 3d 3d 00 4b 45 52 4e 45     ==========.KERNE
  1011c0:	4c 20 50 41 4e 49 43 00 53 79 73 74 65 6d 20 48     L PANIC.System H
  1011d0:	61 6c 74 65 64 2e 00 49 52 51 0a 00                 alted..IRQ..
