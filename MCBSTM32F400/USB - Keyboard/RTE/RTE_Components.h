
/*
 * Auto generated Run-Time-Environment Component Configuration File
 *      *** Do not modify ! ***
 *
 * Project: 'Keyboard' 
 * Target:  'Debug' 
 */

#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H


/*
 * Define the Device Header File: 
 */
#define CMSIS_device_header "stm32f4xx.h"

#define RTE_CMSIS_RTOS                  /* CMSIS-RTOS */
        #define RTE_CMSIS_RTOS_RTX5             /* CMSIS-RTOS Keil RTX5 */
#define RTE_CMSIS_RTOS2                 /* CMSIS-RTOS2 */
        #define RTE_CMSIS_RTOS2_RTX5            /* CMSIS-RTOS2 Keil RTX5 */
#define RTE_Compiler_EventRecorder
          #define RTE_Compiler_EventRecorder_DAP
#define RTE_Compiler_IO_File            /* Compiler I/O: File */
          #define RTE_Compiler_IO_File_FS         /* Compiler I/O: File (File System) */
#define RTE_Compiler_IO_STDOUT          /* Compiler I/O: STDOUT */
          #define RTE_Compiler_IO_STDOUT_User     /* Compiler I/O: STDOUT User */
#define RTE_DEVICE_FRAMEWORK_CLASSIC
#define RTE_DEVICE_HAL_ADC
#define RTE_DEVICE_HAL_COMMON
#define RTE_DEVICE_HAL_CORTEX
#define RTE_DEVICE_HAL_DMA
#define RTE_DEVICE_HAL_GPIO
#define RTE_DEVICE_HAL_PWR
#define RTE_DEVICE_HAL_RCC
#define RTE_DEVICE_HAL_SRAM
#define RTE_DEVICE_STARTUP_STM32F4XX    /* Device Startup for STM32F4 */
#define RTE_Driver_NAND_MemoryBus       /* Driver NAND Flash on Memory Bus */
#define RTE_Drivers_USBH0               /* Driver USBH0 */
#define RTE_Drivers_USBH1               /* Driver USBH1 */
#define RTE_FileSystem_Core             /* File System Core */
          #define RTE_FileSystem_LFN              /* File System with Long Filename support */
          #define RTE_FileSystem_Release          /* File System Release Version */
#define RTE_FileSystem_Drive_NAND_0     /* File System NAND Flash Drive 0 */
#define RTE_USB_Core                    /* USB Core */
          #define RTE_USB_Core_Debug              /* USB Core Debug Version */
#define RTE_USB_Host_0                  /* USB Host 0 */
#define RTE_USB_Host_1                  /* USB Host 1 */
#define RTE_USB_Host_HID                /* USB Host HID */

#endif /* RTE_COMPONENTS_H */
