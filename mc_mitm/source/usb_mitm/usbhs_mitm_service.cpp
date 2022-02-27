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
#include "usbhs_mitm_service.hpp"
#include "usbhs_shim.h"

#include "../logging.hpp"

namespace ams::mitm::usb {

    /* usb::hs::IClientEpSession */

    Result UsbHsClientEpService::ReOpen() {
        DEBUG_LOG("UsbHsClientEpService::ReOpen");

        R_RETURN(usbHsReOpenFwd(m_srv.get()));
    }

    Result UsbHsClientEpService::Close() {
        DEBUG_LOG("UsbHsClientEpService::Close");

        R_RETURN(usbHsCloseFwd(m_srv.get()));
    }

    Result UsbHsClientEpService::GetCompletionEvent(sf::OutCopyHandle out_handle) {
        DEBUG_LOG("UsbHsClientEpService::GetCompletionEvent");

        os::NativeHandle event_handle;
        R_TRY(usbHsGetCompletionEventFwd(m_srv.get(), &event_handle));

        out_handle.SetValue(event_handle, false);
        R_SUCCEED();
    }

    Result UsbHsClientEpService::PopulateRing() {
        DEBUG_LOG("UsbHsClientEpService::PopulateRing");

        R_RETURN(usbHsPopulateRingFwd(m_srv.get()));
    }

    Result UsbHsClientEpService::PostBufferAsync(u32 size, u64 buffer, u64 id, sf::Out<u32> out_xferId) {
        DEBUG_LOG("UsbHsClientEpService::PostBufferAsync");

        R_RETURN(usbHsPostBufferAsyncFwd(m_srv.get(), out_xferId.GetPointer(), size, buffer, id));
    }

    Result UsbHsClientEpService::GetXferReport(u32 max_reports, sf::Out<u32> out_count, const sf::OutAutoSelectBuffer &out_buffer) {
        DEBUG_LOG("UsbHsClientEpService::GetXferReport");

        R_RETURN(usbHsGetXferReportFwd(m_srv.get(), out_count.GetPointer(), max_reports, reinterpret_cast<UsbHsXferReport*>(out_buffer.GetPointer())));
    }

    Result UsbHsClientEpService::BatchBufferAsync(u32 urbCount, u32 unk1, u32 unk2, u64 buffer, u64 id, sf::Out<u32> out_xferId, const sf::InAutoSelectArray<u32> &sizes) {
        DEBUG_LOG("UsbHsClientEpService::BatchBufferAsync");

        R_RETURN(usbHsBatchBufferAsyncFwd(m_srv.get(), out_xferId.GetPointer(), urbCount, unk1, unk2, buffer, id, sizes.GetPointer()));
    }

    Result UsbHsClientEpService::CreateSmmuSpace(u32 size, u64 buffer) {
        DEBUG_LOG("UsbHsClientEpService::CreateSmmuSpace");

        R_RETURN(usbHsCreateSmmuSpaceFwd(m_srv.get(), size, buffer));
    }

    Result UsbHsClientEpService::ShareReportRing(u32 size, sf::CopyHandle &&handle) {
        DEBUG_LOG("UsbHsClientEpService::ShareReportRing");

        R_RETURN(usbHsShareReportRingFwd(m_srv.get(), size, handle.GetOsHandle()));
    }

    /* usb::hs::IClientIfSession */

    Result UsbHsClientIfService::GetStateChangeEvent(sf::OutCopyHandle out_handle) {
        DEBUG_LOG("UsbHsClientIfService::GetStateChangeEvent");

        os::NativeHandle event_handle;
        R_TRY(usbHsGetStateChangeEventFwd(m_srv.get(), &event_handle));

        out_handle.SetValue(event_handle, false);
        R_SUCCEED();
    }

