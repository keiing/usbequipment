package dev.usbequipment.usbequipment
import android.util.Log
import io.flutter.plugin.common.BasicMessageChannel
import io.flutter.plugin.common.BinaryMessenger
import io.flutter.plugin.common.StandardMessageCodec
import kotlin.concurrent.timerTask

class BasicMessageChannelAdaper(messenger: BinaryMessenger) :

    BasicMessageChannel.MessageHandler<Any> {

    private val TAG: String = BasicMessageChannelAdaper::class.java.getSimpleName()

    private var channel: BasicMessageChannel<Any>
    private var count = 0

    init {
        channel = BasicMessageChannel(messenger, "usbequipment/basicMessage", StandardMessageCodec())
        channel.setMessageHandler(this)
        // 绑定成功后通知一次flutter后续不通知
        startTimer()
        Log.d(TAG, "BasicMessageChannelAdaper init")
    }

    fun onDetachedFromEngine(){
        channel.setMessageHandler(null)
    }


    fun startTimer() {
        var map = mapOf("count" to count++)
        channel.send(map, object : BasicMessageChannel.Reply<Any> {
            override fun reply(reply: Any?) {
                Log.d(TAG, "COUNT-reply")
            }
        })
    }

    override fun onMessage(message: Any?, reply: BasicMessageChannel.Reply<Any>) {
        reply.reply("android 发送给 flutter的数据")
    }
}