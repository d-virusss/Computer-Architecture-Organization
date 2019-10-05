/**********************************************************************
 * Copyright (c) 2019
 *  Sang-Hoon Kim <sanghoonkim@ajou.ac.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTIABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

 /* To avoid security error on Visual Studio */
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

/*====================================================================*/
/*          ****** DO NOT MODIFY ANYTHING FROM THIS LINE ******       */
#define MAX_NR_TOKENS	32	/* Maximum length of tokens in a command */
#define MAX_TOKEN_LEN	64	/* Maximum length of single token */
#define MAX_ASSEMBLY	256 /* Maximum length of assembly string */

typedef unsigned char bool;
#define true	1
#define false	0
/*          ****** DO NOT MODIFY ANYTHING UP TO THIS LINE ******      */
/*====================================================================*/


/***********************************************************************
 * translate
 *
 * DESCRIPTION
 *   Translate assembly represented in @tokens[] into a MIPS instruction.
 * This translate should support following 13 assembly commands
 *
 *	- add, sub, and, or, nor - r
 *	- sll, srl, sra - r shift
 *	- addi, andi, ori, lw, sw - i
 *
 * RETURN VALUE
 *   Return a 32-bit MIPS instruction
 *
 */
 /*int bintodec(char* bit)
 {
	 int num = 0;
	 for (int i = 0; i < 32; i++)
	 {
		 num = num * 2 + bit[i] - '0';
	 }
	 return num;
 }*/
 /*char* itoc(int num) // 5bit짜리 2진수로 전환
 {
	 int binary[5] = { 0, };
	 char token[5];

	 int position = 0;
	 while (1)
	 {
		 binary[position] = num % 2;    // 2로 나누었을 때 나머지를 배열에 저장
		 num = num / 2;             // 2로 나눈 몫을 저장

		 position++;    // 자릿수 변경

		 if (num == 0)    // 몫이 0이 되면 반복을 끝냄
			 break;
	 }
	 for (int j = 0; j < 5; j++)
	 {
		 token[j] = (char)binary[(4 - j)] + 0x30;
	 }
	 return token;
 }*/
 /*char* itoc16(int num) // 16bit짜리 2진수로 전환
 {
	 int binary[16] = { 0, };
	 char token[16];

	 int position = 0;
	 while (1)
	 {
		 binary[position] = num % 2;    // 2로 나누었을 때 나머지를 배열에 저장
		 num = num / 2;             // 2로 나눈 몫을 저장

		 position++;    // 자릿수 변경

		 if (num == 0)    // 몫이 0이 되면 반복을 끝냄
			 break;
	 }
	 for (int j = 0; j < 16; j++)
	 {
		 token[j] = (char)binary[(15 - j)] + 0x30;
	 }
	 return token;
 }*/