    Result UsbHsClientIfService::SetInterface(u8 id, const sf::OutMapAliasBuffer &out_buffer) {
        DEBUG_LOG("UsbHsClientIfService::SetInterface");

        //return usbHsSetInterfaceFwd(m_srv.get(), id, reinterpret_cast<UsbHsInterfaceInfo*>(out_buffer.GetPointer()));
        R_RETURN(usbHsIfSetInterface(m_srv.get(), reinterpret_cast<UsbHsInterfaceInfo*>(out_buffer.GetPointer()), id));
    }

    Result UsbHsClientIfService::GetInterface(const sf::OutMapAliasBuffer &out_buffer) {
        DEBUG_LOG("UsbHsClientIfService::GetInterface");

        //return usbHsGetInterfaceFwd(m_srv.get(), reinterpret_cast<UsbHsInterfaceInfo*>(out_buffer.GetPointer()));
        R_RETURN(usbHsIfGetInterface(m_srv.get(), reinterpret_cast<UsbHsInterfaceInfo*>(out_buffer.GetPointer())));
    }

    Result UsbHsClientIfService::GetAlternateInterface(u8 id, const sf::OutMapAliasBuffer &out_buffer) {
        DEBUG_LOG("UsbHsClientIfService::GetAlternateInterface");

        //return usbHsGetAlternateInterfaceFwd(m_srv.get(), id, reinterpret_cast<UsbHsInterfaceInfo*>(out_buffer.GetPointer()));
        R_RETURN(usbHsIfGetAlternateInterface(m_srv.get(), reinterpret_cast<UsbHsInterfaceInfo*>(out_buffer.GetPointer()), id));
    }

    Result UsbHsClientIfService::GetCurrentFrame(sf::Out<u32> out) {
        DEBUG_LOG("UsbHsClientIfService::GetCurrentFrame");

        //return usbHsGetCurrentFrameFwd(m_srv.get(), out.GetPointer());
        R_RETURN(usbHsIfGetCurrentFrame(m_srv.get(), out.GetPointer()));
    }

    Result UsbHsClientIfService::CtrlXferAsync(u8 bmRequestType, u8 bRequest, u16 wValue, u16 wIndex, u16 wLength, u64 buffer) {
        //DEBUG_LOG("UsbHsClientIfService::CtrlXferAsync");
        DEBUG_LOG("UsbHsClientIfService::CtrlXferAsync: bmRequestType=%d, bRequest=%d, wValue=%d, wIndex=%d, wLength=%d, buffer=0x%lx",
            bmRequestType,
            bRequest,
            wValue,
            wIndex,
            wLength,
            buffer
        );

        R_RETURN(usbHsCtrlXferAsyncFwd(m_srv.get(), bmRequestType, bRequest, wValue, wIndex, wLength, buffer));
        //return usbHsCtrlXferAsync(m_srv.get(), bmRequestType, bRequest, wValue, wIndex, wLength, buffer);
    }

    Result UsbHsClientIfService::GetCtrlXferCompletionEvent(sf::OutCopyHandle out_handle) {
        DEBUG_LOG("UsbHsClientIfService::GetCtrlXferCompletionEvent");

        os::NativeHandle event_handle;
        R_TRY(usbHsGetCtrlXferCompletionEventFwd(m_srv.get(), &event_handle));
        out_handle.SetValue(event_handle, false);
        R_SUCCEED();
    }

    Result UsbHsClientIfService::GetCtrlXferReport(const sf::OutMapAliasBuffer &out_buffer) {
        DEBUG_LOG("UsbHsClientIfService::GetCtrlXferReport");

        R_RETURN(usbHsGetCtrlXferReportFwd(m_srv.get(), reinterpret_cast<UsbHsXferReport*>(out_buffer.GetPointer())));
        //return usbHsGetCtrlXferReport(m_srv.get(), reinterpret_cast<UsbHsXferReport*>(out_buffer.GetPointer()));
    }

