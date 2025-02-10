import 'dart:convert';

import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';
import 'package:usb_serial/usb_serial.dart';

import 'usbequipment_platform_interface.dart';

/// An implementation of [UsbequipmentPlatform] that uses method channels.
class MethodChannelUsbequipment extends UsbequipmentPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('usbequipment');
  

  @override
  Future<String?> getPlatformVersion() async {
    final version =
        await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }

  @override
  Future<List> listDevices() async {
    final list = (await methodChannel.invokeMethod<List>('listDevices'));
    if (list == null) {
      return [];
    }
    return list.map<UsbDevice>(UsbDevice.fromJSON).toList();
  }
}
