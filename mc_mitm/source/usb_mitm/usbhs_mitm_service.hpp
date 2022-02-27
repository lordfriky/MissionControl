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
#include <stratosphere.hpp>

#include "../logging.hpp"

#define AMS_USBHS_CLIENT_EP_MITM_INTERFACE_INFO(C, H)                                                                                                                                                                                                            \
    AMS_SF_METHOD_INFO(C, H, 0, Result, ReOpen,             (),                                                                                                                       (),                                                    hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 1, Result, Close,              (),                                                                                                                       (),                                                    hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 2, Result, GetCompletionEvent, (sf::OutCopyHandle out_handle),                                                                                           (out_handle),                                          hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 3, Result, PopulateRing,       (),                                                                                                                       (),                                                    hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 4, Result, PostBufferAsync,    (u32 size, u64 buffer, u64 id, sf::Out<u32> out_xferId),                                                                  (size, buffer, id, out_xferId),                        hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 5, Result, GetXferReport,      (u32 max_reports, sf::Out<u32> out_count, const sf::OutAutoSelectBuffer &out_buffer),                                     (max_reports, out_count, out_buffer),                  hos::Version_3_0_0) \
    AMS_SF_METHOD_INFO(C, H, 6, Result, BatchBufferAsync,   (u32 urbCount, u32 unk1, u32 unk2, u64 buffer, u64 id, sf::Out<u32> out_xferId, const sf::InAutoSelectArray<u32> &sizes), (urbCount, unk1, unk2, buffer, id, out_xferId, sizes), hos::Version_3_0_0) \
    AMS_SF_METHOD_INFO(C, H, 7, Result, CreateSmmuSpace,    (u32 size, u64 buffer),                                                                                                   (size, buffer),                                        hos::Version_4_0_0) \
    AMS_SF_METHOD_INFO(C, H, 8, Result, ShareReportRing,    (u32 size, sf::CopyHandle &&handle),                                                                                      (size, std::move(handle)),                             hos::Version_4_0_0) \

AMS_SF_DEFINE_INTERFACE(ams::mitm::usb::impl, IClientEpSession, AMS_USBHS_CLIENT_EP_MITM_INTERFACE_INFO, 0x8B56EADD)

#define AMS_USBHS_CLIENT_IF_MITM_INTERFACE_INFO(C, H)                                                                                                                                                                                                                                                                                                          \
    AMS_SF_METHOD_INFO(C, H, 0, Result, GetStateChangeEvent,        (sf::OutCopyHandle out_handle),                                                                                                                                                              (out_handle))                                                                                 \
    AMS_SF_METHOD_INFO(C, H, 1, Result, SetInterface,               (u8 id, const sf::OutMapAliasBuffer &out_buffer),                                                                                                                                            (id, out_buffer))                                                                             \
    AMS_SF_METHOD_INFO(C, H, 2, Result, GetInterface,               (const sf::OutMapAliasBuffer out_buffer),                                                                                                                                                    (out_buffer))                                                                                 \
    AMS_SF_METHOD_INFO(C, H, 3, Result, GetAlternateInterface,      (u8 id, const sf::OutMapAliasBuffer &out_buffer),                                                                                                                                            (id, out_buffer))                                                                             \
    AMS_SF_METHOD_INFO(C, H, 4, Result, GetCurrentFrame,            (sf::Out<u32> out),                                                                                                                                                                          (out),                                                                    hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 5, Result, CtrlXferAsync,              (u8 bmRequestType, u8 bRequest, u16 wValue, u16 wIndex, u16 wLength, u64 buffer),                                                                                                            (bmRequestType, bRequest, wValue, wIndex, wLength, buffer),               hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 6, Result, GetCtrlXferCompletionEvent, (sf::OutCopyHandle out_handle),                                                                                                                                                              (out_handle),                                                             hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 7, Result, GetCtrlXferReport,          (const sf::OutMapAliasBuffer &out_buffer),                                                                                                                                                   (out_buffer),                                                             hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 8, Result, ResetDevice,                (),                                                                                                                                                                                          ())                                                                                           \
    AMS_SF_METHOD_INFO(C, H, 9, Result, OpenUsbEp,                  (u16 maxUrbCount, u32 epType, u32 epNumber, u32 epDirection, u32 maxXferSize, sf::Out<usb_endpoint_descriptor> out_desc, sf::Out<sf::SharedPointer<mitm::usb::impl::IClientEpSession>> out), (maxUrbCount, epType, epNumber, epDirection, maxXferSize, out_desc, out), hos::Version_2_0_0) \