    Result UsbHsClientIfService::ResetDevice() {
        DEBUG_LOG("UsbHsClientIfService::ResetDevice");

        //return usbHsResetDeviceFwd(m_srv.get());
        R_RETURN(usbHsIfResetDevice(m_srv.get()));
    }

    Result UsbHsClientIfService::OpenUsbEp(u16 maxUrbCount, u32 epType, u32 epNumber, u32 epDirection, u32 maxXferSize, sf::Out<usb_endpoint_descriptor> out_desc, sf::Out<sf::SharedPointer<mitm::usb::impl::IClientEpSession>> out) {
        //DEBUG_LOG("UsbHsClientIfService::OpenUsbEp");
        DEBUG_LOG("client: 0x%016lx, maxUrbCount: 0x%x, epType: 0x%x, epNumber: 0x%x, epDirection: 0x%x, maxXferSize: 0x%x",
            m_client_info.program_id,
            maxUrbCount,
            epType,
            epNumber,
            epDirection,
            maxXferSize
        );

        //DEBUG_LOG("usb_endpoint_descriptor size: 0x%x", sizeof(usb_endpoint_descriptor));

        UsbHsClientEpSession client_ep;
        //R_TRY(usbHsOpenUsbEpFwd(m_srv.get(), std::addressof(client_ep), maxUrbCount, epType, epNumber, epDirection, maxXferSize));
        //out_desc.SetValue(client_ep.desc);
        R_TRY(usbHsOpenUsbEpFwd(m_srv.get(), std::addressof(client_ep), maxUrbCount, epType, epNumber, epDirection, maxXferSize, out_desc.GetPointer()));
        
        std::memcpy(&client_ep.desc, out_desc.GetPointer(), sizeof(client_ep.desc));

        const sf::cmif::DomainObjectId target_object_id{serviceGetObjectId(std::addressof(client_ep.s))};
        out.SetValue(sf::CreateSharedObjectEmplaced<impl::IClientEpSession, UsbHsClientEpService>(m_client_info, std::make_unique<UsbHsClientEpSession>(client_ep)), target_object_id);
        R_SUCCEED();
    }

    /* usb::hs::IClientRootSession */

    Result UsbHsMitmService::BindClientProcess(sf::CopyHandle &&handle) {
        DEBUG_LOG("UsbHsMitmService::BindClientProcess");

        R_RETURN(usbHsBindClientProcessFwd(m_forward_service.get(), handle.GetOsHandle()));
    }

    Result UsbHsMitmService::QueryAllInterfaces(UsbHsInterfaceFilter filter, sf::Out<s32> out_total, const sf::OutMapAliasArray<UsbHsInterface> &out_interfaces) {
        DEBUG_LOG("UsbHsMitmService::QueryAllInterfaces");

        R_RETURN(usbHsQueryAllInterfacesFwd(m_forward_service.get(), std::addressof(filter), out_interfaces.GetSize(), out_interfaces.GetPointer(), out_total.GetPointer()));
    }

    Result UsbHsMitmService::QueryAvailableInterfaces(UsbHsInterfaceFilter filter, sf::Out<s32> out_total, const sf::OutMapAliasArray<UsbHsInterface> &out_interfaces) {
        DEBUG_LOG("UsbHsMitmService::QueryAvailableInterfaces");

        R_RETURN(usbHsQueryAvailableInterfacesFwd(m_forward_service.get(), std::addressof(filter), out_interfaces.GetSize(), out_interfaces.GetPointer(), out_total.GetPointer()));
    }

    Result UsbHsMitmService::QueryAcquiredInterfaces(sf::Out<s32> out_total, const sf::OutMapAliasArray<UsbHsInterface> &out_interfaces) {
        DEBUG_LOG("UsbHsMitmService::QueryAcquiredInterfaces");

        R_RETURN(usbHsQueryAcquiredInterfacesFwd(m_forward_service.get(), out_interfaces.GetSize(), out_interfaces.GetPointer(), out_total.GetPointer()));
    }

