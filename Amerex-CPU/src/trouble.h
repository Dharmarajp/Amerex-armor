/*
 * trouble.h
 *
 * Created: 15-07-2022 23:50:14
 *  Author: p.dharmaraj
 */


#ifndef TROUBLE_H_
#define TROUBLE_H_

extern bool any_of_trouble_g_b;
extern bool fire_iso_out_g_b;
extern bool trbl_iso_out_g_b;

void Check_any_Trbl(void);
void Trbl_Fire_Iso_Output_Reset(void);
void Trouble_Iso_Output(void);
void Fire_Iso_Output(void);
#endif /* TROUBLE_H_ */