/*
 * Copyright (c) 2020-2021 ndeadly
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <switch.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
typedef struct {
    Service s;
  } McClientIfSession;

typedef struct {
    Service s;
} McClientEpSession;

void mcUsbHsEpClose(McClientEpSession* s);
void mcUsbHsIfClose(McClientIfSession* s);
*/

/* usb::hs::IClientEpSession */
Result usbHsReOpenFwd(UsbHsClientEpSession *client_ep);
Result usbHsCloseFwd(UsbHsClientEpSession *client_ep);
Result usbHsGetCompletionEventFwd(UsbHsClientEpSession *client_ep, Handle *out_handle);
Result usbHsPopulateRingFwd(UsbHsClientEpSession *client_ep);
Result usbHsPostBufferAsyncFwd(UsbHsClientEpSession *client_ep, u32 *xferId, u32 size, u64 buffer, u64 id);
Result usbHsGetXferReportFwd(UsbHsClientEpSession *client_ep, u32 *count, u32 max_reports, UsbHsXferReport *out_reports);
Result usbHsBatchBufferAsyncFwd(UsbHsClientEpSession *client_ep, u32 *xferId,u32 urbCount, u32 unk1, u32 unk2, u64 buffer, u64 id, const u32 *sizes);
Result usbHsCreateSmmuSpaceFwd(UsbHsClientEpSession *client_ep, u32 size, u64 buffer);
Result usbHsShareReportRingFwd(UsbHsClientEpSession *client_ep, u32 size, Handle handle);

/* usb::hs::IClientIfSession */
Result usbHsGetStateChangeEventFwd(UsbHsClientIfSession *client_if, Handle *out_handle);
Result usbHsSetInterfaceFwd(UsbHsClientIfSession *client_if, u8 id, UsbHsInterfaceInfo *out_info);
Result usbHsGetInterfaceFwd(UsbHsClientIfSession *client_if, UsbHsInterfaceInfo *out_info);
Result usbHsGetAlternateInterfaceFwd(UsbHsClientIfSession *client_if, u8 id, UsbHsInterfaceInfo *out_info);
Result usbHsGetCurrentFrameFwd(UsbHsClientIfSession *client_if, u32 *out);
Result usbHsCtrlXferAsyncFwd(UsbHsClientIfSession *client_if, u8 bmRequestType, u8 bRequest, u16 wValue, u16 wIndex, u16 wLength, u64 buffer);
Result usbHsGetCtrlXferCompletionEventFwd(UsbHsClientIfSession *client_if, Handle *out_handle);
Result usbHsGetCtrlXferReportFwd(UsbHsClientIfSession *client_if, UsbHsXferReport *out_report);
Result usbHsResetDeviceFwd(UsbHsClientIfSession *client_if);
//Result usbHsOpenUsbEpFwd(UsbHsClientIfSession *client_if, UsbHsClientEpSession *client_ep, u16 maxUrbCount, u32 epType, u32 epNumber, u32 epDirection, u32 maxXferSize);
Result usbHsOpenUsbEpFwd(UsbHsClientIfSession *client_if, UsbHsClientEpSession *client_ep, u16 maxUrbCount, u32 epType, u32 epNumber, u32 epDirection, u32 maxXferSize, struct usb_endpoint_descriptor *out_desc);


/* usb::hs::IClientRootSession */
Result usbHsBindClientProcessFwd(Service *s, Handle handle);
Result usbHsQueryAllInterfacesFwd(Service *s, const UsbHsInterfaceFilter *filter, size_t max_interfaces, UsbHsInterface *out_interfaces, s32 *out_total);
Result usbHsQueryAvailableInterfacesFwd(Service *s, const UsbHsInterfaceFilter *filter, size_t max_interfaces, UsbHsInterface *out_interfaces, s32 *out_total);
Result usbHsQueryAcquiredInterfacesFwd(Service *s, size_t max_interfaces, UsbHsInterface *out_interfaces, s32 *out_total);
Result usbHsCreateInterfaceAvailableEventFwd(Service *s, u8 index, const UsbHsInterfaceFilter *filter, Handle *out_handle);
Result usbHsDestroyInterfaceAvailableEventFwd(Service *s, u8 index);
Result usbHsGetInterfaceStateChangeEventFwd(Service *s, Handle *out_handle);
//Result usbHsAcquireUsbIfFwd(Service *s, UsbHsClientIfSession *client_if);
Result usbHsAcquireUsbIfFwd(Service *s, UsbHsClientIfSession *client_if, u32 id, void *buffer1, size_t buffer1_size, void *buffer2, size_t buffer2_size);
Result usbHsSetTestModeFwd(Service *s, u32 unk1, u32 unk2, u32 unk3, s32 *out);

#ifdef __cplusplus
}
#endif