    Result UsbHsMitmService::CreateInterfaceAvailableEvent(u8 index, UsbHsInterfaceFilter filter, sf::OutCopyHandle out_handle) {
        DEBUG_LOG("UsbHsMitmService::CreateInterfaceAvailableEvent");

        os::NativeHandle event_handle;
        R_TRY(usbHsCreateInterfaceAvailableEventFwd(m_forward_service.get(), index, std::addressof(filter), std::addressof(event_handle)));
        out_handle.SetValue(event_handle, false);
        R_SUCCEED();
    }

    Result UsbHsMitmService::DestroyInterfaceAvailableEvent(u8 index) {
        DEBUG_LOG("UsbHsMitmService::DestroyInterfaceAvailableEvent");

        R_RETURN(usbHsDestroyInterfaceAvailableEventFwd(m_forward_service.get(), index));
    }

    Result UsbHsMitmService::GetInterfaceStateChangeEvent(sf::OutCopyHandle out_handle) {
        DEBUG_LOG("UsbHsMitmService::GetInterfaceStateChangeEvent");

        os::NativeHandle event_handle;
        R_TRY(usbHsGetInterfaceStateChangeEventFwd(m_forward_service.get(), std::addressof(event_handle)));
        out_handle.SetValue(event_handle, false);
        R_SUCCEED();
    }

    Result UsbHsMitmService::AcquireUsbIf(u32 id, const sf::OutMapAliasBuffer &out_buffer1, const sf::OutMapAliasBuffer &out_buffer2, sf::Out<sf::SharedPointer<mitm::usb::impl::IClientIfSession>> out) {
        DEBUG_LOG("UsbHsMitmService::AcquireUsbIf: client: 0x%016xl", m_client_info.program_id);
        //DEBUG_LOG("ID: 0x%x", id);
        //DEBUG_LOG("out_buffer1 size: 0x%x", out_buffer1.GetSize());
        //DEBUG_LOG("out_buffer2 size: 0x%x", out_buffer2.GetSize());
        //DEBUG_LOG("sizeof(UsbHsInterface) - sizeof(UsbHsInterfaceInfo): 0x%x", sizeof(UsbHsInterface) - sizeof(UsbHsInterfaceInfo));
        //DEBUG_LOG("sizeof(UsbHsInterfaceInfo): 0x%x", sizeof(UsbHsInterfaceInfo));

        //UsbHsClientIfSession client_if = {};
        //client_if.ID = id;
        //R_TRY(usbHsAcquireUsbIfFwd(m_forward_service.get(), std::addressof(client_if)));
        //std::memcpy(out_buffer1.GetPointer(), std::addressof(client_if.inf.pathstr), out_buffer1.GetSize());
        //std::memcpy(out_buffer2.GetPointer(), std::addressof(client_if.inf.inf), out_buffer2.GetSize());
        UsbHsClientIfSession client_if;
        R_TRY(usbHsAcquireUsbIfFwd(m_forward_service.get(), std::addressof(client_if), id, out_buffer1.GetPointer(), out_buffer1.GetSize(), out_buffer2.GetPointer(), out_buffer2.GetSize()));

        client_if.ID = id;

        const sf::cmif::DomainObjectId target_object_id{serviceGetObjectId(std::addressof(client_if.s))};
        out.SetValue(sf::CreateSharedObjectEmplaced<impl::IClientIfSession, UsbHsClientIfService>(m_client_info, std::make_unique<UsbHsClientIfSession>(client_if)), target_object_id);
        R_SUCCEED();
    }

    Result UsbHsMitmService::SetTestMode(u32 unk1, u32 unk2, u32 unk3, sf::Out<s32> out_unk) {
        DEBUG_LOG("UsbHsMitmService::SetTestMode");

        R_RETURN(usbHsSetTestModeFwd(m_forward_service.get(), unk1, unk2, unk3, out_unk.GetPointer()));
    }

}