static unsigned int translate(int nr_tokens, char *tokens[])
{
	/* TODO:
	 * This is an example MIPS instruction. You should change it accordingly.
	 */
	int R_format = 0; // 0이면 R포맷, 1이면 I포맷
	int shift = 0; // 0이면 그냥 R포맷, 1이면 R포맷 shift

	int dec = 0;
	int bit32[32] = { 0, };
	int OP_code[6] = { 0, };
	int rs[5] = { 0, };
	int rt[5] = { 0, };
	int rd[5] = { 0, };
	int shamt[5] = { 0, };
	int func[6] = { 0, };
	int constant[16] = { 0, };
	int rstoken;
	int rttoken;
	int rdtoken;
	int shamt_token;
	int constant_token;
	int minus = 0; // immediate value가 음수면 1 음수가 아니면 0

	int zero = 0; int at = 1; int v0 = 2; int v1 = 3;
	int a0 = 4; int a1 = 5; int a2 = 6; int a3 = 7;
	int t0 = 8; int t1 = 9; int t2 = 10; int t3 = 11; int t4 = 12; int t5 = 13; int t6 = 14; int t7 = 15;
	int s0 = 16; int s1 = 17; int s2 = 18; int s3 = 19; int s4 = 20; int s5 = 21; int s6 = 22; int s7 = 23;
	int t8 = 24; int t9 = 25;
	int k1 = 26; int k2 = 27;
	int gp = 28; int sp = 29; int fp = 30; int ra = 31;
	int add = 32; int sub = 34; int and = 36; int or = 37; int nor = 39;
	int addi = 8; int andi = 12; int ori = 13; int lw = 35; int sw = 43;
	int sll = 0; int srl = 2; int sra = 3;
	
	R_format = strcmp(tokens[0], "add");
	//(|| strcmp(tokens[0], "sub") || strcmp(tokens[0], "and") || strcmp(tokens[0], "or") || strcmp(tokens[0], "nor"))
	// sll, srl, sra
	if (strcmp(tokens[0], "add") == 0)
	{
		R_format = 1; shift = 0;
	}
	else if (strcmp(tokens[0], "sub") == 0)
	{
		R_format = 1; shift = 0;
	}
	else if (strcmp(tokens[0], "and") == 0)
	{
		R_format = 1; shift = 0;
	}
	else if (strcmp(tokens[0], "or") == 0)
	{
		R_format = 1; shift = 0;
	}
	else if (strcmp(tokens[0], "nor") == 0)
	{
		R_format = 1; shift = 0;
	}
	else if (strcmp(tokens[0], "sll") == 0)
	{
		R_format = 1; shift = 1;
	}
	else if(strcmp(tokens[0], "srl") == 0)
	{
		R_format = 1; shift = 1;
	}
	else if (strcmp(tokens[0], "sra") == 0)
	{
		R_format = 1; shift = 1;
	}
	else
	{
		R_format = 0;
	} // tokens[0]의 값에 따라 포맷 나눠주는 분기문
	
	
	if ((R_format == 1) && (shift == 0)) // R format일 때
	{
		// R format은 OPcode가 어차피 0 이므로 제어할 필요 없음
		if (strcmp(tokens[2], "zero") == 0)
			rstoken = 0;
		else if (strcmp(tokens[2], "at") == 0)
			rstoken = 1;
		else if (strcmp(tokens[2], "v0") == 0)
			rstoken = 2;
		else if (strcmp(tokens[2], "v1") == 0)
			rstoken = 3;
		else if (strcmp(tokens[2], "a0") == 0)
			rstoken = 4;
		else if (strcmp(tokens[2], "a1") == 0)
			rstoken = 5;
		else if (strcmp(tokens[2], "a2") == 0)
			rstoken = 6;
		else if (strcmp(tokens[2], "a3") == 0)
			rstoken = 7;
		else if (strcmp(tokens[2], "t0") == 0)
			rstoken = 8;
		else if (strcmp(tokens[2], "t1") == 0)
			rstoken = 9;
		else if (strcmp(tokens[2], "t2") == 0)
			rstoken = 10;
		else if (strcmp(tokens[2], "t3") == 0)
			rstoken = 11;
		else if (strcmp(tokens[2], "t4") == 0)
			rstoken = 12;
		else if (strcmp(tokens[2], "t5") == 0)
			rstoken = 13;
		else if (strcmp(tokens[2], "t6") == 0)
			rstoken = 14;
		else if (strcmp(tokens[2], "t7") == 0)
			rstoken = 15;
		else if (strcmp(tokens[2], "s0") == 0)
			rstoken = 16;
		else if (strcmp(tokens[2], "s1") == 0)
			rstoken = 17;
		else if (strcmp(tokens[2], "s2") == 0)
			rstoken = 18;
		else if (strcmp(tokens[2], "s3") == 0)
			rstoken = 19;
		else if (strcmp(tokens[2], "s4") == 0)
			rstoken = 20;
		else if (strcmp(tokens[2], "s5") == 0)
			rstoken = 21;
		else if (strcmp(tokens[2], "s6") == 0)
			rstoken = 22;
		else if (strcmp(tokens[2], "s7") == 0)
			rstoken = 23;
		else if (strcmp(tokens[2], "t8") == 0)
			rstoken = 24;
		else if (strcmp(tokens[2], "t9") == 0)
			rstoken = 25;
		else if (strcmp(tokens[2], "k1") == 0)
			rstoken = 26;
		else if (strcmp(tokens[2], "k2") == 0)
			rstoken = 27;
		else if (strcmp(tokens[2], "gp") == 0)
			rstoken = 28;
		else if (strcmp(tokens[2], "sp") == 0)
			rstoken = 29;
		else if (strcmp(tokens[2], "fp") == 0)
			rstoken = 30;
		else if (strcmp(tokens[2], "ra") == 0)
			rstoken = 31;
		else
			rstoken = atoi(tokens[2]); // tokens[2]를 rstoken로 초기화
		if (strcmp(tokens[3], "zero") == 0)
			rttoken = 0;
		else if (strcmp(tokens[3], "at") == 0)
			rttoken = 1;
		else if (strcmp(tokens[3], "v0") == 0)
			rttoken = 2;
		else if (strcmp(tokens[3], "v1") == 0)
			rttoken = 3;
		else if (strcmp(tokens[3], "a0") == 0)
			rttoken = 4;
		else if (strcmp(tokens[3], "a1") == 0)
			rttoken = 5;
		else if (strcmp(tokens[3], "a2") == 0)
			rttoken = 6;
		else if (strcmp(tokens[3], "a3") == 0)
			rttoken = 7;
		else if (strcmp(tokens[3], "t0") == 0)
			rttoken = 8;
		else if (strcmp(tokens[3], "t1") == 0)
			rttoken = 9;
		else if (strcmp(tokens[3], "t2") == 0)
			rttoken = 10;
		else if (strcmp(tokens[3], "t3") == 0)
			rttoken = 11;
		else if (strcmp(tokens[3], "t4") == 0)
			rttoken = 12;
		else if (strcmp(tokens[3], "t5") == 0)
			rttoken = 13;
		else if (strcmp(tokens[3], "t6") == 0)
			rttoken = 14;
		else if (strcmp(tokens[3], "t7") == 0)
			rttoken = 15;
		else if (strcmp(tokens[3], "s0") == 0)
			rttoken = 16;
		else if (strcmp(tokens[3], "s1") == 0)
			rttoken = 17;
		else if (strcmp(tokens[3], "s2") == 0)
			rttoken = 18;
		else if (strcmp(tokens[3], "s3") == 0)
			rttoken = 19;
		else if (strcmp(tokens[3], "s4") == 0)
			rttoken = 20;
		else if (strcmp(tokens[3], "s5") == 0)
			rttoken = 21;
		else if (strcmp(tokens[3], "s6") == 0)
			rttoken = 22;
		else if (strcmp(tokens[3], "s7") == 0)
			rttoken = 23;
		else if (strcmp(tokens[3], "t8") == 0)
			rttoken = 24;
		else if (strcmp(tokens[3], "t9") == 0)
			rttoken = 25;
		else if (strcmp(tokens[3], "k1") == 0)
			rttoken = 26;
		else if (strcmp(tokens[3], "k2") == 0)
			rttoken = 27;
		else if (strcmp(tokens[3], "gp") == 0)
			rttoken = 28;
		else if (strcmp(tokens[3], "sp") == 0)
			rttoken = 29;
		else if (strcmp(tokens[3], "fp") == 0)
			rttoken = 30;
		else if (strcmp(tokens[3], "ra") == 0)
			rttoken = 31;
		else
			rttoken = atoi(tokens[3]); // tokens[3]를 rttoken로 초기화

		if (strcmp(tokens[1], "zero") == 0)
			rdtoken = 0;
		else if (strcmp(tokens[1], "at") == 0)
			rdtoken = 1;
		else if (strcmp(tokens[1], "v0") == 0)
			rdtoken = 2;
		else if (strcmp(tokens[1], "v1") == 0)
			rdtoken = 3;
		else if (strcmp(tokens[1], "a0") == 0)
			rdtoken = 4;
		else if (strcmp(tokens[1], "a1") == 0)
			rdtoken = 5;
		else if (strcmp(tokens[1], "a2") == 0)
			rdtoken = 6;
		else if (strcmp(tokens[1], "a3") == 0)
			rdtoken = 7;
		else if (strcmp(tokens[1], "t0") == 0)
			rdtoken = 8;
		else if (strcmp(tokens[1], "t1") == 0)
			rdtoken = 9;
		else if (strcmp(tokens[1], "t2") == 0)
			rdtoken = 10;
		else if (strcmp(tokens[1], "t3") == 0)
			rdtoken = 11;
		else if (strcmp(tokens[1], "t4") == 0)
			rdtoken = 12;
		else if (strcmp(tokens[1], "t5") == 0)
			rdtoken = 13;
		else if (strcmp(tokens[1], "t6") == 0)
			rdtoken = 14;
		else if (strcmp(tokens[1], "t7") == 0)
			rdtoken = 15;
		else if (strcmp(tokens[1], "s0") == 0)
			rdtoken = 16;
		else if (strcmp(tokens[1], "s1") == 0)
			rdtoken = 17;
		else if (strcmp(tokens[1], "s2") == 0)
			rdtoken = 18;
		else if (strcmp(tokens[1], "s3") == 0)
			rdtoken = 19;
		else if (strcmp(tokens[1], "s4") == 0)
			rdtoken = 20;
		else if (strcmp(tokens[1], "s5") == 0)
			rdtoken = 21;
		else if (strcmp(tokens[1], "s6") == 0)
			rdtoken = 22;
		else if (strcmp(tokens[1], "s7") == 0)
			rdtoken = 23;
		else if (strcmp(tokens[1], "t8") == 0)
			rdtoken = 24;
		else if (strcmp(tokens[1], "t9") == 0)
			rdtoken = 25;
		else if (strcmp(tokens[1], "k1") == 0)
			rdtoken = 26;
		else if (strcmp(tokens[1], "k2") == 0)
			rdtoken = 27;
		else if (strcmp(tokens[1], "gp") == 0)
			rdtoken = 28;
		else if (strcmp(tokens[1], "sp") == 0)
			rdtoken = 29;
		else if (strcmp(tokens[1], "fp") == 0)
			rdtoken = 30;
		else if (strcmp(tokens[1], "ra") == 0)
			rdtoken = 31;
		else
			rdtoken = atoi(tokens[1]); // tokens[1]을 rdtoken로 초기화

		for (int j = 0; j < 5; j++)
		{
			rs[4 - j] = rstoken % 2;
			rstoken = rstoken / 2;
			if (rstoken == 0)
				break;
		} // rstoken의 값을 rs int형 배열에 이진법표기로 입력
		for (int j = 0; j < 5; j++)
		{
			rt[4 - j] = rttoken % 2;
			rttoken = rttoken / 2;
			if (rttoken == 0)
				break;
		} // rttoken에 같은 행동 반복
		for (int j = 0; j < 5; j++)
		{
			rd[4 - j] = rdtoken % 2;
			rdtoken = rdtoken / 2;
			if (rdtoken == 0)
				break;
		} // rdtoken에 같은 행동 반복
		if (strcmp(tokens[0], "add") == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				func[5 - j] = add % 2;
				add = add / 2;
				if (add == 0)
					break;
			}
		} // instruction에 따른 func 값 입력 - add
		if (strcmp(tokens[0], "sub") == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				func[5 - j] = sub % 2;
				sub = sub / 2;
				if (sub == 0)
					break;
			}
		} // sub
		if (strcmp(tokens[0], "and") == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				func[5 - j] = and % 2;
				and = and / 2;
				if (add == 0)
					break;
			}
		} // and
		if (strcmp(tokens[0], "or") == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				func[5 - j] = or % 2;
				or = or / 2;
				if (or == 0)
					break;
			}
		} // or
		if (strcmp(tokens[0], "nor") == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				func[5 - j] = nor % 2;
				nor = nor / 2;
				if (nor == 0)
					break;
			}
		} // nor
		for (int i = 0; i < 5; i++)
		{
			bit32[i + 6] = rs[i];
			bit32[i + 11] = rt[i];
			bit32[i + 16] = rd[i];
			bit32[i + 26] = func[i];
		}
		bit32[31] = func[5]; // bit32 배열에 전부 저장 완료
	}
	else if ((R_format == 1) && (shift == 1)) // R format중 shift일 때
	{
		if (strcmp(tokens[1], "zero") == 0)
			rdtoken = 0;
		else if (strcmp(tokens[1], "at") == 0)
			rdtoken = 1;
		else if (strcmp(tokens[1], "v0") == 0)
			rdtoken = 2;
		else if (strcmp(tokens[1], "v1") == 0)
			rdtoken = 3;
		else if (strcmp(tokens[1], "a0") == 0)
			rdtoken = 4;
		else if (strcmp(tokens[1], "a1") == 0)
			rdtoken = 5;
		else if (strcmp(tokens[1], "a2") == 0)
			rdtoken = 6;
		else if (strcmp(tokens[1], "a3") == 0)
			rdtoken = 7;
		else if (strcmp(tokens[1], "t0") == 0)
			rdtoken = 8;
		else if (strcmp(tokens[1], "t1") == 0)
			rdtoken = 9;
		else if (strcmp(tokens[1], "t2") == 0)
			rdtoken = 10;
		else if (strcmp(tokens[1], "t3") == 0)
			rdtoken = 11;
		else if (strcmp(tokens[1], "t4") == 0)
			rdtoken = 12;
		else if (strcmp(tokens[1], "t5") == 0)
			rdtoken = 13;
		else if (strcmp(tokens[1], "t6") == 0)
			rdtoken = 14;
		else if (strcmp(tokens[1], "t7") == 0)
			rdtoken = 15;
		else if (strcmp(tokens[1], "s0") == 0)
			rdtoken = 16;
		else if (strcmp(tokens[1], "s1") == 0)
			rdtoken = 17;
		else if (strcmp(tokens[1], "s2") == 0)
			rdtoken = 18;
		else if (strcmp(tokens[1], "s3") == 0)
			rdtoken = 19;
		else if (strcmp(tokens[1], "s4") == 0)
			rdtoken = 20;
		else if (strcmp(tokens[1], "s5") == 0)
			rdtoken = 21;
		else if (strcmp(tokens[1], "s6") == 0)
			rdtoken = 22;
		else if (strcmp(tokens[1], "s7") == 0)
			rdtoken = 23;
		else if (strcmp(tokens[1], "t8") == 0)
			rdtoken = 24;
		else if (strcmp(tokens[1], "t9") == 0)
			rdtoken = 25;
		else if (strcmp(tokens[1], "k1") == 0)
			rdtoken = 26;
		else if (strcmp(tokens[1], "k2") == 0)
			rdtoken = 27;
		else if (strcmp(tokens[1], "gp") == 0)
			rdtoken = 28;
		else if (strcmp(tokens[1], "sp") == 0)
			rdtoken = 29;
		else if (strcmp(tokens[1], "fp") == 0)
			rdtoken = 30;
		else if (strcmp(tokens[1], "ra") == 0)
			rdtoken = 31;
		else
			rdtoken = atoi(tokens[1]); // tokens[1]을 rdtoken 초기화

		if (strcmp(tokens[2], "zero") == 0)
			rttoken = 0;
		else if (strcmp(tokens[2], "at") == 0)
			rttoken = 1;
		else if (strcmp(tokens[2], "v0") == 0)
			rttoken = 2;
		else if (strcmp(tokens[2], "v1") == 0)
			rttoken = 3;
		else if (strcmp(tokens[2], "a0") == 0)
			rttoken = 4;
		else if (strcmp(tokens[2], "a1") == 0)
			rttoken = 5;
		else if (strcmp(tokens[2], "a2") == 0)
			rttoken = 6;
		else if (strcmp(tokens[2], "a3") == 0)
			rttoken = 7;
		else if (strcmp(tokens[2], "t0") == 0)
			rttoken = 8;
		else if (strcmp(tokens[2], "t1") == 0)
			rttoken = 9;
		else if (strcmp(tokens[2], "t2") == 0)
			rttoken = 10;
		else if (strcmp(tokens[2], "t3") == 0)
			rttoken = 11;
		else if (strcmp(tokens[2], "t4") == 0)
			rttoken = 12;
		else if (strcmp(tokens[2], "t5") == 0)
			rttoken = 13;
		else if (strcmp(tokens[2], "t6") == 0)
			rttoken = 14;
		else if (strcmp(tokens[2], "t7") == 0)
			rttoken = 15;
		else if (strcmp(tokens[2], "s0") == 0)
			rttoken = 16;
		else if (strcmp(tokens[2], "s1") == 0)
			rttoken = 17;
		else if (strcmp(tokens[2], "s2") == 0)
			rttoken = 18;
		else if (strcmp(tokens[2], "s3") == 0)
			rttoken = 19;
		else if (strcmp(tokens[2], "s4") == 0)
			rttoken = 20;
		else if (strcmp(tokens[2], "s5") == 0)
			rttoken = 21;
		else if (strcmp(tokens[2], "s6") == 0)
			rttoken = 22;
		else if (strcmp(tokens[2], "s7") == 0)
			rttoken = 23;
		else if (strcmp(tokens[2], "t8") == 0)
			rttoken = 24;
		else if (strcmp(tokens[2], "t9") == 0)
			rttoken = 25;
		else if (strcmp(tokens[2], "k1") == 0)
			rttoken = 26;
		else if (strcmp(tokens[2], "k2") == 0)
			rttoken = 27;
		else if (strcmp(tokens[2], "gp") == 0)
			rttoken = 28;
		else if (strcmp(tokens[2], "sp") == 0)
			rttoken = 29;
		else if (strcmp(tokens[2], "fp") == 0)
			rttoken = 30;
		else if (strcmp(tokens[2], "ra") == 0)
			rttoken = 31;
		else
			rttoken = atoi(tokens[2]); // tokens[2]를 rttoken 초기화

		if (atoi(tokens[3]) == 0) {
			shamt_token = strtol(tokens[3], NULL, 16);
			if (shamt_token < 0)
			{
				minus = 1;
			}
		}
		else
		{
			shamt_token = atoi(tokens[3]);
			if (shamt_token < 0)
			{
				minus = 1;
			}
		} // 16진수로 들어올때, 10진수로 들어올 때 구분

		for (int j = 0; j < 5; j++)
		{
			rt[4 - j] = rttoken % 2;
			rttoken = rttoken / 2;
			if (rttoken == 0)
				break;
		} // rttoken에 있는 값을 rt배열에 2진법 형태로 입력
		for (int j = 0; j < 5; j++)
		{
			rd[4 - j] = rdtoken % 2;
			rdtoken = rdtoken / 2;
			if (rdtoken == 0)
				break;
		} // rdtoken에 같은 행동 반복
		if (minus == 0)
		{
			for (int j = 0; j < 5; j++)
			{
				shamt[4 - j] = shamt_token % 2;
				shamt_token = shamt_token / 2;
				if (shamt_token == 0)
					break;
			} // shamt에 양수가 있을 때
		}
		else
		{

		} // shamt에 음수가 있을 때 2의 보수법을 이용하여 변환해준다
		if (strcmp(tokens[0], "sll") == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				func[5 - j] = sll % 2;
				sll = sll / 2;
				if (sll == 0)
					break;
			}
		} // sll
		if (strcmp(tokens[0], "srl") == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				func[5 - j] = srl % 2;
				srl = srl / 2;
				if (srl == 0)
					break;
			}
		} // srl
		if (strcmp(tokens[0], "sra") == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				func[5 - j] = sra % 2;
				sra = sra / 2;
				if (sra == 0)
					break;
			}
		} // sra

		for (int i = 0; i < 5; i++)
		{
			bit32[i + 11] = rt[i];
			bit32[i + 16] = rd[i];
			bit32[i + 21] = shamt[i];
			bit32[i + 26] = func[i];
		}
		bit32[31] = func[5]; // bit32 배열에 전부 저장 완료
	}
	else if ((R_format == 0)) // I format 일 때
	{
		if (strcmp(tokens[0], "addi") == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				OP_code[5 - j] = addi % 2;
				addi = addi / 2;
				if (addi == 0)
					break;
			}
		} //instruction에 따른 I format의 OPcode 이진법형태로 OP_code 배열에 입력
		if (strcmp(tokens[0], "andi") == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				OP_code[5 - j] = andi % 2;
				andi = andi / 2;
				if (andi == 0)
					break;
			}
		}
		if (strcmp(tokens[0], "ori") == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				OP_code[5 - j] = ori % 2;
				ori = ori / 2;
				if (ori == 0)
					break;
			}
		}
		if (strcmp(tokens[0], "lw") == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				OP_code[5 - j] = lw % 2;
				lw = lw / 2;
				if (lw == 0)
					break;
			}
		}
		if (strcmp(tokens[0], "sw") == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				OP_code[5 - j] = sw % 2;
				sw = sw / 2;
				if (sw == 0)
					break;
			}
		}
		if (strcmp(tokens[2], "zero") == 0)
			rstoken = 0;
		else if (strcmp(tokens[2], "at") == 0)
			rstoken = 1;
		else if (strcmp(tokens[2], "v0") == 0)
			rstoken = 2;
		else if (strcmp(tokens[2], "v1") == 0)
			rstoken = 3;
		else if (strcmp(tokens[2], "a0") == 0)
			rstoken = 4;
		else if (strcmp(tokens[2], "a1") == 0)
			rstoken = 5;
		else if (strcmp(tokens[2], "a2") == 0)
			rstoken = 6;
		else if (strcmp(tokens[2], "a3") == 0)
			rstoken = 7;
		else if (strcmp(tokens[2], "t0") == 0)
			rstoken = 8;
		else if (strcmp(tokens[2], "t1") == 0)
			rstoken = 9;
		else if (strcmp(tokens[2], "t2") == 0)
			rstoken = 10;
		else if (strcmp(tokens[2], "t3") == 0)
			rstoken = 11;
		else if (strcmp(tokens[2], "t4") == 0)
			rstoken = 12;
		else if (strcmp(tokens[2], "t5") == 0)
			rstoken = 13;
		else if (strcmp(tokens[2], "t6") == 0)
			rstoken = 14;
		else if (strcmp(tokens[2], "t7") == 0)
			rstoken = 15;
		else if (strcmp(tokens[2], "s0") == 0)
			rstoken = 16;
		else if (strcmp(tokens[2], "s1") == 0)
			rstoken = 17;
		else if (strcmp(tokens[2], "s2") == 0)
			rstoken = 18;
		else if (strcmp(tokens[2], "s3") == 0)
			rstoken = 19;
		else if (strcmp(tokens[2], "s4") == 0)
			rstoken = 20;
		else if (strcmp(tokens[2], "s5") == 0)
			rstoken = 21;
		else if (strcmp(tokens[2], "s6") == 0)
			rstoken = 22;
		else if (strcmp(tokens[2], "s7") == 0)
			rstoken = 23;
		else if (strcmp(tokens[2], "t8") == 0)
			rstoken = 24;
		else if (strcmp(tokens[2], "t9") == 0)
			rstoken = 25;
		else if (strcmp(tokens[2], "k1") == 0)
			rstoken = 26;
		else if (strcmp(tokens[2], "k2") == 0)
			rstoken = 27;
		else if (strcmp(tokens[2], "gp") == 0)
			rstoken = 28;
		else if (strcmp(tokens[2], "sp") == 0)
			rstoken = 29;
		else if (strcmp(tokens[2], "fp") == 0)
			rstoken = 30;
		else if (strcmp(tokens[2], "ra") == 0)
			rstoken = 31; 
		else
			rstoken = atoi(tokens[3]); // tokens[2]을 rstoken로 초기화

		if (strcmp(tokens[1], "zero") == 0)
			rttoken = 0;
		else if (strcmp(tokens[1], "at") == 0)
			rttoken = 1;
		else if (strcmp(tokens[1], "v0") == 0)
			rttoken = 2;
		else if (strcmp(tokens[1], "v1") == 0)
			rttoken = 3;
		else if (strcmp(tokens[1], "a0") == 0)
			rttoken = 4;
		else if (strcmp(tokens[1], "a1") == 0)
			rttoken = 5;
		else if (strcmp(tokens[1], "a2") == 0)
			rttoken = 6;
		else if (strcmp(tokens[1], "a3") == 0)
			rttoken = 7;
		else if (strcmp(tokens[1], "t0") == 0)
			rttoken = 8;
		else if (strcmp(tokens[1], "t1") == 0)
			rttoken = 9;
		else if (strcmp(tokens[1], "t2") == 0)
			rttoken = 10;
		else if (strcmp(tokens[1], "t3") == 0)
			rttoken = 11;
		else if (strcmp(tokens[1], "t4") == 0)
			rttoken = 12;
		else if (strcmp(tokens[1], "t5") == 0)
			rttoken = 13;
		else if (strcmp(tokens[1], "t6") == 0)
			rttoken = 14;
		else if (strcmp(tokens[1], "t7") == 0)
			rttoken = 15;
		else if (strcmp(tokens[1], "s0") == 0)
			rttoken = 16;
		else if (strcmp(tokens[1], "s1") == 0)
			rttoken = 17;
		else if (strcmp(tokens[1], "s2") == 0)
			rttoken = 18;
		else if (strcmp(tokens[1], "s3") == 0)
			rttoken = 19;
		else if (strcmp(tokens[1], "s4") == 0)
			rttoken = 20;
		else if (strcmp(tokens[1], "s5") == 0)
			rttoken = 21;
		else if (strcmp(tokens[1], "s6") == 0)
			rttoken = 22;
		else if (strcmp(tokens[1], "s7") == 0)
			rttoken = 23;
		else if (strcmp(tokens[1], "t8") == 0)
			rttoken = 24;
		else if (strcmp(tokens[1], "t9") == 0)
			rttoken = 25;
		else if (strcmp(tokens[1], "k1") == 0)
			rttoken = 26;
		else if (strcmp(tokens[1], "k2") == 0)
			rttoken = 27;
		else if (strcmp(tokens[1], "gp") == 0)
			rttoken = 28;
		else if (strcmp(tokens[1], "sp") == 0)
			rttoken = 29;
		else if (strcmp(tokens[1], "fp") == 0)
			rttoken = 30;
		else if (strcmp(tokens[1], "ra") == 0)
			rttoken = 31;
		else
			rttoken = atoi(tokens[1]); // tokens[1]을 rttoken로 초기화

		for (int j = 0; j < 5; j++)
		{
			rs[4 - j] = rstoken % 2;
			rstoken = rstoken / 2;
			if (rstoken == 0)
				break;
		} // rstoken의 값을 rs int형 배열에 이진법표기로 입력
		for (int j = 0; j < 5; j++)
		{
			rt[4 - j] = rttoken % 2;
			rttoken = rttoken / 2;
			if (rttoken == 0)
				break;
		} // rttoken에 같은 행동 반복


		if (atoi(tokens[3]) == 0) {
			constant_token = strtol(tokens[3], NULL, 16);
			if (constant_token < 0)
			{
				minus = 1;
				constant_token = constant_token * (-1);
			}
		}
		else
		{
			constant_token = atoi(tokens[3]);
			if (constant_token < 0)
			{
				minus = 1;
				constant_token = constant_token * (-1);
			}
		} // 16진수로 들어올때, 10진수로 들어올 때 구분
		if (minus == 1)
		{
			//printf("%d", constant_token);
			for (int j = 0; j < 16; j++)
			{
				constant[15 - j] = constant_token % 2;
				constant_token = constant_token / 2;
				if (constant_token == 0)
					break;
			}
			for (int j = 0; j < 16; j++) 
			{
				if (constant[j] == 1)
					constant[j] = 0;
				else 
					constant[j] = 1;
			}
			for (int i = 15; i >= 0; i--) {
				if (constant[i] == 1)
					constant[i] = 0;
				else {
					constant[i] = 1;
					break;
				}
			}
		}
		else
		{
			for (int j = 0; j < 16; j++)
			{
				constant[15 - j] = constant_token % 2;
				constant_token = constant_token / 2;
				if (constant_token == 0)
					break;
			}
		}

		/*for (int i = 0; i < 16; i++)
		{
			printf("%d", constant[i]);
		}
		printf("\n");*/
		for (int i = 0; i < 5; i++)
		{
			bit32[i] = OP_code[i];
			bit32[i + 6] = rs[i];
			bit32[i + 11] = rt[i];
			bit32[i + 16] = constant[i];
		}
		for (int i = 5; i < 16; i++)
		{
			bit32[i + 16] = constant[i];
		}
		bit32[5] = OP_code[5]; // bit32 배열에 저장 완료

	}

	
	dec += bit32[31] * 1;
	dec += bit32[30] * 2;
	dec += bit32[29] * 2 * 2;
	dec += bit32[28] * 2 * 2 * 2;
	dec += bit32[27] * 2 * 2 * 2 * 2;
	dec += bit32[26] * 2 * 2 * 2 * 2 * 2;
	dec += bit32[25] * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[24] * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[23] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[22] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[21] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[20] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[19] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[18] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[17] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[16] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[15] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[14] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[13] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[12] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[11] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[10] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[9] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[8] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[7] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[6] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[5] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[4] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[3] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[2] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[1] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	dec += bit32[0] * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2;
	
	return dec;
}



