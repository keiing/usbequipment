#include "include/usbequipment/usbequipment_plugin_c_api.h"

#include "usbequipment_plugin.h"

void UsbequipmentPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar)
{
    usbequipment::UsbequipmentPlugin::RegisterWithRegistrar(
        flutter::PluginRegistrarManager::GetInstance()
            ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));

    usbequipment::UsbequipmentPlugin::RegisterStreamWithRegistrar(
        flutter::PluginRegistrarManager::GetInstance()->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
    // usbequipment::UsbequipmentPlugin::RegisterStreamWithRegistrar(
    //     flutter::PluginRegistrarManager::GetInstance()
    //         ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar)

    // );
}