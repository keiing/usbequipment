import 'dart:io';

import 'package:usb_serial/usb_serial.dart' as usb_serial;
import 'package:shenyukejiusb/shenyukejiusb.dart' as shenyukejiusb;

import 'usbequipment_platform_interface.dart';

export 'package:usb_serial/usb_serial.dart';
export 'package:usb_serial/transaction.dart';
export 'package:shenyukejiusb/src/desktop.dart';

// class UsbDevice extends usb_serial.UsbDevice {
//   UsbDevice(
//       super.deviceName,
//       super.vid,
//       super.pid,
//       super.productName,
//       super.manufacturerName,
//       super.deviceId,
//       super.serial,
//       super.interfaceCount);
// }

class Usbequipment {
  static UsbequipmentPlatform get instance {
    return UsbequipmentPlatform.instance;
  }

  // 获取系统版本
  static Future<String?> getPlatformVersion() {
    return UsbequipmentPlatform.instance.getPlatformVersion();
  }

  /// 获取 usb 列表
  ///
  ///
  static Future<List<usb_serial.UsbDevice>> getUSBDeviceList() async {
    List<usb_serial.UsbDevice> deviceList = [];
    try {
      if (Platform.isAndroid) {
        deviceList = (await usb_serial.UsbSerial.listDevices())
            .cast<usb_serial.UsbDevice>();
      } else if (Platform.isWindows) {
        /// Todo: windows 获取usb设备列表未完善
        // deviceList = (await UsbequipmentPlatform.instance.listDevices())
        //     .cast<usb_serial.UsbDevice>();
      }
    } catch (err) {}
    return deviceList;
  }

  static Stream<dynamic>? get usbEventStream {
    return UsbequipmentPlatform.usbEventStream;
  }
  // static Stream<usb_serial.UsbEvent>? get usbEventStream {
  //   if (Platform.isAndroid) {
  //     return usb_serial.UsbSerial.usbEventStream;
  //   } else if (Platform.isWindows) {
  //     return UsbequipmentPlatform.usbEventStream;
  //   }
  // }

  // 设置发送消息
  static void setMessageHandler(Future<dynamic> Function(dynamic)? handler) {
    return UsbequipmentPlatform.basicMessageChannel.setMessageHandler(handler);
  }

  // 设置接受消息
  static Future<dynamic> send(dynamic message) {
    return UsbequipmentPlatform.basicMessageChannel.send(message);
  }
}