/***********************************************************************
 * parse_command
 *
 * DESCRIPTION
 *  Parse @assembly, and put each assembly token into @tokens[] and the number of
 *  tokes into @nr_tokens. You may use this implemention or your own from PA0.
 *
 * A assembly token is defined as a string without any whitespace (i.e., *space*
 * and *tab* in this programming assignment). For exmaple,
 *   command = "  add t1   t2 s0 "
 *
 * then, nr_tokens = 4, and tokens is
 *   tokens[0] = "add"
 *   tokens[1] = "t0"
 *   tokens[2] = "t1"
 *   tokens[3] = "s0"
 *
 * You can assume that the input string is all lowercase for testing.
 *
 * RETURN VALUE
 *  Return 0 after filling in @nr_tokens and @tokens[] properly
 *
 */
static bool __is_separator(char *c)
{
	char *separators = " \t\r\n,.";

	for (size_t i = 0; i < strlen(separators); i++) {
		if (*c == separators[i]) return true;
	}

	return false;
}
static int parse_command(char *assembly, int *nr_tokens, char *tokens[])
{
	char *curr = assembly;
	int token_started = false;
	*nr_tokens = 0;

	while (*curr != '\0') {
		if (__is_separator(curr)) {
			*curr = '\0';
			token_started = false;
		}
		else {
			if (!token_started) {
				tokens[*nr_tokens] = curr;
				*nr_tokens += 1;
				token_started = true;
			}
		}
		curr++;
	}

	return 0;
}



/*====================================================================*/
/*          ****** DO NOT MODIFY ANYTHING FROM THIS LINE ******       */

/***********************************************************************
 * The main function of this program.
 */
int main(int argc, char * const argv[])
{
	char assembly[MAX_ASSEMBLY] = { '\0' };
	FILE *input = stdin;

	if (argc > 1) {
		input = fopen(argv[1], "r");
		if (!input) {
			fprintf(stderr, "No input file %s\n", argv[0]);
			return EXIT_FAILURE;
		}
	}

	if (input == stdin) {
		printf("*********************************************************\n");
		printf("*          >> SCE212 MIPS translator  v0.01 <<          *\n");
		printf("*                                                       *\n");
		printf("*                                       .---.           *\n");
		printf("*                           .--------.  |___|           *\n");
		printf("*                           |.------.|  |=. |           *\n");
		printf("*                           || >>_  ||  |-- |           *\n");
		printf("*                           |'------'|  |   |           *\n");
		printf("*                           ')______('~~|___|           *\n");
		printf("*                                                       *\n");
		printf("*                                   Fall 2019           *\n");
		printf("*********************************************************\n\n");
		printf(">> ");
	}

	while (fgets(assembly, sizeof(assembly), input)) {
		char *tokens[MAX_NR_TOKENS] = { NULL };
		int nr_tokens = 0;
		unsigned int machine_code;

		for (size_t i = 0; i < strlen(assembly); i++) {
			assembly[i] = tolower(assembly[i]);
		}

		if (parse_command(assembly, &nr_tokens, tokens) < 0)
			continue;

		machine_code = translate(nr_tokens, tokens);

		fprintf(stderr, "0x%08x\n", machine_code);

		if (input == stdin) printf(">> ");
	}

	if (input != stdin) fclose(input);

	return EXIT_SUCCESS;
}