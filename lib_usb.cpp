/* 
Copyright 2013 Brad Quick

Based one code Copyright (c) 2010, Peter Barrett  

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

** Brad: This file was pretty much hacked from the arduino core.  It will allow the
** usb port to be used for serial comunication. In general, don't call these functions directly.
** Instead, use lib_serial with port number USBPORTNUMBER
*/

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "projectsettings.h"
#include "lib_usb.h"

#include "lib_timers.h"

#if (defined(USBCON) && defined(USESERIALPORTUSB))

#define USB_PID 0x8036
#define USB_VID 0x2341

#define EP_TYPE_CONTROL            0x00
#define EP_TYPE_BULK_IN            0x81
#define EP_TYPE_BULK_OUT         0x80
#define EP_TYPE_INTERRUPT_IN      0xC1
#define EP_TYPE_INTERRUPT_OUT      0xC0
#define EP_TYPE_ISOCHRONOUS_IN      0x41
#define EP_TYPE_ISOCHRONOUS_OUT      0x40

/** Pulse generation counters to keep track of the number of milliseconds remaining for each pulse type */
#define TX_RX_LED_PULSE_MS 100
volatile u8 TxLEDPulse; /**< Milliseconds remaining for data Tx LED pulse */
volatile u8 RxLEDPulse; /**< Milliseconds remaining for data Rx LED pulse */

//==================================================================
//==================================================================

extern const u16 STRING_LANGUAGE[] PROGMEM;
extern const u16 STRING_IPRODUCT[] PROGMEM;
extern const u16 STRING_IMANUFACTURER[] PROGMEM;
extern const DeviceDescriptor USB_DeviceDescriptor PROGMEM;
extern const DeviceDescriptor USB_DeviceDescriptorA PROGMEM;

const u16 STRING_LANGUAGE[2] = {
   (3<<8) | (2+2),
   0x0409   // English
};

const u16 STRING_IPRODUCT[17] = {
   (3<<8) | (2+2*16),
#if USB_PID == 0x8036   
   'A','r','d','u','i','n','o',' ','L','e','o','n','a','r','d','o'
#elif USB_PID == 0x8037
   'A','r','d','u','i','n','o',' ','M','i','c','r','o',' ',' ',' '
#elif USB_PID == 0x9208
   'L','i','l','y','P','a','d','U','S','B',' ',' ',' ',' ',' ',' '
#else
   'U','S','B',' ','I','O',' ','B','o','a','r','d',' ',' ',' ',' '
#endif
};

const u16 STRING_IMANUFACTURER[12] = {
   (3<<8) | (2+2*11),
#if USB_VID == 0x2341
   'A','r','d','u','i','n','o',' ','L','L','C'
#elif USB_VID == 0x1b4f
   'S','p','a','r','k','F','u','n',' ',' ',' '
#else
   'U','n','k','n','o','w','n',' ',' ',' ',' '
#endif
};

#ifdef CDC_ENABLED
#define DEVICE_CLASS 0x02
#else
#define DEVICE_CLASS 0x00
#endif

//   DEVICE DESCRIPTOR
const DeviceDescriptor USB_DeviceDescriptor =
   D_DEVICE(0x00,0x00,0x00,64,USB_VID,USB_PID,0x100,IMANUFACTURER,IPRODUCT,0,1);

const DeviceDescriptor USB_DeviceDescriptorA =
   D_DEVICE(DEVICE_CLASS,0x00,0x00,64,USB_VID,USB_PID,0x100,IMANUFACTURER,IPRODUCT,0,1);

//==================================================================
//==================================================================

typedef struct
   {
   u32   dwDTERate;
   u8   bCharFormat;
   u8    bParityType;
   u8    bDataBits;
   u8   lineState;
   } LineInfo;

static volatile LineInfo _usbLineInfo = { 57600, 0x00, 0x00, 0x00, 0x00 };

#define WEAK __attribute__ ((weak))

