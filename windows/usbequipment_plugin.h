#ifndef FLUTTER_PLUGIN_USBEQUIPMENT_PLUGIN_H_
#define FLUTTER_PLUGIN_USBEQUIPMENT_PLUGIN_H_

// This must be included before many other Windows headers.
#include <windows.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <flutter/standard_method_codec.h>

#include <flutter/event_sink.h>
#include <flutter/event_channel.h>
#include <flutter/event_stream_handler.h>
#include <flutter/event_stream_handler_functions.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <memory>
#include <sstream>

#include <fstream>
#include <iostream>
#include <thread>

namespace usbequipment
{
    void ThreadProc(std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&events);

    class UsbequipmentPlugin : public flutter::Plugin
    {
    public:
        static std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>
            UsbequipmentPlugin::customEventChannel;

        static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);
        static void RegisterStreamWithRegistrar(flutter::PluginRegistrarWindows *registrar);

        UsbequipmentPlugin();

        virtual ~UsbequipmentPlugin();

        // Disallow copy and assign.
        UsbequipmentPlugin(const UsbequipmentPlugin &) = delete;
        UsbequipmentPlugin &operator=(const UsbequipmentPlugin &) = delete;

        // Called when a method is called on this plugin's channel from Dart.
        void HandleMethodCall(
            const flutter::MethodCall<flutter::EncodableValue> &method_call,
            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

        std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> OnListen(const flutter::EncodableValue *arguments, std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&events);

        std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> OnCancel(const flutter::EncodableValue *arguments);

        // void success(std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&events);
        void longRunningTask();
    };

} // namespace usbequipment

#endif // FLUTTER_PLUGIN_USBEQUIPMENT_PLUGIN_H_