AMS_SF_DEFINE_INTERFACE(ams::mitm::usb::impl, IClientIfSession, AMS_USBHS_CLIENT_IF_MITM_INTERFACE_INFO, 0x81128C7E)

#define AMS_USBHS_MITM_INTERFACE_INFO(C, H)                                                                                                                                                                                                                                                       \
    AMS_SF_METHOD_INFO(C, H, 0, Result, BindClientProcess,              (sf::CopyHandle &&handle),                                                                                                                                       (std::move(handle)),                 hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 1, Result, QueryAllInterfaces,             (UsbHsInterfaceFilter filter, sf::Out<s32> out_total, const sf::OutMapAliasArray<UsbHsInterface> &out_interfaces),                                               (filter, out_total, out_interfaces), hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 2, Result, QueryAvailableInterfaces,       (UsbHsInterfaceFilter filter, sf::Out<s32> out_total, const sf::OutMapAliasArray<UsbHsInterface> &out_interfaces),                                               (filter, out_total, out_interfaces), hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 3, Result, QueryAcquiredInterfaces,        (sf::Out<s32> out_total, const sf::OutMapAliasArray<UsbHsInterface> &out_interfaces),                                                                            (out_total, out_interfaces),         hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 4, Result, CreateInterfaceAvailableEvent,  (u8 index, UsbHsInterfaceFilter filter, sf::OutCopyHandle out_handle),                                                                                           (index, filter, out_handle),         hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 5, Result, DestroyInterfaceAvailableEvent, (u8 index),                                                                                                                                                      (index),                             hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 6, Result, GetInterfaceStateChangeEvent,   (sf::OutCopyHandle out_handle),                                                                                                                                  (out_handle),                        hos::Version_2_0_0) \
    AMS_SF_METHOD_INFO(C, H, 7, Result, AcquireUsbIf,                   (u32 id, const sf::OutMapAliasBuffer &out_buffer1, const sf::OutMapAliasBuffer &out_buffer2, sf::Out<sf::SharedPointer<mitm::usb::impl::IClientIfSession>> out), (id, out_buffer1, out_buffer2, out), hos::Version_3_0_0) \
    AMS_SF_METHOD_INFO(C, H, 8, Result, SetTestMode,                    (u32 unk1, u32 unk2, u32 unk3, sf::Out<s32> out_unk),                                                                                                            (unk1, unk2, unk3, out_unk),         hos::Version_6_0_0) \

AMS_SF_DEFINE_MITM_INTERFACE(ams::mitm::usb::impl, IUsbHsMitmInterface, AMS_USBHS_MITM_INTERFACE_INFO, 0xB3E41D38)

namespace ams::mitm::usb {

    class UsbHsClientEpService {
        private:
            sm::MitmProcessInfo m_client_info;
            std::unique_ptr<::UsbHsClientEpSession> m_srv;
        public:
            UsbHsClientEpService(const sm::MitmProcessInfo &cl, std::unique_ptr<::UsbHsClientEpSession> s) : m_client_info(cl), m_srv(std::move(s)) { /* .. */ 
                DEBUG_LOG("UsbHsClientEpService::UsbHsClientEpService");
            }

            virtual ~UsbHsClientEpService() {
                DEBUG_LOG("UsbHsClientEpService::~UsbHsClientEpService");

                serviceAssumeDomain(&m_srv.get()->s);
                usbHsEpClose(m_srv.get());
            }
        public:
            Result ReOpen();
            Result Close();
            Result GetCompletionEvent(sf::OutCopyHandle out_handle);
            Result PopulateRing();
            Result PostBufferAsync(u32 size, u64 buffer, u64 id, sf::Out<u32> out_xferId);
            Result GetXferReport(u32 max_reports, sf::Out<u32> out_count, const sf::OutAutoSelectBuffer &out_buffer);
            Result BatchBufferAsync(u32 urbCount, u32 unk1, u32 unk2, u64 buffer, u64 id, sf::Out<u32> out_xferId, const sf::InAutoSelectArray<u32> &sizes);
            Result CreateSmmuSpace(u32 size, u64 buffer);
            Result ShareReportRing(u32 size, sf::CopyHandle &&handle);
    };
    static_assert(impl::IsIClientEpSession<UsbHsClientEpService>);

