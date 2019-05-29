/*!
 * \file FTDI_SPI.h
 *
 * \author FTDI
 * \date 20110523
 *
 * Copyright © 2000-2014 Future Technology Devices International Limited
 *
 *
 * THIS SOFTWARE IS PROVIDED BY FUTURE TECHNOLOGY DEVICES INTERNATIONAL LIMITED ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL FUTURE TECHNOLOGY DEVICES INTERNATIONAL LIMITED
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Project: libMPSSE
 * Module: SPI
 *
 * Rivision History:
 * 0.1  - initial version
 * 0.2  - 20110708 - added function SPI_ChangeCS, moved SPI_Read/WriteGPIO to middle layer
 * 0.3  - 20111103 - added SPI_ReadWrite
 * 0.41 - 20140903 - fixed compile warnings
 */

#ifndef FTDI_SPI_H
#define FTDI_SPI_H

#include "ftdi_infra.h"


/******************************************************************************/
/*								Macro defines								  */
/******************************************************************************/
/* Bit definition of the transferOptions parameter in SPI_Read, SPI_Write & SPI_Transfer  */

/* transferOptions-Bit0: If this bit is 0 then it means that the transfer size provided is in bytes */
#define	SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES			0x00000000
/* transferOptions-Bit0: If this bit is 1 then it means that the transfer size provided is in bytes */
#define	SPI_TRANSFER_OPTIONS_SIZE_IN_BITS			0x00000001
/* transferOptions-Bit1: if BIT1 is 1 then CHIP_SELECT line will be enabled at start of transfer */
#define	SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE		0x00000002
/* transferOptions-Bit2: if BIT2 is 1 then CHIP_SELECT line will be disabled at end of transfer */
#define SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE		0x00000004

/* Bit definition of the Options member of configOptions structure */
#define SPI_CONFIG_OPTION_MODE_MASK		0x00000003
#define SPI_CONFIG_OPTION_MODE0			0x00000000
#define SPI_CONFIG_OPTION_MODE1			0x00000001
#define SPI_CONFIG_OPTION_MODE2			0x00000002
#define SPI_CONFIG_OPTION_MODE3			0x00000003

#define SPI_CONFIG_OPTION_CS_MASK		0x0000001C		/* 111 00 */
#define SPI_CONFIG_OPTION_CS_DBUS3		0x00000000		/* 000 00 */
#define SPI_CONFIG_OPTION_CS_DBUS4		0x00000004		/* 001 00 */
#define SPI_CONFIG_OPTION_CS_DBUS5		0x00000008		/* 010 00 */
#define SPI_CONFIG_OPTION_CS_DBUS6		0x0000000C		/* 011 00 */
#define SPI_CONFIG_OPTION_CS_DBUS7		0x00000010		/* 100 00 */

#define SPI_CONFIG_OPTION_CS_ACTIVELOW	0x00000020


/******************************************************************************/
/*								Type defines								  */
/******************************************************************************/

/* Structure contains configuration information of the SPI channel. It is populated by the user
application during initialization of the channel and then it is saved in a linked-list and used
internally by other SPI functions during operation. The structure is removed from the list when
the user application calls SPI_CloseChannel */
typedef struct ChannelConfig_t
{
	uint32	ClockRate; /* SPI clock rate, value should be <= 30000000 */
	uint8	LatencyTimer; /* value in milliseconds, maximum value should be <= 255 */
	uint32	configOptions;	/* This member provides a way to enable/disable features
	specific to the protocol that are implemented in the chip
	BIT1-0=CPOL-CPHA:	00 - MODE0 - data captured on rising edge, propagated on falling
 						01 - MODE1 - data captured on falling edge, propagated on rising
 						10 - MODE2 - data captured on falling edge, propagated on rising
 						11 - MODE3 - data captured on rising edge, propagated on falling
	BIT4-BIT2: 000 - A/B/C/D_DBUS3=ChipSelect
			 : 001 - A/B/C/D_DBUS4=ChipSelect
 			 : 010 - A/B/C/D_DBUS5=ChipSelect
 			 : 011 - A/B/C/D_DBUS6=ChipSelect
 			 : 100 - A/B/C/D_DBUS7=ChipSelect
 	BIT5: ChipSelect is active high if this bit is 0
	BIT6 -BIT31		: Reserved
	*/
	uint32		Pin;/* BIT7   -BIT0:   Initial direction of the pins	*/
					/* BIT15 -BIT8:   Initial values of the pins		*/
					/* BIT23 -BIT16: Final direction of the pins		*/
					/* BIT31 -BIT24: Final values of the pins		*/
	uint16		currentPinState;/* BIT7   -BIT0:   Current direction of the pins	*/
								/* BIT15 -BIT8:   Current values of the pins	*/
}ChannelConfig;

/* This structure associates the channel configuration information to a handle stores them in the
form of a linked list */
typedef struct ChannelContext_t
{
	FT_HANDLE 		handle;
	ChannelConfig	config;
	struct ChannelContext_t *next;
}ChannelContext;


/******************************************************************************/
/*								External variables							  */
/******************************************************************************/





/******************************************************************************/
/*								Function declarations						  */
/******************************************************************************/
FTDI_API FT_STATUS SPI_GetNumChannels(uint32 *numChannels);
FTDI_API FT_STATUS SPI_GetChannelInfo(uint32 index,
	FT_DEVICE_LIST_INFO_NODE *chanInfo);
FTDI_API FT_STATUS SPI_OpenChannel(uint32 index, FT_HANDLE *handle);
FTDI_API FT_STATUS SPI_InitChannel(FT_HANDLE handle, ChannelConfig *config);
FTDI_API FT_STATUS SPI_CloseChannel(FT_HANDLE handle);
FTDI_API FT_STATUS SPI_Read(FT_HANDLE handle, uint8 *buffer,
	uint32 sizeToTransfer, uint32 *sizeTransfered, uint32 options);
FTDI_API FT_STATUS SPI_Write(FT_HANDLE handle, uint8 *buffer,
	uint32 sizeToTransfer, uint32 *sizeTransfered, uint32 options);
FTDI_API FT_STATUS SPI_ReadWrite(FT_HANDLE handle, uint8 *inBuffer,
	uint8 *outBuffer, uint32 sizeToTransfer, uint32 *sizeTransferred,
	uint32 transferOptions);
FTDI_API FT_STATUS SPI_IsBusy(FT_HANDLE handle, bool *state);
FTDI_API void Init_libMPSSE(void);
FTDI_API void Cleanup_libMPSSE(void);
FTDI_API FT_STATUS SPI_ChangeCS(FT_HANDLE handle, uint32 configOptions);
FTDI_API FT_STATUS SPI_ToggleCS(FT_HANDLE handle, bool state);

/******************************************************************************/


#endif	/*FTDI_SPI_H*/


