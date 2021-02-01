#ifndef MM_TLK_XFS5152_H
#define MM_TLK_XFS5152_H

/**********************************************************************************
 * @File     MM_t2s_xfs5152.h
 * @AUthor   Daniel Babekuhl
 * @Date     7th June 2020
 * @Brief    This file contains a library for the XFS5152 text-to-speech module 
 *           to interface with the MiniMech main software. See MM_t2s.xfs5152.c
 *           for details of operation.
 * ********************************************************************************/

#include <stdint.h>
#include <stm8s.h>

void MM_T2S_init(void);
void MM_T2S_sendPhrase(void);
void MM_T2S_stopPhrase(void);
uint8_t MM_T2S_getStatus(void);

#endif
