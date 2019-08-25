/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   httpTrx2.h
 * Author: jcaf
 *
 * Created on August 21, 2019, 12:09 PM
 */

#ifndef HTTPTRX2_H
#define HTTPTRX2_H

typedef struct _HTTPTRX
{
    char *URI;
    char *Host;
    char *HdrLine;
}HTTPTRX;

typedef struct _JSON
{
    char *name;
    char *txtval;
}JSON;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* HTTPTRX2_H */

