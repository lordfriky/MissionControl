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
#include "usbhs_shim.h"
#include <stratosphere/sf/sf_mitm_dispatch.h>

void mcUsbHsEpClose(UsbHsClientEpSession* s) {
    serviceClose(&s->s);
}

void mcUsbHsIfClose(UsbHsClientIfSession* s) {
    serviceClose(&s->s);
}

/* usb::hs::IClientEpSession */

Result usbHsReOpenFwd(UsbHsClientEpSession *client_ep) {
    serviceAssumeDomain(&client_ep->s);
    return serviceDispatch(&client_ep->s, 0);
}

Result usbHsCloseFwd(UsbHsClientEpSession *client_ep) {
    serviceAssumeDomain(&client_ep->s);
    return serviceDispatch(&client_ep->s, 1);
}

Result usbHsGetCompletionEventFwd(UsbHsClientEpSession *client_ep, Handle *out_handle) {
    serviceAssumeDomain(&client_ep->s);
    return serviceDispatch(&client_ep->s, 2,
        .out_handle_attrs = { SfOutHandleAttr_HipcCopy },
        .out_handles = out_handle,
    );
}

Result usbHsPopulateRingFwd(UsbHsClientEpSession *client_ep) {
    serviceAssumeDomain(&client_ep->s);
    return serviceDispatch(&client_ep->s, 3);
}

Result usbHsPostBufferAsyncFwd(UsbHsClientEpSession *client_ep, u32 *xferId, u32 size, u64 buffer, u64 id) {
    const struct {
        u32 size;
        u32 pad;
        u64 buffer;
        u64 id;
    } in = { size, 0, (u64)buffer, id };

    serviceAssumeDomain(&client_ep->s);
    return serviceDispatchInOut(&client_ep->s, 4, in, *xferId);
}

Result usbHsGetXferReportFwd(UsbHsClientEpSession *client_ep, u32 *count, u32 max_reports, UsbHsXferReport *out_reports) {
    serviceAssumeDomain(&client_ep->s);
    return serviceDispatchInOut(&client_ep->s, 5, max_reports, *count,
        .buffer_attrs = { (hosversionBefore(3,0,0) ? SfBufferAttr_HipcMapAlias : SfBufferAttr_HipcAutoSelect) | SfBufferAttr_Out },
        .buffers = { { out_reports, max_reports * sizeof(UsbHsXferReport) } },
    );
}

Result usbHsBatchBufferAsyncFwd(UsbHsClientEpSession *client_ep, u32 *xferId, u32 urbCount, u32 unk1, u32 unk2, u64 buffer, u64 id, const u32 *sizes) {
    const struct {
        u32 urbCount;
        u32 unk1;
        u32 unk2;
        u32 pad;
        u64 buffer;
        u64 id;
    } in = { urbCount, unk1, unk2, 0, buffer, id };
    
    serviceAssumeDomain(&client_ep->s);
    return serviceDispatchInOut(&client_ep->s, 6, in, *xferId,
        .buffer_attrs = { (hosversionBefore(3,0,0) ? SfBufferAttr_HipcMapAlias : SfBufferAttr_HipcAutoSelect) | SfBufferAttr_In },
        .buffers = { { sizes, urbCount * sizeof(u32) } },
    );
}

Result usbHsCreateSmmuSpaceFwd(UsbHsClientEpSession *client_ep, u32 size, u64 buffer) {
    const struct {
        u32 size;
        u32 pad;
        u64 buffer;
    } in = { size, 0, buffer };

    serviceAssumeDomain(&client_ep->s);
    return serviceDispatchIn(&client_ep->s, 7, in);
}

Result usbHsShareReportRingFwd(UsbHsClientEpSession *client_ep, u32 size, Handle handle) {
    serviceAssumeDomain(&client_ep->s);
    return serviceDispatchIn(&client_ep->s, 8, size,
        .in_num_handles = 1,
        .in_handles = { handle }
    );
}

/* usb::hs::IClientIfSession */

Result usbHsGetStateChangeEventFwd(UsbHsClientIfSession *client_if, Handle *out_handle) {
    serviceAssumeDomain(&client_if->s);
    return serviceDispatch(&client_if->s, 0, 
        .out_handle_attrs = { SfOutHandleAttr_HipcCopy },
        .out_handles = out_handle,
    );
}

