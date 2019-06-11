*******************************************************************************************
* @file    readme.txt                                                              
* @author  MCD Application Team                                                    
* @version V1.0                                                                    
* @date    27-August-2015                                                          
* @brief   Boundary Scan Description Language(BSDL) files for the STM32F469 and F479 MCUs.    
*******************************************************************************************
* COPYRIGHT(c) 2015 STMicroelectronics                                             
*                                                                                  
* Redistribution and use in source and binary forms, with or without modification, 
* are permitted provided that the following conditions are met:                    
*   1. Redistributions of source code must retain the above copyright notice,      
*      this list of conditions and the following disclaimer.                       
*   2. Redistributions in binary form must reproduce the above copyright notice,   
*      this list of conditions and the following disclaimer in the documentation  
*      and/or other materials provided with the distribution.                      
*   3. Neither the name of STMicroelectronics nor the names of its contributors    
*      may be used to endorse or promote products derived from this software       
*      without specific prior written permission.                                  
*                                                                                  
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"      
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE        
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE   
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE     
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL       
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR       
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER       
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,    
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE    
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.             
*******************************************************************************************

=======================
How to use BSDL Files :
=======================

The STM32F469 and F479 MCUs integrate two serially connected JTAG TAPs, the boundary scan
TAP (IR is 5-bit wide) and the CortexMx TAP (IR is 4-bit wide).

The package contains :

   + A BSDL File for the CortexMx TAP and is common to all STM32F469 and F479 devices
   + Boundary scan BSDL Files for each STM32F4xx device on different packages :
   
	  o TFBGA216
	  o LQFP208
	  o LQFP176
	  o UFBGA176
	  o UFBGA169
	  o WLCSP168

In order to run boundary scan, always provide two BSDL files to your JTAG Boundary scan tool:
the "CortexMx.bsd" and your selected "STM32Fxx_device_Package.bsd".  

WARNING : Do not combine both BSDL files in a single TAP with 9-bit wide !

For more details on the internal TAPs description refer to the Reference Manual
of the selected STM32xxxx device , Section : JTAG TAP connection.


========================
* V1.0 - 27-August-2015 
========================
  Created.
  

******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE
