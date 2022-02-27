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
#include "usbmitm_module.hpp"
#include "usbhs_mitm_service.hpp"
#include "../utils.hpp"
#include <stratosphere.hpp>

#include "../logging.hpp"

namespace ams::mitm::usb {

    namespace {

        enum PortIndex {
            PortIndex_UsbHsMitm,
            PortIndex_UsbHsAMitm,
            PortIndex_Count,
        };

        constexpr sm::ServiceName UsbHsMitmServiceName = sm::ServiceName::Encode("usb:hs");
        constexpr size_t UsbHsMaxSessions = 20;

        constexpr sm::ServiceName UsbHsAMitmServiceName = sm::ServiceName::Encode("usb:hs:a");
        constexpr size_t UsbHsAMaxSessions = 3;

        struct ServerOptions {
            static constexpr size_t PointerBufferSize   = 0x1000;
            static constexpr size_t MaxDomains          = 0x100;
            static constexpr size_t MaxDomainObjects    = 0x100;
            static constexpr bool CanDeferInvokeRequest = false;
            static constexpr bool CanManageMitmServers  = true;
        };

        constexpr size_t MaxSessions = UsbHsMaxSessions + UsbHsAMaxSessions;

        class ServerManager final : public sf::hipc::ServerManager<PortIndex_Count, ServerOptions, MaxSessions> {
            private:
                virtual Result OnNeedsToAccept(int port_index, Server *server) override;
        };

        ServerManager g_server_manager;

        Result ServerManager::OnNeedsToAccept(int port_index, Server *server) {
            /* Acknowledge the mitm session. */
            std::shared_ptr<::Service> fsrv;
            sm::MitmProcessInfo client_info;
            server->AcknowledgeMitmSession(std::addressof(fsrv), std::addressof(client_info));

            switch (port_index) {
                case PortIndex_UsbHsMitm:
                    DEBUG_LOG("Processing port: usb:hs");
                    return this->AcceptMitmImpl(server, sf::CreateSharedObjectEmplaced<impl::IUsbHsMitmInterface, UsbHsMitmService>(decltype(fsrv)(fsrv), client_info), fsrv);
                case PortIndex_UsbHsAMitm:
                    DEBUG_LOG("Processing port: usb:hs:a");
                    return this->AcceptMitmImpl(server, sf::CreateSharedObjectEmplaced<impl::IUsbHsMitmInterface, UsbHsMitmService>(decltype(fsrv)(fsrv), client_info), fsrv);
                AMS_UNREACHABLE_DEFAULT_CASE();
            }
        }

        constexpr s32 ThreadPriority = -12;
        constexpr size_t TotalThreads = 2;
        constexpr size_t NumExtraThreads = TotalThreads - 1;
        constexpr size_t ThreadStackSize = 0x4000;

        constinit os::ThreadType g_thread;
        alignas(os::ThreadStackAlignment) constinit u8 g_thread_stack[ThreadStackSize];

        constinit os::ThreadType g_extra_threads[NumExtraThreads];
        alignas(os::MemoryPageSize) constinit u8 g_extra_thread_stacks[NumExtraThreads][ThreadStackSize];

        void LoopServerThread(void *) {
            g_server_manager.LoopProcess();
        }

        void ProcessForServerOnAllThreads() {
            /* Initialize threads. */
            if constexpr (NumExtraThreads > 0) {
                const s32 priority = os::GetThreadCurrentPriority(os::GetCurrentThread());
                for (size_t i = 0; i < NumExtraThreads; i++) {
                    R_ABORT_UNLESS(os::CreateThread(g_extra_threads + i, LoopServerThread, nullptr, g_extra_thread_stacks[i], ThreadStackSize, priority));
                    os::SetThreadNamePointer(g_extra_threads + i, "mc::UsbMitmThread");
                }
            }

            /* Start extra threads. */
            if constexpr (NumExtraThreads > 0) {
                for (size_t i = 0; i < NumExtraThreads; i++) {
                    os::StartThread(g_extra_threads + i);
                }
            }

            /* Loop this thread. */
            LoopServerThread(nullptr);

            /* Wait for extra threads to finish. */
            if constexpr (NumExtraThreads > 0) {
                for (size_t i = 0; i < NumExtraThreads; i++) {
                    os::WaitThread(g_extra_threads + i);
                }
            }
        }

        void UsbMitmThreadFunction(void *) {
            DEBUG_LOG("usb:hs mitm thread started");

            R_ABORT_UNLESS((g_server_manager.RegisterMitmServer<UsbHsMitmService>(PortIndex_UsbHsMitm, UsbHsMitmServiceName)));
            R_ABORT_UNLESS((g_server_manager.RegisterMitmServer<UsbHsMitmService>(PortIndex_UsbHsAMitm, UsbHsAMitmServiceName)));

            ProcessForServerOnAllThreads();
        }

    }

    void Launch() {
        R_ABORT_UNLESS(os::CreateThread(&g_thread,
            UsbMitmThreadFunction,
            nullptr,
            g_thread_stack,
            ThreadStackSize,
            ThreadPriority
        ));

        os::SetThreadNamePointer(&g_thread, "mc::UsbMitmThread");
        os::StartThread(&g_thread);
    }

    void WaitFinished() {
        os::WaitThread(&g_thread);
    }

}