extern const CDCDescriptor _cdcInterface PROGMEM;
const CDCDescriptor _cdcInterface =
   {
   D_IAD(0,2,CDC_COMMUNICATION_INTERFACE_CLASS,CDC_ABSTRACT_CONTROL_MODEL,1),

   //   CDC communication interface
   D_INTERFACE(CDC_ACM_INTERFACE,1,CDC_COMMUNICATION_INTERFACE_CLASS,CDC_ABSTRACT_CONTROL_MODEL,0),
   D_CDCCS(CDC_HEADER,0x10,0x01),                        // Header (1.10 bcd)
   D_CDCCS(CDC_CALL_MANAGEMENT,1,1),                     // Device handles call management (not)
   D_CDCCS4(CDC_ABSTRACT_CONTROL_MANAGEMENT,6),            // SET_LINE_CODING, GET_LINE_CODING, SET_CONTROL_LINE_STATE supported
   D_CDCCS(CDC_UNION,CDC_ACM_INTERFACE,CDC_DATA_INTERFACE),   // Communication interface is master, data interface is slave 0
   D_ENDPOINT(USB_ENDPOINT_IN (CDC_ENDPOINT_ACM),USB_ENDPOINT_TYPE_INTERRUPT,0x10,0x40),

   //   CDC data interface
   D_INTERFACE(CDC_DATA_INTERFACE,2,CDC_DATA_INTERFACE_CLASS,0,0),
   D_ENDPOINT(USB_ENDPOINT_OUT(CDC_ENDPOINT_OUT),USB_ENDPOINT_TYPE_BULK,0x40,0),
   D_ENDPOINT(USB_ENDPOINT_IN (CDC_ENDPOINT_IN ),USB_ENDPOINT_TYPE_BULK,0x40,0)
   };

int WEAK CDC_GetInterface(u8* interfaceNum)
   {
   interfaceNum[0] += 2;   // uses 2
   return USB_SendControl(TRANSFER_PGM,&_cdcInterface,sizeof(_cdcInterface));
   }

bool WEAK CDC_Setup(Setup& setup)
   {
   u8 r = setup.bRequest;
   u8 requestType = setup.bmRequestType;

   if (REQUEST_DEVICETOHOST_CLASS_INTERFACE == requestType)
      {
      if (CDC_GET_LINE_CODING == r)
         {
         USB_SendControl(0,(void*)&_usbLineInfo,7);
         return true;
         }
      }

   if (REQUEST_HOSTTODEVICE_CLASS_INTERFACE == requestType)
      {
      if (CDC_SET_LINE_CODING == r)
         {
         USB_RecvControl((void*)&_usbLineInfo,7);
         return true;
         }

      if (CDC_SET_CONTROL_LINE_STATE == r)
         {
         _usbLineInfo.lineState = setup.wValueL;

         // auto-reset into the bootloader is triggered when the port, already 
         // open at 1200 bps, is closed.  this is the signal to start the watchdog
         // with a relatively long period so it can finish housekeeping tasks
         // like servicing endpoints before the sketch ends
         if (1200 == _usbLineInfo.dwDTERate)
            {
            // We check DTR state to determine if host port is open (bit 0 of lineState).
            if ((_usbLineInfo.lineState & 0x01) == 0)
               {
               *(uint16_t *)0x0800 = 0x7777;
               wdt_enable(WDTO_120MS);
               }
            else
               {
               // Most OSs do some intermediate steps when configuring ports and DTR can
               // twiggle more than once before stabilizing.
               // To avoid spurious resets we set the watchdog to 250ms and eventually
               // cancel if DTR goes back high.
   
               wdt_disable();
               wdt_reset();
               *(uint16_t *)0x0800 = 0x0;
               }
            }
         return true;
         }
      }
   return false;
   }

volatile u8 _usbConfiguration = 0;

static inline void WaitIN(void)
   {
   while (!(UEINTX & (1<<TXINI)));
   }

static inline void ClearIN(void)
   {
   UEINTX = ~(1<<TXINI);
   }

static inline void WaitOUT(void)
   {
   while (!(UEINTX & (1<<RXOUTI)));
   }

static inline u8 WaitForINOrOUT()
   {
   while (!(UEINTX & ((1<<TXINI)|(1<<RXOUTI))));
   return (UEINTX & (1<<RXOUTI)) == 0;
   }

static inline void ClearOUT(void)
   {
   UEINTX = ~(1<<RXOUTI);
   }

void Recv(volatile u8* data, u8 count)
   {
   while (count--)
      *data++ = UEDATX;
   
   RXLED1;               // light the RX LED
   RxLEDPulse = TX_RX_LED_PULSE_MS;   
   }

static inline u8 Recv8()
   {
   RXLED1;               // light the RX LED
   RxLEDPulse = TX_RX_LED_PULSE_MS;

   return UEDATX;   
   }

static inline void Send8(u8 d)
   {
   UEDATX = d;
   }

static inline void SetEP(u8 ep)
   {
   UENUM = ep;
   }

static inline u8 FifoByteCount()
   {
   return UEBCLX;
   }

