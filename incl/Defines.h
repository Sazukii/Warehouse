#ifndef DEFINES_H
#define DEFINES_H

/*System Includes */
#include <stdexcept>

extern "C"
{
#include <pixtend.h>
#include <stdio.h>
#include <stdint.h>
}
typedef enum systemStates
{
    CALIBRATING,
    IDLE,
    SEARCHING_CONTAINER,
    WAITING_FOR_MESSAGE
} SystemStates;



#define xleft 1915
#define xmiddle 1330
#define xright 745
#define ytop 85
#define ymiddle 435
#define ybottom 820
#define xconveyor 15
#define yconveyor 700

/* DEFINES byDigitalOut0 pins */
#define BDO0_Power_Pos (uint8_t)0U
#define BDO0_Power_Msk (uint8_t)0b11
#define BDO0_Power (uint8_t)(BDO0_Power_Msk << BDO0_Power_Pos)

#define BDO0_Conveyor_FWD_Pos (uint8_t)4U
#define BDO0_Conveyor_FWD_Msk (uint8_t)0b1
#define BDO0_Conveyor_FWD (uint8_t)(BDO0_Conveyor_FWD_Msk << BDO0_Conveyor_FWD_Pos)

#define BDO0_Conveyor_BWD_Pos (uint8_t)5U
#define BDO0_Conveyor_BWD_Msk (uint8_t)0b1
#define BDO0_Conveyor_BWD (uint8_t)(BDO0_Conveyor_BWD_Msk << BDO0_Conveyor_BWD_Pos)

#define BDO0_Arm_FWD_Pos (uint8_t)6U
#define BDO0_Arm_FWD_Msk (uint8_t)0b1
#define BDO0_Arm_FWD (uint8_t)(BDO0_Arm_FWD_Msk << BDO0_Arm_FWD_Pos)

#define BDO0_Arm_BWD_Pos (uint8_t)7U
#define BDO0_Arm_BWD_Msk (uint8_t)0b1
#define BDO0_Arm_BWD (uint8_t)(BDO0_Arm_BWD_Msk << BDO0_Arm_BWD_Pos)
/* End of byDigitalOut0 pins */

/* DEFINES byDigitalOut1 pins */
#define BDO1_Motor_H_Negative_Pos (uint8_t)0U
#define BDO1_Motor_H_Negative_Msk (uint8_t)0b1
#define BDO1_Motor_H_Negative (uint8_t)(BDO1_Motor_H_Negative_Msk << BDO1_Motor_H_Negative_Pos)

#define BDO1_Motor_H_Positive_Pos (uint8_t)1U
#define BDO1_Motor_H_Positive_Msk (uint8_t)0b1
#define BDO1_Motor_H_Positive (uint8_t)(BDO1_Motor_H_Positive_Msk << BDO1_Motor_H_Positive_Pos)

#define BDO1_Motor_V_Down_Pos (uint8_t)2U
#define BDO1_Motor_V_Down_Msk (uint8_t)0b1
#define BDO1_Motor_V_Down (uint8_t)(BDO1_Motor_V_Down_Msk << BDO1_Motor_V_Down_Pos)
#define BDO1_Motor_V_Positive_Pos (uint8_t)2U
#define BDO1_Motor_V_Positive_Msk (uint8_t)0b1
#define BDO1_Motor_V_Positive (uint8_t)(BDO1_Motor_V_Positive_Msk << BDO1_Motor_V_Positive_Pos)

#define BDO1_Motor_V_Up_Pos (uint8_t)3U
#define BDO1_Motor_V_Up_Msk (uint8_t)0b1
#define BDO1_Motor_V_Up (uint8_t)(BDO1_Motor_V_Up_Msk << BDO1_Motor_V_Up_Pos)
#define BDO1_Motor_V_Negative_Pos (uint8_t)3U
#define BDO1_Motor_V_Negative_Msk (uint8_t)0b1
#define BDO1_Motor_V_Negative (uint8_t)(BDO1_Motor_V_Negative_Msk << BDO1_Motor_V_Negative_Pos)
/* End of byDigitalOut0 pins */