Result usbHsSetInterfaceFwd(UsbHsClientIfSession *client_if, u8 id, UsbHsInterfaceInfo *out_info) {
    serviceAssumeDomain(&client_if->s);
    return serviceDispatchIn(&client_if->s, 1, id,
        .buffer_attrs = { SfBufferAttr_HipcMapAlias | SfBufferAttr_Out },
        .buffers = { { out_info, sizeof(UsbHsInterfaceInfo) } },
    );
}

Result usbHsGetInterfaceFwd(UsbHsClientIfSession *client_if, UsbHsInterfaceInfo *out_info) {
    serviceAssumeDomain(&client_if->s);
    return serviceDispatch(&client_if->s, 2,
        .buffer_attrs = { SfBufferAttr_HipcMapAlias | SfBufferAttr_Out },
        .buffers = { { out_info, sizeof(UsbHsInterfaceInfo) } },
    );
}

Result usbHsGetAlternateInterfaceFwd(UsbHsClientIfSession *client_if, u8 id, UsbHsInterfaceInfo *out_info) {
    serviceAssumeDomain(&client_if->s);
    return serviceDispatchIn(&client_if->s, 3, id,
        .buffer_attrs = { SfBufferAttr_HipcMapAlias | SfBufferAttr_Out },
        .buffers = { { out_info, sizeof(UsbHsInterfaceInfo) } },
    );
}

Result usbHsGetCurrentFrameFwd(UsbHsClientIfSession *client_if, u32 *out) {
    if (hosversionBefore(2,0,0))
        return 0;

    serviceAssumeDomain(&client_if->s);
    return serviceDispatchOut(&client_if->s, 4, *out);
}

Result usbHsCtrlXferAsyncFwd(UsbHsClientIfSession *client_if, u8 bmRequestType, u8 bRequest, u16 wValue, u16 wIndex, u16 wLength, u64 buffer) {
    const struct {
        u8 bmRequestType;
        u8 bRequest;
        u16 wValue;
        u16 wIndex;
        u16 wLength;
        u64 buffer;
    } in = { bmRequestType, bRequest, wValue, wIndex, wLength, buffer };

    serviceAssumeDomain(&client_if->s);
    return serviceDispatchIn(&client_if->s, 5, in);
}

Result usbHsGetCtrlXferCompletionEventFwd(UsbHsClientIfSession *client_if, Handle *out_handle) {
    serviceAssumeDomain(&client_if->s);
    return serviceDispatch(&client_if->s, 6, 
        .out_handle_attrs = { SfOutHandleAttr_HipcCopy },
        .out_handles = out_handle,
    );
}

Result usbHsGetCtrlXferReportFwd(UsbHsClientIfSession *client_if, UsbHsXferReport* out_report) {
    serviceAssumeDomain(&client_if->s);
    return serviceDispatch(&client_if->s, 7,
        .buffer_attrs = { SfBufferAttr_HipcMapAlias | SfBufferAttr_Out },
        .buffers = { { out_report, sizeof(UsbHsXferReport) } },
    );
}

Result usbHsResetDeviceFwd(UsbHsClientIfSession *client_if) {
    if (hosversionBefore(2,0,0))
        return 0;

    serviceAssumeDomain(&client_if->s);
    return serviceDispatch(&client_if->s, 8);
}

//Result usbHsOpenUsbEpFwd(UsbHsClientIfSession *client_if, UsbHsClientEpSession* client_ep, u16 maxUrbCount, u32 epType, u32 epNumber, u32 epDirection, u32 maxXferSize) {
Result usbHsOpenUsbEpFwd(UsbHsClientIfSession *client_if, UsbHsClientEpSession* client_ep, u16 maxUrbCount, u32 epType, u32 epNumber, u32 epDirection, u32 maxXferSize, struct usb_endpoint_descriptor *out_desc) {
    const struct {
        u16 maxUrbCount;
        u16 pad;
        u32 epType;
        u32 epNumber;
        u32 epDirection;
        u32 maxXferSize;
    } in = { maxUrbCount, 0, epType, epNumber, epDirection, maxXferSize };

    /*
    return serviceDispatchInOut(&client_if->s, hosversionAtLeast(2,0,0) ? 9 : 4, in, client_ep->desc,
        .out_num_objects = 1,
        .out_objects = &client_ep->s,
    );
    */

    serviceAssumeDomain(&client_if->s);
    return serviceDispatchInOut(&client_if->s, hosversionAtLeast(2,0,0) ? 9 : 4, in, *out_desc,
        .out_num_objects = 1,
        .out_objects = &client_ep->s,
    );
}

