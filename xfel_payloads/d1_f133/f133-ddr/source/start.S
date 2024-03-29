/*
 * start.S
 *
 * Copyright(c) 2007-2021 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <linkage.h>
#include <riscv64.h>

	.global _start
_start:
	li t1, 0x1 << 22
	csrs mxstatus, t1
	li t1, 0x30013
	csrs mcor, t1
	j reset

ddr_param:
	.word 528
	.word 2
	.word 0x07b7bf9
	.word 0x00
	.word 0x000000d2
	.word 0x00000000
	.word 0x00000e73
	.word 0x02
	.word 0x0
	.word 0x0
	.word 0x00471992
	.word 0x0131a10c
	.word 0x00057041
	.word 0xb4787896
	.word 0x0
	.word 0x48484848
	.word 0x48
	.word 0x1621121e
	.word 0x0
	.word 0x0
	.word 0x00000000
	.word 0x00030010
	.word 0x00000035
	.word 0x34000000
	.word 0, 0, 0, 0, 0, 0, 0, 0

reset:
	addi sp, sp, -32
	sd s0, 8(sp)
	sd s1, 16(sp)
	sd ra, 24(sp)
	mv s0, a0

	li t0, 0x07090108
	sw zero, (t0)
	jal sys_uart_init
	jal sys_clock_init
	la a0, ddr_param
	jal sys_dram_init

	ld ra, 24(sp)
	ld s0, 8(sp)
	ld s1, 16(sp)
	addi sp, sp, 32
	ret