static inline u8 ReceivedSetupInt()
   {
   return UEINTX & (1<<RXSTPI);
   }

static inline void ClearSetupInt()
   {
   UEINTX = ~((1<<RXSTPI) | (1<<RXOUTI) | (1<<TXINI));
   }

static inline void Stall()
   {
   UECONX = (1<<STALLRQ) | (1<<EPEN);
   }

static inline u8 ReadWriteAllowed()
   {
   return UEINTX & (1<<RWAL);
   }

static inline u8 Stalled()
   {
   return UEINTX & (1<<STALLEDI);
   }

static inline u8 FifoFree()
   {
   return UEINTX & (1<<FIFOCON);
   }

static inline void ReleaseRX()
   {
   UEINTX = 0x6B;   // FIFOCON=0 NAKINI=1 RWAL=1 NAKOUTI=0 RXSTPI=1 RXOUTI=0 STALLEDI=1 TXINI=1
   }

static inline void ReleaseTX()
   {
   UEINTX = 0x3A;   // FIFOCON=0 NAKINI=0 RWAL=1 NAKOUTI=1 RXSTPI=1 RXOUTI=0 STALLEDI=1 TXINI=0
   }

static inline u8 FrameNumber()
   {
   return UDFNUML;
   }

//==================================================================
//==================================================================

u8 USBGetConfiguration(void)
   {
   return _usbConfiguration;
   }

#define USB_RECV_TIMEOUT

class LockEP
   {
   u8 _sreg;
public:
   LockEP(u8 ep) : _sreg(SREG)
      {
      cli();
      SetEP(ep & 7);
      }
   ~LockEP()
      {
      SREG = _sreg;
      }
   };

//   Number of bytes, assumes a rx endpoint
u8 lib_usb_charsavailable(u8 ep)
   {
   LockEP lock(ep);
   return FifoByteCount();
   }

//   Non Blocking receive
//   Return number of bytes read
int lib_usb_recv(u8 ep, void* d, int len)
   {
   if (!_usbConfiguration || len < 0)
      return -1;
   
   LockEP lock(ep);
   u8 n = FifoByteCount();
   if (n<len) len=n;
//   len = min(n,len);
   n = len;
   u8* dst = (u8*)d;
   while (n--)
      *dst++ = Recv8();
   if (len && !FifoByteCount())   // release empty buffer
      ReleaseRX();
   
   return len;
   }

//   Recv 1 byte if ready
int lib_usb_recv(u8 ep)
   {
   u8 c;
   if (lib_usb_recv(ep,&c,1) != 1)
      return -1;
   return c;
   }

//   Space in send EP
u8 USB_SendSpace(u8 ep)
   {
   LockEP lock(ep);
   if (!ReadWriteAllowed())
      return 0;
   return 64 - FifoByteCount();
   }

//   Blocking Send of data to an endpoint
int lib_usb_send(u8 ep, const void* d, int len)
   {
   if (!_usbConfiguration)
      return -1;

   int r = len;
   const u8* data = (const u8*)d;
   u8 zero = ep & TRANSFER_ZERO;
   u8 timeout = 250;      // 250ms timeout on send? TODO
   while (len)
      {
      u8 n = USB_SendSpace(ep);
      if (n == 0)
         {
         if (!(--timeout))
            return -1;
            lib_timers_delaymilliseconds(1);
         continue;
         }

      if (n > len)
         n = len;
      len -= n;
         {
         LockEP lock(ep);
         if (ep & TRANSFER_ZERO)
            {
            while (n--)
               Send8(0);
            }
         else if (ep & TRANSFER_PGM)
            {
            while (n--)
               Send8(pgm_read_byte(data++));
            }
         else
            {
            while (n--)
               Send8(*data++);
            }
         if (!ReadWriteAllowed() || ((len == 0) && (ep & TRANSFER_RELEASE)))   // Release full buffer
            ReleaseTX();
         }
      }
   TXLED1;               // light the TX LED
   TxLEDPulse = TX_RX_LED_PULSE_MS;
   return r;
   }

extern const u8 _initEndpoints[] PROGMEM;
const u8 _initEndpoints[] = 
   {
   0,
   
#ifdef CDC_ENABLED
   EP_TYPE_INTERRUPT_IN,      // CDC_ENDPOINT_ACM
   EP_TYPE_BULK_OUT,         // CDC_ENDPOINT_OUT
   EP_TYPE_BULK_IN,         // CDC_ENDPOINT_IN
#endif

#ifdef HID_ENABLED
   EP_TYPE_INTERRUPT_IN      // HID_ENDPOINT_INT
#endif
   };

