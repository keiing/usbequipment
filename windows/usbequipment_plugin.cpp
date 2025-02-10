#include "usbequipment_plugin.h"

namespace usbequipment
{
  void ThreadProc(std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&events)
  {
    for (int i = 0; i < 100; ++i)
    {
      std::cout << "子级别:i = " << i << std::endl;
      events->Success(flutter::EncodableValue(i));

      // 让该线程休眠两秒
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
  }

  // void success(std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&events)
  // {
  //   std::cout << "successF" << std::endl;
  //   ::Sleep(3000);
  //   auto map = (flutter::EncodableMap(
  //       {
  //           {flutter::EncodableValue("deviceName"), flutter::EncodableValue("deviceNameF")},
  //           {flutter::EncodableValue("vid"), 3},
  //           {flutter::EncodableValue("pid"), 4},
  //           {flutter::EncodableValue("productName"), flutter::EncodableValue("custom windows manufacturerName2")},
  //           {flutter::EncodableValue("manufacturerName"), flutter::EncodableValue("manufacturerName 2")},
  //           {flutter::EncodableValue("deviceId"), 5},
  //           {flutter::EncodableValue("serialNumber"), flutter::EncodableValue("serial2")},
  //           {flutter::EncodableValue("interfaceCount"), 6},
  //           {flutter::EncodableValue("event"), flutter::EncodableValue("event")},

  //       }));

  //   events.get()->Success(map);
  // }

  void longRunningTask()
  {
    // 子线程开启成功
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "print" << std::endl;
  }

  class UsbDevice
  {
  public:
    char deviceName[128];       // deviceName
    int vid;                    // deviceVid
    int deviceId;               // deviceId
    int pid;                    // devicePid
    char manufacturerName[128]; // deviceName
    char productName[128];      // deviceName
  };

  std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> UsbequipmentPlugin::customEventChannel = nullptr;

  // static MethodChannel
  void UsbequipmentPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    // ------- MethodChannel
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "usbequipment",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<UsbequipmentPlugin>();

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result)
        {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });
    registrar->AddPlugin(std::move(plugin));
    // ------- MethodChannel
  }

  // static eventChange
  void UsbequipmentPlugin::RegisterStreamWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {

    // std::thread t(usbequipment::longRunningTask);
    // t.detach();

    // ------- EventChannel
    auto plugin = std::make_unique<UsbequipmentPlugin>();
    // create stream handler object
    auto eventHandler = std::make_unique<
        flutter::StreamHandlerFunctions<flutter::EncodableValue>>(
        [plugin_pointer = plugin.get()](
            const flutter::EncodableValue *arguments,
            std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&events)
            -> std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>>
        {
          return plugin_pointer->OnListen(arguments, std::move(events));
        },
        [plugin_pointer = plugin.get()](const flutter::EncodableValue *arguments)
            -> std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>>
        {
          // 释放
          return plugin_pointer->OnCancel(arguments);
        });
    auto eventChannel = std::make_unique<flutter::EventChannel<flutter::EncodableValue>>(
        registrar->messenger(), "usbequipment/stream",
        &flutter::StandardMethodCodec::GetInstance());

    eventChannel->SetStreamHandler(std::move(eventHandler));

    // ------- EventChannel

    // ---------2
  }

  UsbequipmentPlugin::UsbequipmentPlugin() {}

  UsbequipmentPlugin::~UsbequipmentPlugin() {}

  void UsbequipmentPlugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    if (method_call.method_name().compare("getPlatformVersion") == 0)
    {
      std::ostringstream version_stream;
      version_stream << "Windows ";
      if (IsWindows10OrGreater())
      {
        version_stream << "10+";
      }
      else if (IsWindows8OrGreater())
      {
        version_stream << "8";
      }
      else if (IsWindows7OrGreater())
      {
        version_stream << "7";
      }

      if (customEventChannel != nullptr)
      {
        // 另外启动一个线程
        // 防止当前线程阻塞
        std::thread t([&version_stream]()
                      {

                        // 让该线程休眠两秒
                        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "customEventChannel != nullptr" << std::endl;

          customEventChannel.get()->Success(flutter::EncodableValue(version_stream.str())); });
        t.detach();
      }

      result->Success(flutter::EncodableValue("success 1"));
    }
    else if (method_call.method_name().compare("listDevices") == 0)
    {
      // result->Success(flutter::EncodableList({1000.0, 2.0, 3.4, 7.0, 50.0}));
      // char device[] = {1000.0, 2.0, 3.4, 7.0, 50.0};
      auto pl = flutter::EncodableList{};

      auto map = flutter::EncodableMap(
          {
              {flutter::EncodableValue("deviceName"), flutter::EncodableValue("deviceName")},
              {flutter::EncodableValue("vid"), 1},
              {flutter::EncodableValue("pid"), 1},
              {flutter::EncodableValue("productName"), flutter::EncodableValue("custom windows manufacturerName")},
              {flutter::EncodableValue("manufacturerName"), flutter::EncodableValue("manufacturerName")},
              {flutter::EncodableValue("deviceId"), 1},
              {flutter::EncodableValue("serialNumber"), flutter::EncodableValue("serial")},
              {flutter::EncodableValue("interfaceCount"), 1},
          });

      pl.push_back(map);

      std::thread t(usbequipment::longRunningTask);
      t.detach();
      std::cout << "中文" << std::endl;

      result->Success(pl);
      // result->Success( std::make_unique<flutter::EncodableValue>(flutter::EncodableList({1000.0, 2.0, 3.4, 7.0, 50.0}));
    }

    else
    {
      result->NotImplemented();
    }
  }

  // activelySendingMessagesToTheFlutterEnd
  std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> UsbequipmentPlugin::OnListen(const flutter::EncodableValue *arguments, std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&events)
  {
    std::cout << "OnListen" << std::endl;
    // proactivelySending
    // events.reset(events.release());
    // std::ofstream outfile;
    // outfile.open("afile.txt");
    // std::cout << arguments << std::endl;
    // outfile << "write to file data" << std::endl;
    // outfile << events << std::endl;
    // std::cin.ignore();
    // outfile.close();
    // std::cout << events << std::endl;
    // auto pl = flutter::EncodableList{};
    // 涓?鏂?
    // Timer t;
    // int i = 0;
    // bool run = true;
    // t.setTimeout([&t, &run]()
    //              {11q
    // std::cout << u8"缁忚繃6.18s"  << std::endl;
    // t.stop(); run = false; },
    //              6180);
    // 绔嬪嵆鎵ц??
    // events.reset(events.release());

    auto pl = flutter::EncodableList{};

    auto map = flutter::EncodableMap(
        {
            {flutter::EncodableValue("deviceName"), flutter::EncodableValue("deviceNameF")},
            {flutter::EncodableValue("vid"), 3},
            {flutter::EncodableValue("pid"), 4},
            {flutter::EncodableValue("productName"), flutter::EncodableValue("custom windows manufacturerName2")},
            {flutter::EncodableValue("manufacturerName"), flutter::EncodableValue("manufacturerName 2")},
            {flutter::EncodableValue("deviceId"), 5},
            {flutter::EncodableValue("serialNumber"), flutter::EncodableValue("serial2")},
            {flutter::EncodableValue("interfaceCount"), 6},
            // {flutter::EncodableValue("event"), flutter::EncodableValue("event")},

        });

    // events.get()->Success(map);

    // ::Sleep(10000);
    std::cout << "writing to the file2" << std::endl;
    // // events.reset(events.release());
    // events.get()->Success(map);
    // onceASubthreadIsUsedItWillFail
    // ::Sleep(10000);
    // std::thread t(usbequipment::success, events);
    // t.join();
    // HANDLE thread = CreateThread(NULL, 0, usbequipment::ThreadProc, NULL, 0, NULL);
    // CloseHandle(thread);

    // for (int i = 0; i < 5; ++i)
    // {
    //   std::cout << "主线程1 = " << i << std::endl;
    // }

    // 储存
    customEventChannel = std::move(events);

    // 另外启动一个线程
    // 防止当前线程阻塞
    // std::thread t(ThreadProc, std::move(customEventChannel));
    // t.detach();

    return nullptr;
  }

  // triggerWhenFlutterCancelsListening
  std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> UsbequipmentPlugin::OnCancel(const flutter::EncodableValue *arguments)
  {
    std::cout << "OnCancel" << std::endl;
    // 释放
    customEventChannel.reset();
    customEventChannel = nullptr;
    return nullptr;
  }

} // namespace usbequipment
