package dev.usbequipment.usbequipment


import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.media.AudioManager
import android.os.Build
import android.util.Log
import io.flutter.embedding.engine.plugins.FlutterPlugin
import io.flutter.plugin.common.BinaryMessenger
import io.flutter.plugin.common.EventChannel
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel


/** UsbequipmentPlugin */
class UsbequipmentPlugin : FlutterPlugin {

    private val TAG: String = UsbequipmentPlugin::class.java.getSimpleName()

    /// 静态变量
    companion object {
        private const val ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION"
        private const val MEHTOD_HANDLER_NAME = "usbequipment"
        private const val STREAM_HANDLER_NAME = "usbequipment/stream"
        // usb连接/断开
        const val ACTION_USB_ATTACHED = "android.hardware.usb.action.USB_DEVICE_ATTACHED"
        const val ACTION_USB_DETACHED = "android.hardware.usb.action.USB_DEVICE_DETACHED"
        // 音量大小发生变化
        const val VOLUME_CHANGED_ACTION = "android.media.VOLUME_CHANGED_ACTION"
    }

    private lateinit var audioManager: AudioManager;

    /// 自定义属性
    private var context: Context? = null
    private var m_InterfaceId = 0
    private var messenger: BinaryMessenger? = null
    // 事件派发对象
    private var eventSink: EventChannel.EventSink? = null



    // usb 广播
//    private val usbReceiver: BroadcastReceiver = object : BroadcastReceiver() {
//        // 重写回复
//        override fun onReceive(context: Context, intent: Intent) {
//            val action = intent.action ?: return
//            if (eventSink != null) {
//                val dev = HashMap<String, Any?>()
//                dev["event"] = VOLUME_CHANGED_ACTION;
//                eventSink!!.success(dev)
//            }
//
//        }
//
//    }

    // 声音 广播
    private val audioReceiver: BroadcastReceiver = object : BroadcastReceiver() {
        // 重写回复
        override fun onReceive(context: Context, intent: Intent) {

            Log.d(TAG,"onrecivie");

            when (intent.action) {
                VOLUME_CHANGED_ACTION -> {
                    if (eventSink != null) {
                        val dev = HashMap<String, Any?>()
                        dev["event"] = intent.action;
                        eventSink!!.success(dev)
                        return;
                    }
                }

                else -> {
                    return;
                }
            }
        }
    }
    
    private var methodChannel: MethodChannel? = null;
    private var eventChannel: EventChannel?=null;

    /// 绑定监听事件
    private val streamHandler: EventChannel.StreamHandler = object : EventChannel.StreamHandler {
        override fun onListen(arguments: Any?, events: EventChannel.EventSink?) {
            // 处理监听事件
            // 可以在这里进行初始化操作或发送事件到 events
            eventSink = events
        }

        override fun onCancel(arguments: Any?) {
            // 处理取消监听事件
            // 可以在这里进行清理操作
            eventSink = null
        }
    }


    private val methodCallHandler:MethodChannel.MethodCallHandler = object : MethodChannel.MethodCallHandler {
            
        /// method 触发事件
        override fun onMethodCall(call: MethodCall, result: MethodChannel.Result) {

            when (call.method) {
                "getPlatformVersion" -> {
                    getPlatformVersion(result);

                    if (eventSink != null) {
                        val dev = HashMap<String, Any?>()
                        dev["event"] = ACTION_USB_DETACHED;
                        eventSink!!.success(dev)
                    }
                }

                else -> {
                    result.notImplemented()
                }
            }
        }
    }


    /// Todo:分割线
    /// The MethodChannel that will the communication between Flutter and native Android
    /// 绑定 MethodChannel
    private fun getPlatformVersion(result: MethodChannel.Result) {


        try {
            // 回复成功
            result.success("Android ${Build.VERSION.RELEASE}");
        } catch (e: Exception) {
            // 回复失败
            result.error("getPlatformVersion", "Android ${Build.VERSION.RELEASE}", null);
        } finally {
            // 无论是否发生错误都需要执行的代码
        }

    }

    /// 绑定流事件
    private fun register(currentMessenger: BinaryMessenger, currentContext: Context) {
        // message
        messenger = currentMessenger
        context = currentContext

        // 绑定 MethodChannel
        methodChannel = MethodChannel(messenger!!, MEHTOD_HANDLER_NAME)
        methodChannel?.setMethodCallHandler(methodCallHandler)

        // 绑定 EventChangl
        eventChannel = EventChannel(messenger!!, STREAM_HANDLER_NAME)
        eventChannel?.setStreamHandler(streamHandler)

        // 绑定usb连接与关闭
        // 设置选项
//        val filter = IntentFilter()
//        filter.addAction(ACTION_USB_DETACHED)
//        filter.addAction(ACTION_USB_ATTACHED)
        // 注册这个广播
//        context!!.registerReceiver(usbReceiver, filter)
    }

    /// 移除所有事件
    private fun unregister() {
        // 解绑 methodChannel
        methodChannel?.setMethodCallHandler(null)
        methodChannel = null
        // 解绑流
        eventChannel?.setStreamHandler(null)
        eventChannel = null
        // 解绑 receiver
//        context!!.unregisterReceiver(usbReceiver)
        context = null
        messenger = null
    }


    private var basicMessageChannelAdaper : BasicMessageChannelAdaper? = null;
    /// 初始化 入口函数
    override fun onAttachedToEngine(flutterPluginBinding: FlutterPlugin.FlutterPluginBinding) {
        register(flutterPluginBinding.binaryMessenger, flutterPluginBinding.getApplicationContext());

        // 获取 audio 实例
        audioManager = context!!.getSystemService(Context.AUDIO_SERVICE) as AudioManager
        // 创建事件
        val filter = IntentFilter()
        filter.addAction(VOLUME_CHANGED_ACTION)
        context?.registerReceiver(audioReceiver, filter);
        basicMessageChannelAdaper = BasicMessageChannelAdaper(messenger!!);
        Log.d(TAG, "onAttachedToEngine")


    }
    override fun onDetachedFromEngine(flutterPluginBinding: FlutterPlugin.FlutterPluginBinding) {
        unregister()
        // 解绑 监听声音变化
        context?.unregisterReceiver(audioReceiver)
        basicMessageChannelAdaper?.onDetachedFromEngine();
        basicMessageChannelAdaper = null
        Log.d(TAG, "onDetachedFromEngine")

    }
}