#define EP_SINGLE_64 0x32   // EP0
#define EP_DOUBLE_64 0x36   // Other endpoints

static
void InitEP(u8 index, u8 type, u8 size)
   {
   UENUM = index;
   UECONX = 1;
   UECFG0X = type;
   UECFG1X = size;
   }

static
void InitEndpoints()
   {
   for (u8 i = 1; i < sizeof(_initEndpoints); i++)
      {
      UENUM = i;
      UECONX = 1;
      UECFG0X = pgm_read_byte(_initEndpoints+i);
      UECFG1X = EP_DOUBLE_64;
      }
   UERST = 0x7E;   // And reset them
   UERST = 0;
   }

//   Handle CLASS_INTERFACE requests
static
bool ClassInterfaceRequest(Setup& setup)
   {
   u8 i = setup.wIndex;

#ifdef CDC_ENABLED
   if (CDC_ACM_INTERFACE == i)
      return CDC_Setup(setup);
#endif

#ifdef HID_ENABLED
   if (HID_INTERFACE == i)
      return HID_Setup(setup);
#endif
   return false;
   }

int _cmark;
int _cend;
void InitControl(int end)
   {
   SetEP(0);
   _cmark = 0;
   _cend = end;
   }

static
bool SendControl(u8 d)
   {
   if (_cmark < _cend)
      {
      if (!WaitForINOrOUT())
         return false;
      Send8(d);
      if (!((_cmark + 1) & 0x3F))
         ClearIN();   // Fifo is full, release this packet
      }
   _cmark++;
   return true;
   };

//   Clipped by _cmark/_cend
int USB_SendControl(u8 flags, const void* d, int len)
   {
   int sent = len;
   const u8* data = (const u8*)d;
   bool pgm = flags & TRANSFER_PGM;
   while (len--)
      {
      u8 c = pgm ? pgm_read_byte(data++) : *data++;
      if (!SendControl(c))
         return -1;
      }
   return sent;
   }

//   Does not timeout or cross fifo boundaries
//   Will only work for transfers <= 64 bytes
//   TODO
int USB_RecvControl(void* d, int len)
   {
   WaitOUT();
   Recv((u8*)d,len);
   ClearOUT();
   return len;
   }

int SendInterfaces()
   {
   int total = 0;
   u8 interfaces = 0;

#ifdef CDC_ENABLED
   total = CDC_GetInterface(&interfaces);
#endif

#ifdef HID_ENABLED
   total += HID_GetInterface(&interfaces);
#endif

   return interfaces;
   }

//   Construct a dynamic configuration descriptor
//   This really needs dynamic endpoint allocation etc
//   TODO
static
bool SendConfiguration(int maxlen)
   {
   //   Count and measure interfaces
   InitControl(0);   
   int interfaces = SendInterfaces();
   ConfigDescriptor config = D_CONFIG(_cmark + sizeof(ConfigDescriptor),interfaces);

   //   Now send them
   InitControl(maxlen);
   USB_SendControl(0,&config,sizeof(ConfigDescriptor));
   SendInterfaces();
   return true;
   }

u8 _cdcComposite = 0;

static
bool SendDescriptor(Setup& setup)
   {
   u8 t = setup.wValueH;
   if (USB_CONFIGURATION_DESCRIPTOR_TYPE == t)
      return SendConfiguration(setup.wLength);

   InitControl(setup.wLength);
#ifdef HID_ENABLED
   if (HID_REPORT_DESCRIPTOR_TYPE == t)
      return HID_GetDescriptor(t);
#endif

   u8 desc_length = 0;
   const u8* desc_addr = 0;
   if (USB_DEVICE_DESCRIPTOR_TYPE == t)
      {
      if (setup.wLength == 8)
         _cdcComposite = 1;
      desc_addr = _cdcComposite ?  (const u8*)&USB_DeviceDescriptorA : (const u8*)&USB_DeviceDescriptor;
      }
   else if (USB_STRING_DESCRIPTOR_TYPE == t)
      {
      if (setup.wValueL == 0)
         desc_addr = (const u8*)&STRING_LANGUAGE;
      else if (setup.wValueL == IPRODUCT) 
         desc_addr = (const u8*)&STRING_IPRODUCT;
      else if (setup.wValueL == IMANUFACTURER)
         desc_addr = (const u8*)&STRING_IMANUFACTURER;
      else
         return false;
      }

   if (desc_addr == 0)
      return false;
   if (desc_length == 0)
      desc_length = pgm_read_byte(desc_addr);

   USB_SendControl(TRANSFER_PGM,desc_addr,desc_length);
   return true;
   }