/* DEFINES byDigitalIn0 pins */
#define BDI0_Photocell_Inside_Pos (uint8_t)0U
#define BDI0_Photocell_Inside_Msk (uint8_t)0b1
#define BDI0_Photocell_Inside (uint8_t)(BDI0_Photocell_Inside_Msk << BDI0_Photocell_Inside_Pos)

#define BDI0_Photocell_Outside_Pos (uint8_t)1U
#define BDI0_Photocell_Outside_Msk (uint8_t)0b1
#define BDI0_Photocell_Outside (uint8_t)(BDI0_Photocell_Outside_Msk << BDI0_Photocell_Outside_Pos)

#define BDI0_TrailSensor_Bottom_Pos (uint8_t)2U
#define BDI0_TrailSensor_Bottom_Msk (uint8_t)0b1
#define BDI0_TrailSensor_Bottom (uint8_t)(BDI0_TrailSensor_Bottom_Msk << BDI0_TrailSensor_Bottom_Pos)

#define BDI0_TrailSensor_Top_Pos (uint8_t)3U
#define BDI0_TrailSensor_Top_Msk (uint8_t)0b1
#define BDI0_TrailSensor_Top (uint8_t)(BDI0_TrailSensor_Top_Msk << BDI0_TrailSensor_Top_Pos)

#define BDI0_Ref_Switch_Arm_Front_Pos (uint8_t)4U
#define BDI0_Ref_Switch_Arm_Front_Msk (uint8_t)0b1
#define BDI0_Ref_Switch_Arm_Front (uint8_t)(BDI0_Ref_Switch_Arm_Front_Msk << BDI0_Ref_Switch_Arm_Front_Pos)

#define BDI0_Ref_Switch_Arm_Back_Pos (uint8_t)5U
#define BDI0_Ref_Switch_Arm_Back_Msk (uint8_t)0b1
#define BDI0_Ref_Switch_Arm_Back (uint8_t)(BDI0_Ref_Switch_Arm_Back_Msk << BDI0_Ref_Switch_Arm_Back_Pos)

#define BDI0_Ref_Switch_Horizontal_Pos (uint8_t)6U
#define BDI0_Ref_Switch_Horizontal_Msk (uint8_t)0b1
#define BDI0_Ref_Switch_Horizontal (uint8_t)(BDI0_Ref_Switch_Horizontal_Msk << BDI0_Ref_Switch_Horizontal_Pos)

#define BDI0_Ref_Switch_Vertical_Pos (uint8_t)7U
#define BDI0_Ref_Switch_Vertical_Msk (uint8_t)0b1
#define BDI0_Ref_Switch_Vertical (uint8_t)(BDI0_Ref_Switch_Vertical_Msk << BDI0_Ref_Switch_Vertical_Pos)
/* End of byDigitalIn0 pins */

/* DEFINES byDigitalIn1 pins */
#define BDI1_Encoder_Horizontal_1_Pos (uint8_t)4U
#define BDI1_Encoder_Horizontal_1_Msk (uint8_t)0b1
#define BDI1_Encoder_Horizontal_1 (uint8_t)(BDI1_Encoder_Horizontal_1_Msk << BDI1_Encoder_Horizontal_1_Pos)

#define BDI1_Encoder_Horizontal_2_Pos (uint8_t)5U
#define BDI1_Encoder_Horizontal_2_Msk (uint8_t)0b1
#define BDI1_Encoder_Horizontal_2 (uint8_t)(BDI1_Encoder_Horizontal_2_Msk << BDI1_Encoder_Horizontal_2_Pos)

#define BDI1_Encoder_Vertical_1_Pos (uint8_t)6U
#define BDI1_Encoder_Vertical_1_Msk (uint8_t)0b1
#define BDI1_Encoder_Vertical_1 (uint8_t)(BDI1_Encoder_Vertical_1_Msk << BDI1_Encoder_Vertical_1_Pos)

#define BDI1_Encoder_Vertical_2_Pos (uint8_t)7U
#define BDI1_Encoder_Vertical_2_Msk (uint8_t)0b1
#define BDI1_Encoder_Vertical_2 (uint8_t)(BDI1_Encoder_Vertical_2_Msk << BDI1_Encoder_Vertical_2_Pos)
/* End of byDigitalIn1 pins */
#endif