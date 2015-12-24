#ifndef _VARIANT_PLATFORM_H
#define _VARIANT_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#undef F_CPU

#include <contiki.h>
#include <net/netstack.h>
#include <dev/watchdog.h>
#include <dev/uart0.h>
#include <dev/uart1.h>
#include <lib/random.h>

#include <jendefs.h>
#include <MicroSpecific.h>

#define F_INTERNAL_SYSCLOCK (F_CPU/2)

#define SystemCoreClock F_INTERNAL_SYSCLOCK

#define __disable_irq() MICRO_DISABLE_INTERRUPTS()
#define __enable_irq()  MICRO_ENABLE_INTERRUPTS()

#define   US_MR_CHRL_5_BIT (0x0u << 6) /**< \brief (US_MR) Character length is 5 bits */
#define   US_MR_CHRL_6_BIT (0x1u << 6) /**< \brief (US_MR) Character length is 6 bits */
#define   US_MR_CHRL_7_BIT (0x2u << 6) /**< \brief (US_MR) Character length is 7 bits */
#define   US_MR_CHRL_8_BIT (0x3u << 6) /**< \brief (US_MR) Character length is 8 bits */
#define   US_MR_NBSTOP_1_BIT (0x0u << 12) /**< \brief (US_MR) 1 stop bit */
#define   US_MR_NBSTOP_1_5_BIT (0x1u << 12) /**< \brief (US_MR) 1.5 stop bit (SYNC = 0) or reserved (SYNC = 1) */
#define   US_MR_NBSTOP_2_BIT (0x2u << 12) /**< \brief (US_MR) 2 stop bits */
#define   UART_MR_PAR_EVEN (0x0u << 9) /**< \brief (UART_MR) Even parity */
#define   UART_MR_PAR_ODD (0x1u << 9) /**< \brief (UART_MR) Odd parity */
#define   UART_MR_PAR_SPACE (0x2u << 9) /**< \brief (UART_MR) Space: parity forced to 0 */
#define   UART_MR_PAR_MARK (0x3u << 9) /**< \brief (UART_MR) Mark: parity forced to 1 */
#define   UART_MR_PAR_NO (0x4u << 9) /**< \brief (UART_MR) No parity */
typedef uint8_t Uart;

typedef uint8_t IRQn_Type;

typedef uint8_t Pio;
//typedef enum { PioType_NONE } EPioType;

#ifdef __cplusplus
}
#endif

#endif //PLATFORM_DEPEND_INCLUDES_H