//   Endpoint 0 interrupt
ISR(USB_COM_vect)
   {
    SetEP(0);
   if (!ReceivedSetupInt())
      return;

   Setup setup;
   Recv((u8*)&setup,8);
   ClearSetupInt();

   u8 requestType = setup.bmRequestType;
   if (requestType & REQUEST_DEVICETOHOST)
      WaitIN();
   else
      ClearIN();

    bool ok = true;
   if (REQUEST_STANDARD == (requestType & REQUEST_TYPE))
      {
      //   Standard Requests
      u8 r = setup.bRequest;
      if (GET_STATUS == r)
         {
         Send8(0);      // TODO
         Send8(0);
         }
      else if (CLEAR_FEATURE == r)
         {
         }
      else if (SET_FEATURE == r)
         {
         }
      else if (SET_ADDRESS == r)
         {
         WaitIN();
         UDADDR = setup.wValueL | (1<<ADDEN);
         }
      else if (GET_DESCRIPTOR == r)
         {
         ok = SendDescriptor(setup);
         }
      else if (SET_DESCRIPTOR == r)
         {
         ok = false;
         }
      else if (GET_CONFIGURATION == r)
         {
         Send8(1);
         }
      else if (SET_CONFIGURATION == r)
         {
         if (REQUEST_DEVICE == (requestType & REQUEST_RECIPIENT))
            {
            InitEndpoints();
            _usbConfiguration = setup.wValueL;
            } else
            ok = false;
         }
      else if (GET_INTERFACE == r)
         {
         }
      else if (SET_INTERFACE == r)
         {
         }
      }
   else
      {
      InitControl(setup.wLength);      //   Max length of transfer
      ok = ClassInterfaceRequest(setup);
      }

   if (ok)
      ClearIN();
   else
      {
      Stall();
      }
   }

void USB_Flush(u8 ep)
   {
   SetEP(ep);
   if (FifoByteCount())
      ReleaseTX();
   }

//   General interrupt
ISR(USB_GEN_vect)
   {
   u8 udint = UDINT;
   UDINT = 0;

   //   End of Reset
   if (udint & (1<<EORSTI))
      {
      InitEP(0,EP_TYPE_CONTROL,EP_SINGLE_64);   // init ep0
      _usbConfiguration = 0;         // not configured yet
      UEIENX = 1 << RXSTPE;         // Enable interrupts for ep0
      }

   //   Start of Frame - happens every millisecond so we use it for TX and RX LED one-shot timing, too
   if (udint & (1<<SOFI))
      {
#ifdef CDC_ENABLED
      USB_Flush(CDC_TX);            // Send a tx frame if found
//      if (USB_Available(CDC_RX))   // Handle received bytes (if any)
//         Serial.accept();
#endif
      
      // check whether the one-shot period has elapsed.  if so, turn off the LED
      if (TxLEDPulse && !(--TxLEDPulse))
         TXLED0;
      if (RxLEDPulse && !(--RxLEDPulse))
         RXLED0;
      }
   }

//   VBUS or counting frames
//   Any frame counting?
u8 USBConnected()
   {
   u8 f = UDFNUML;
      lib_timers_delaymilliseconds(3);
   return f != UDFNUML;
   }

//=======================================================================
//=======================================================================

void lib_usb_init()
   {
   _usbConfiguration = 0;
   UHWCON = 0x01;                  // power internal reg
   USBCON = (1<<USBE)|(1<<FRZCLK);      // clock frozen, usb enabled
#if F_CPU == 16000000UL
   PLLCSR = 0x12;                  // Need 16 MHz xtal
#elif F_CPU == 8000000UL
   PLLCSR = 0x02;                  // Need 8 MHz xtal
#endif
   while (!(PLLCSR & (1<<PLOCK)))      // wait for lock pll
      ;

   // Some tests on specific versions of macosx (10.7.3), reported some
   // strange behaviuors when the board is reset using the serial
   // port touch at 1200 bps. This delay fixes this behaviour.
      lib_timers_delaymilliseconds(1);

   USBCON = ((1<<USBE)|(1<<OTGPADE));   // start USB clock
   UDIEN = (1<<EORSTE)|(1<<SOFE);      // Enable interrupts for EOR (End of Reset) and SOF (start of frame)
   UDCON = 0;                     // enable attach resistor
   
   TX_RX_LED_INIT;
   }

#endif /* if defined(USBCON) */