/* usb::hs::IClientRootSession */

Result usbHsBindClientProcessFwd(Service *s, Handle handle) {
    serviceAssumeDomain(s);
    return serviceDispatch(s, 0,
        .in_num_handles = 1,
        .in_handles = { handle }
    );
}

Result usbHsQueryAllInterfacesFwd(Service *s, const UsbHsInterfaceFilter *filter, size_t max_interfaces, UsbHsInterface *out_interfaces, s32 *out_total) {
    serviceAssumeDomain(s);
    return serviceDispatchInOut(s, 1, *filter, *out_total,
        .buffer_attrs = { SfBufferAttr_HipcMapAlias | SfBufferAttr_Out },
        .buffers = { { out_interfaces, max_interfaces * sizeof(UsbHsInterface) } },
    );
}

Result usbHsQueryAvailableInterfacesFwd(Service *s, const UsbHsInterfaceFilter *filter, size_t max_interfaces, UsbHsInterface *out_interfaces, s32 *out_total) {
    serviceAssumeDomain(s);
    return serviceDispatchInOut(s, 2, *filter, *out_total,
        .buffer_attrs = { SfBufferAttr_HipcMapAlias | SfBufferAttr_Out },
        .buffers = { { out_interfaces, max_interfaces * sizeof(UsbHsInterface) } },
    );
}

Result usbHsQueryAcquiredInterfacesFwd(Service *s, size_t max_interfaces, UsbHsInterface *out_interfaces, s32 *out_total) {
    serviceAssumeDomain(s);
    return serviceDispatchOut(s, 3, *out_total,
        .buffer_attrs = { SfBufferAttr_HipcMapAlias | SfBufferAttr_Out },
        .buffers = { { out_interfaces, max_interfaces * sizeof(UsbHsInterface) } },
    );
}

Result usbHsCreateInterfaceAvailableEventFwd(Service *s, u8 index, const UsbHsInterfaceFilter *filter, Handle *out_handle) {
    const struct {
        u8 index;
        u8 pad;
        UsbHsInterfaceFilter filter;
    } in = { index, 0, *filter };
    
    serviceAssumeDomain(s);
    return serviceDispatchIn(s, 4, in,
        .out_handle_attrs = { SfOutHandleAttr_HipcCopy },
        .out_handles = out_handle,
    );
}

Result usbHsDestroyInterfaceAvailableEventFwd(Service *s, u8 index) {
    serviceAssumeDomain(s);
    return serviceDispatchIn(s, 5, index);
}

Result usbHsGetInterfaceStateChangeEventFwd(Service *s, Handle *out_handle) {
    serviceAssumeDomain(s);
    return serviceDispatch(s, 6, 
        .out_handle_attrs = { SfOutHandleAttr_HipcCopy },
        .out_handles = out_handle,
    );
}

//Result usbHsAcquireUsbIfFwd(Service *s, UsbHsClientIfSession* client_if) {
Result usbHsAcquireUsbIfFwd(Service *s, UsbHsClientIfSession *client_if, u32 id, void *buffer1, size_t buffer1_size, void *buffer2, size_t buffer2_size) {
    serviceAssumeDomain(s);
    //return serviceDispatchIn(s, 7, client_if->ID,
    return serviceDispatchIn(s, 7, id,
        .buffer_attrs = {
            SfBufferAttr_HipcMapAlias | SfBufferAttr_Out,
            SfBufferAttr_HipcMapAlias | SfBufferAttr_Out,
        },
        .buffers = {
            //{ &client_if->inf.pathstr, sizeof(UsbHsInterface) - sizeof(UsbHsInterfaceInfo) },
            //{ &client_if->inf.inf, sizeof(UsbHsInterfaceInfo) },
            { buffer1, buffer1_size },
            { buffer2, buffer2_size },
        },
        .out_num_objects = 1,
        .out_objects = &client_if->s,
    );
}

Result usbHsSetTestModeFwd(Service *s, u32 unk1, u32 unk2, u32 unk3, s32 *out) {
    const struct {
        u32 unk1;
        u32 unk2;
        u32 unk3;
    } in = { unk1, unk2, unk3 };

    serviceAssumeDomain(s);
    return serviceDispatchInOut(s, 8, in, *out);
}