    class UsbHsClientIfService {
        private:
            sm::MitmProcessInfo m_client_info;
            std::unique_ptr<::UsbHsClientIfSession> m_srv;
        public:
            UsbHsClientIfService(const sm::MitmProcessInfo &cl, std::unique_ptr<::UsbHsClientIfSession> s) : m_client_info(cl), m_srv(std::move(s)) { /* .. */ 
                DEBUG_LOG("UsbHsClientIfService::UsbHsClientIfService");
            }

            virtual ~UsbHsClientIfService() {
                DEBUG_LOG("UsbHsClientIfService::~UsbHsClientIfService");

                serviceAssumeDomain(&m_srv.get()->s);
                usbHsIfClose(m_srv.get());
            }
        public:
            Result GetStateChangeEvent(sf::OutCopyHandle out_handle);
            Result SetInterface(u8 id, const sf::OutMapAliasBuffer &out_buffer);
            Result GetInterface(const sf::OutMapAliasBuffer &out_buffer);
            Result GetAlternateInterface(u8 id, const sf::OutMapAliasBuffer &out_buffer);
            Result GetCurrentFrame(sf::Out<u32> out);
            Result CtrlXferAsync(u8 bmRequestType, u8 bRequest, u16 wValue, u16 wIndex, u16 wLength, u64 buffer);
            Result GetCtrlXferCompletionEvent(sf::OutCopyHandle out_handle);
            Result GetCtrlXferReport(const sf::OutMapAliasBuffer &out_buffer);
            Result ResetDevice();
            Result OpenUsbEp(u16 maxUrbCount, u32 epType, u32 epNumber, u32 epDirection, u32 maxXferSize, sf::Out<usb_endpoint_descriptor> out_desc, sf::Out<sf::SharedPointer<mitm::usb::impl::IClientEpSession>> out);
    };
    static_assert(impl::IsIClientIfSession<UsbHsClientIfService>);

    class UsbHsMitmService : public sf::MitmServiceImplBase {
        public:
            using MitmServiceImplBase::MitmServiceImplBase;
        public:
            static bool ShouldMitm(const sm::MitmProcessInfo &client_info) {
                return client_info.program_id == ncm::SystemProgramId::Hid;
            }
        public:
            Result BindClientProcess(sf::CopyHandle &&handle);
            Result QueryAllInterfaces(UsbHsInterfaceFilter filter, sf::Out<s32> out_total, const sf::OutMapAliasArray<UsbHsInterface> &out_interfaces);
            Result QueryAvailableInterfaces(UsbHsInterfaceFilter filter, sf::Out<s32> out_total, const sf::OutMapAliasArray<UsbHsInterface> &out_interfaces);
            Result QueryAcquiredInterfaces(sf::Out<s32> out_total, const sf::OutMapAliasArray<UsbHsInterface> &out_interfaces);
            Result CreateInterfaceAvailableEvent(u8 index, UsbHsInterfaceFilter filter, sf::OutCopyHandle out_handle);
            Result DestroyInterfaceAvailableEvent(u8 index);
            Result GetInterfaceStateChangeEvent(sf::OutCopyHandle out_handle);
            Result AcquireUsbIf(u32 id, const sf::OutMapAliasBuffer &out_buffer1, const sf::OutMapAliasBuffer &out_buffer2, sf::Out<sf::SharedPointer<mitm::usb::impl::IClientIfSession>> out);
            Result SetTestMode(u32 unk1, u32 unk2, u32 unk3, sf::Out<s32> out_unk);
    };
    static_assert(impl::IsIUsbHsMitmInterface<UsbHsMitmService>);

}
