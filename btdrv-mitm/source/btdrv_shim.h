#pragma once
#include <switch.h>

#ifdef __cplusplus
extern "C" {
#endif

Result btdrvInitializeBluetoothFwd(Service* s, Handle *out_handle);
Result btdrvFinalizeBluetoothFwd(Service* s);

Result btdrvCancelBondFwd(Service* s, const BluetoothAddress *address);

//Result btdrvGetEventInfoFwd(Service* s, BluetoothEventType *type, u8 *buffer, size_t size);
Result btdrvInitializeHidFwd(Service* s, Handle *out_handle, u16 version);
Result btdrvWriteHidDataFwd(Service* s, const BluetoothAddress *address, const BluetoothHidData *data);

//Result btdrvGetPairedDeviceInfoFwd(Service* s, const BluetoothAddress *address, BluetoothDevicesSettings *device);

Result btdrvFinalizeHidFwd(Service* s);
//Result btdrvGetHidEventInfoFwd(Service* s, HidEventType *type, u8 *buffer, size_t size);
Result btdrvRegisterHidReportEventFwd(Service* s, Handle *out_handle);
Result btdrvGetHidReportEventInfoDeprecatedFwd(Service* s, HidEventType *type, u8 *buffer, size_t size);
Result btdrvGetHidReportEventInfoFwd(Service* s, Handle *out_handle);
Result btdrvInitializeBleFwd(Service* s, Handle *out_handle);
Result btdrvFinalizeBleFwd(Service* s);
//Result btdrvGetBleManagedEventInfoFwd(Service* s, BleEventType *type, u8 *buffer, u16 length)

#ifdef __cplusplus
}
#endif
