import 'package:flutter/services.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';
import 'package:usbequipment/usbequipment.dart';

import 'usbequipment_method_channel.dart';

abstract class UsbequipmentPlatform extends PlatformInterface {
  /// Constructs a UsbequipmentPlatform.
  UsbequipmentPlatform() : super(token: _token);

  static final Object _token = Object();

  static UsbequipmentPlatform _instance = MethodChannelUsbequipment();

  static const EventChannel eventChannel = EventChannel('usbequipment/stream');

  static const BasicMessageChannel basicMessageChannel =
      BasicMessageChannel('usbequipment/basicMessage', StandardMessageCodec());

  /// The default instance of [UsbequipmentPlatform] to use.
  ///
  /// Defaults to [MethodChannelUsbequipment].
  static UsbequipmentPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [UsbequipmentPlatform] when
  /// they register themselves.
  static set instance(UsbequipmentPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  static Stream<dynamic>? _eventStream;

  /// Use this stream to detect if a USB device is plugged in or removed.
  ///
  /// Example
  /// ```dart
  /// @override
  /// void initState() {
  ///   super.initState();
  ///
  ///   UsbSerial.usbEventStream.listen((String event) {
  ///     print("Usb Event $event");
  ///     setState(() {
  ///       _lastEvent = event;
  ///     });
  ///   });
  /// }
  /// ```
  static Stream<dynamic>? get usbEventStream {
    // if (_eventStream == null) {
    //   _eventStream = eventChannel.receiveBroadcastStream();
    // }
    _eventStream ??= eventChannel.receiveBroadcastStream();
    return _eventStream;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }

  Future<List> listDevices() {
    throw UnimplementedError('listDevices() has not been implemented.');
  }
}
