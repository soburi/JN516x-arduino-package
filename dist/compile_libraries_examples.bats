#!/usr/bin/env bats


setup() {
        echo "setup"
        ADIR=~/arduino-${ARDUINO_VERSION}
        PATH=$ADIR:$PATH:~/BA2-toolchain/bin
        BUILDDIR=/tmp/compile_example
        mkdir -p ${BUILDDIR}
}

teardown() {
        echo "teardown"
        rm -rf ${BUILDDIR}
}


compile_example() {
                                ${ADIR}/arduino-builder -hardware ${ADIR}/hardware/ \
                                                                                -hardware ~/packages/ \
                                                                                -tools ~/BA2-toolchain \
                                                                                -tools ~/intiki \
                                                                                -tools ${ADIR}/tools-builder/ \
                                                                                -tools ${ADIR}/hardware/tools/ \
                                                                                -libraries ${ADIR}/libraries/ \
                                                                                -fqbn "intiki:intiki:twelite:config=TWELITE_RED, trace=OFF, rpl=ON" \
                                                                                -build-path=${BUILDDIR} \
                                                                                ${ADIR}/$1
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/CircuitPlaygroundFirmata/CircuitPlaygroundFirmata.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/CircuitPlaygroundFirmata/CircuitPlaygroundFirmata.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/FidgetSpinner/FidgetSpinner.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/FidgetSpinner/FidgetSpinner.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_Accelerometer/Hello_Accelerometer.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_Accelerometer/Hello_Accelerometer.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_Blink/Hello_Blink.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_Blink/Hello_Blink.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_Buttons/Hello_Buttons.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_Buttons/Hello_Buttons.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_LightSensor/Hello_LightSensor.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_LightSensor/Hello_LightSensor.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_NeoPixels/Hello_NeoPixels.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_NeoPixels/Hello_NeoPixels.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_SlideSwitch/Hello_SlideSwitch.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_SlideSwitch/Hello_SlideSwitch.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_SoundSensor/Hello_SoundSensor.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_SoundSensor/Hello_SoundSensor.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_Speaker/Hello_Speaker.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_Speaker/Hello_Speaker.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_Temperature/Hello_Temperature.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Hello_CircuitPlayground/Hello_Temperature/Hello_Temperature.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Infrared_NeoPixel/Infrared_NeoPixel.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Infrared_NeoPixel/Infrared_NeoPixel.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Infrared_Read/Infrared_read.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Infrared_Read/Infrared_read.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Infrared_Record/Infrared_Record.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Infrared_Record/Infrared_Record.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Infrared_Send/Infrared_Send.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Infrared_Send/Infrared_Send.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Infrared_Testpattern/Infrared_Testpattern.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Infrared_Testpattern/Infrared_Testpattern.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Microphone/mic_fft/mic_fft.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Microphone/mic_fft/mic_fft.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Microphone/mic_meter/mic_meter.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Microphone/mic_meter/mic_meter.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/ScatchSensorBoard/ScratchSensorBoard.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/ScatchSensorBoard/ScratchSensorBoard.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Speech/Talk_UK_Acorn/Talk_UK_Acorn.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Speech/Talk_UK_Acorn/Talk_UK_Acorn.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Speech/Talk_US_Clock/Talk_US_Clock.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Speech/Talk_US_Clock/Talk_US_Clock.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Speech/Talk_US_Large/Talk_US_Large.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Speech/Talk_US_Large/Talk_US_Large.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Speech/Talk_US_Male/Talk_US_Male.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Speech/Talk_US_Male/Talk_US_Male.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/Speech/Talk_US_TI99/Talk_US_TI99.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/Speech/Talk_US_TI99/Talk_US_TI99.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/accelTap/accelTap.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/accelTap/accelTap.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/accel_mouse/accel_mouse.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/accel_mouse/accel_mouse.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/analog_sensors/analog_sensors.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/analog_sensors/analog_sensors.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/color_sense/color_sense.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/color_sense/color_sense.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/comm_badge/comm_badge.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/comm_badge/comm_badge.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/demo/demo.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/demo/demo.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/mega_demo/mega_demo.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/mega_demo/mega_demo.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/tachometer/tachometer.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/tachometer/tachometer.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/tachometer_led_display/tachometer_led_display.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/tachometer_led_display/tachometer_led_display.ino
}

@test "Compile libraries/Adafruit_Circuit_Playground/examples/vu_meter/vu_meter.ino." {
	skip; compile_example libraries/Adafruit_Circuit_Playground/examples/vu_meter/vu_meter.ino
}

@test "Compile libraries/Bridge/examples/Bridge/Bridge.ino." {
	skip; compile_example libraries/Bridge/examples/Bridge/Bridge.ino
}

@test "Compile libraries/Bridge/examples/ConsoleAsciiTable/ConsoleAsciiTable.ino." {
	skip; compile_example libraries/Bridge/examples/ConsoleAsciiTable/ConsoleAsciiTable.ino
}

@test "Compile libraries/Bridge/examples/ConsolePixel/ConsolePixel.ino." {
	skip; compile_example libraries/Bridge/examples/ConsolePixel/ConsolePixel.ino
}

@test "Compile libraries/Bridge/examples/ConsoleRead/ConsoleRead.ino." {
	skip; compile_example libraries/Bridge/examples/ConsoleRead/ConsoleRead.ino
}

@test "Compile libraries/Bridge/examples/Datalogger/Datalogger.ino." {
	skip; compile_example libraries/Bridge/examples/Datalogger/Datalogger.ino
}

@test "Compile libraries/Bridge/examples/FileWriteScript/FileWriteScript.ino." {
	skip; compile_example libraries/Bridge/examples/FileWriteScript/FileWriteScript.ino
}

@test "Compile libraries/Bridge/examples/HttpClient/HttpClient.ino." {
	skip; compile_example libraries/Bridge/examples/HttpClient/HttpClient.ino
}

@test "Compile libraries/Bridge/examples/HttpClientConsole/HttpClientConsole.ino." {
	skip; compile_example libraries/Bridge/examples/HttpClientConsole/HttpClientConsole.ino
}

@test "Compile libraries/Bridge/examples/MailboxReadMessage/MailboxReadMessage.ino." {
	skip; compile_example libraries/Bridge/examples/MailboxReadMessage/MailboxReadMessage.ino
}

@test "Compile libraries/Bridge/examples/Process/Process.ino." {
	skip; compile_example libraries/Bridge/examples/Process/Process.ino
}

@test "Compile libraries/Bridge/examples/RemoteDueBlink/RemoteDueBlink.ino." {
	skip; compile_example libraries/Bridge/examples/RemoteDueBlink/RemoteDueBlink.ino
}

@test "Compile libraries/Bridge/examples/ShellCommands/ShellCommands.ino." {
	skip; compile_example libraries/Bridge/examples/ShellCommands/ShellCommands.ino
}

@test "Compile libraries/Bridge/examples/TemperatureWebPanel/TemperatureWebPanel.ino." {
	skip; compile_example libraries/Bridge/examples/TemperatureWebPanel/TemperatureWebPanel.ino
}

@test "Compile libraries/Bridge/examples/TimeCheck/TimeCheck.ino." {
	skip; compile_example libraries/Bridge/examples/TimeCheck/TimeCheck.ino
}

@test "Compile libraries/Bridge/examples/WiFiStatus/WiFiStatus.ino." {
	skip; compile_example libraries/Bridge/examples/WiFiStatus/WiFiStatus.ino
}

@test "Compile libraries/Bridge/examples/YunFirstConfig/YunFirstConfig.ino." {
	skip; compile_example libraries/Bridge/examples/YunFirstConfig/YunFirstConfig.ino
}

@test "Compile libraries/Bridge/examples/YunSerialTerminal/YunSerialTerminal.ino." {
	skip; compile_example libraries/Bridge/examples/YunSerialTerminal/YunSerialTerminal.ino
}

@test "Compile libraries/Esplora/examples/Beginners/EsploraAccelerometer/EsploraAccelerometer.ino." {
	skip; compile_example libraries/Esplora/examples/Beginners/EsploraAccelerometer/EsploraAccelerometer.ino
}

@test "Compile libraries/Esplora/examples/Beginners/EsploraBlink/EsploraBlink.ino." {
	skip; compile_example libraries/Esplora/examples/Beginners/EsploraBlink/EsploraBlink.ino
}

@test "Compile libraries/Esplora/examples/Beginners/EsploraJoystickMouse/EsploraJoystickMouse.ino." {
	skip; compile_example libraries/Esplora/examples/Beginners/EsploraJoystickMouse/EsploraJoystickMouse.ino
}

@test "Compile libraries/Esplora/examples/Beginners/EsploraLedShow/EsploraLedShow.ino." {
	skip; compile_example libraries/Esplora/examples/Beginners/EsploraLedShow/EsploraLedShow.ino
}

@test "Compile libraries/Esplora/examples/Beginners/EsploraLedShow2/EsploraLedShow2.ino." {
	skip; compile_example libraries/Esplora/examples/Beginners/EsploraLedShow2/EsploraLedShow2.ino
}

@test "Compile libraries/Esplora/examples/Beginners/EsploraLightCalibrator/EsploraLightCalibrator.ino." {
	skip; compile_example libraries/Esplora/examples/Beginners/EsploraLightCalibrator/EsploraLightCalibrator.ino
}

@test "Compile libraries/Esplora/examples/Beginners/EsploraMusic/EsploraMusic.ino." {
	skip; compile_example libraries/Esplora/examples/Beginners/EsploraMusic/EsploraMusic.ino
}

@test "Compile libraries/Esplora/examples/Beginners/EsploraSoundSensor/EsploraSoundSensor.ino." {
	skip; compile_example libraries/Esplora/examples/Beginners/EsploraSoundSensor/EsploraSoundSensor.ino
}

@test "Compile libraries/Esplora/examples/Beginners/EsploraTemperatureSensor/EsploraTemperatureSensor.ino." {
	skip; compile_example libraries/Esplora/examples/Beginners/EsploraTemperatureSensor/EsploraTemperatureSensor.ino
}

@test "Compile libraries/Esplora/examples/Experts/EsploraKart/EsploraKart.ino." {
	skip; compile_example libraries/Esplora/examples/Experts/EsploraKart/EsploraKart.ino
}

@test "Compile libraries/Esplora/examples/Experts/EsploraPong/EsploraPong.ino." {
	skip; compile_example libraries/Esplora/examples/Experts/EsploraPong/EsploraPong.ino
}

@test "Compile libraries/Esplora/examples/Experts/EsploraRemote/EsploraRemote.ino." {
	skip; compile_example libraries/Esplora/examples/Experts/EsploraRemote/EsploraRemote.ino
}

@test "Compile libraries/Esplora/examples/Experts/EsploraTable/EsploraTable.ino." {
	skip; compile_example libraries/Esplora/examples/Experts/EsploraTable/EsploraTable.ino
}

@test "Compile libraries/Ethernet/examples/AdvancedChatServer/AdvancedChatServer.ino." {
	skip; compile_example libraries/Ethernet/examples/AdvancedChatServer/AdvancedChatServer.ino
}

@test "Compile libraries/Ethernet/examples/BarometricPressureWebServer/BarometricPressureWebServer.ino." {
	skip; compile_example libraries/Ethernet/examples/BarometricPressureWebServer/BarometricPressureWebServer.ino
}

@test "Compile libraries/Ethernet/examples/ChatServer/ChatServer.ino." {
	skip; compile_example libraries/Ethernet/examples/ChatServer/ChatServer.ino
}

@test "Compile libraries/Ethernet/examples/DhcpAddressPrinter/DhcpAddressPrinter.ino." {
	skip; compile_example libraries/Ethernet/examples/DhcpAddressPrinter/DhcpAddressPrinter.ino
}

@test "Compile libraries/Ethernet/examples/DhcpChatServer/DhcpChatServer.ino." {
	skip; compile_example libraries/Ethernet/examples/DhcpChatServer/DhcpChatServer.ino
}

@test "Compile libraries/Ethernet/examples/TelnetClient/TelnetClient.ino." {
	skip; compile_example libraries/Ethernet/examples/TelnetClient/TelnetClient.ino
}

@test "Compile libraries/Ethernet/examples/UDPSendReceiveString/UDPSendReceiveString.ino." {
	skip; compile_example libraries/Ethernet/examples/UDPSendReceiveString/UDPSendReceiveString.ino
}

@test "Compile libraries/Ethernet/examples/UdpNtpClient/UdpNtpClient.ino." {
	skip; compile_example libraries/Ethernet/examples/UdpNtpClient/UdpNtpClient.ino
}

@test "Compile libraries/Ethernet/examples/WebClient/WebClient.ino." {
	skip; compile_example libraries/Ethernet/examples/WebClient/WebClient.ino
}

@test "Compile libraries/Ethernet/examples/WebClientRepeating/WebClientRepeating.ino." {
	skip; compile_example libraries/Ethernet/examples/WebClientRepeating/WebClientRepeating.ino
}

@test "Compile libraries/Ethernet/examples/WebServer/WebServer.ino." {
	skip; compile_example libraries/Ethernet/examples/WebServer/WebServer.ino
}

@test "Compile libraries/Firmata/examples/AllInputsFirmata/AllInputsFirmata.ino." {
	skip; compile_example libraries/Firmata/examples/AllInputsFirmata/AllInputsFirmata.ino
}

@test "Compile libraries/Firmata/examples/AnalogFirmata/AnalogFirmata.ino." {
	skip; compile_example libraries/Firmata/examples/AnalogFirmata/AnalogFirmata.ino
}

@test "Compile libraries/Firmata/examples/EchoString/EchoString.ino." {
	skip; compile_example libraries/Firmata/examples/EchoString/EchoString.ino
}

@test "Compile libraries/Firmata/examples/OldStandardFirmata/OldStandardFirmata.ino." {
	skip; compile_example libraries/Firmata/examples/OldStandardFirmata/OldStandardFirmata.ino
}

@test "Compile libraries/Firmata/examples/ServoFirmata/ServoFirmata.ino." {
	skip; compile_example libraries/Firmata/examples/ServoFirmata/ServoFirmata.ino
}

@test "Compile libraries/Firmata/examples/SimpleAnalogFirmata/SimpleAnalogFirmata.ino." {
	skip; compile_example libraries/Firmata/examples/SimpleAnalogFirmata/SimpleAnalogFirmata.ino
}

@test "Compile libraries/Firmata/examples/SimpleDigitalFirmata/SimpleDigitalFirmata.ino." {
	skip; compile_example libraries/Firmata/examples/SimpleDigitalFirmata/SimpleDigitalFirmata.ino
}

@test "Compile libraries/Firmata/examples/StandardFirmata/StandardFirmata.ino." {
	skip; compile_example libraries/Firmata/examples/StandardFirmata/StandardFirmata.ino
}

@test "Compile libraries/Firmata/examples/StandardFirmataBLE/StandardFirmataBLE.ino." {
	skip; compile_example libraries/Firmata/examples/StandardFirmataBLE/StandardFirmataBLE.ino
}

@test "Compile libraries/Firmata/examples/StandardFirmataChipKIT/StandardFirmataChipKIT.ino." {
	skip; compile_example libraries/Firmata/examples/StandardFirmataChipKIT/StandardFirmataChipKIT.ino
}

@test "Compile libraries/Firmata/examples/StandardFirmataEthernet/StandardFirmataEthernet.ino." {
	skip; compile_example libraries/Firmata/examples/StandardFirmataEthernet/StandardFirmataEthernet.ino
}

@test "Compile libraries/Firmata/examples/StandardFirmataPlus/StandardFirmataPlus.ino." {
	skip; compile_example libraries/Firmata/examples/StandardFirmataPlus/StandardFirmataPlus.ino
}

@test "Compile libraries/Firmata/examples/StandardFirmataWiFi/StandardFirmataWiFi.ino." {
	skip; compile_example libraries/Firmata/examples/StandardFirmataWiFi/StandardFirmataWiFi.ino
}

@test "Compile libraries/Firmata/test/firmata_test/firmata_test.ino." {
	skip; compile_example libraries/Firmata/test/firmata_test/firmata_test.ino
}

@test "Compile libraries/GSM/examples/GsmWebClient/GsmWebClient.ino." {
	skip; compile_example libraries/GSM/examples/GsmWebClient/GsmWebClient.ino
}

@test "Compile libraries/GSM/examples/GsmWebServer/GsmWebServer.ino." {
	skip; compile_example libraries/GSM/examples/GsmWebServer/GsmWebServer.ino
}

@test "Compile libraries/GSM/examples/MakeVoiceCall/MakeVoiceCall.ino." {
	skip; compile_example libraries/GSM/examples/MakeVoiceCall/MakeVoiceCall.ino
}

@test "Compile libraries/GSM/examples/ReceiveSMS/ReceiveSMS.ino." {
	skip; compile_example libraries/GSM/examples/ReceiveSMS/ReceiveSMS.ino
}

@test "Compile libraries/GSM/examples/ReceiveVoiceCall/ReceiveVoiceCall.ino." {
	skip; compile_example libraries/GSM/examples/ReceiveVoiceCall/ReceiveVoiceCall.ino
}

@test "Compile libraries/GSM/examples/SendSMS/SendSMS.ino." {
	skip; compile_example libraries/GSM/examples/SendSMS/SendSMS.ino
}

@test "Compile libraries/GSM/examples/Tools/BandManagement/BandManagement.ino." {
	skip; compile_example libraries/GSM/examples/Tools/BandManagement/BandManagement.ino
}

@test "Compile libraries/GSM/examples/Tools/GsmScanNetworks/GsmScanNetworks.ino." {
	skip; compile_example libraries/GSM/examples/Tools/GsmScanNetworks/GsmScanNetworks.ino
}

@test "Compile libraries/GSM/examples/Tools/PinManagement/PinManagement.ino." {
	skip; compile_example libraries/GSM/examples/Tools/PinManagement/PinManagement.ino
}

@test "Compile libraries/GSM/examples/Tools/TestGPRS/TestGPRS.ino." {
	skip; compile_example libraries/GSM/examples/Tools/TestGPRS/TestGPRS.ino
}

@test "Compile libraries/GSM/examples/Tools/TestModem/TestModem.ino." {
	skip; compile_example libraries/GSM/examples/Tools/TestModem/TestModem.ino
}

@test "Compile libraries/GSM/examples/Tools/TestWebServer/TestWebServer.ino." {
	skip; compile_example libraries/GSM/examples/Tools/TestWebServer/TestWebServer.ino
}

@test "Compile libraries/LiquidCrystal/examples/Autoscroll/Autoscroll.ino." {
	compile_example libraries/LiquidCrystal/examples/Autoscroll/Autoscroll.ino
}

@test "Compile libraries/LiquidCrystal/examples/Blink/Blink.ino." {
	compile_example libraries/LiquidCrystal/examples/Blink/Blink.ino
}

@test "Compile libraries/LiquidCrystal/examples/Cursor/Cursor.ino." {
	compile_example libraries/LiquidCrystal/examples/Cursor/Cursor.ino
}

@test "Compile libraries/LiquidCrystal/examples/CustomCharacter/CustomCharacter.ino." {
	compile_example libraries/LiquidCrystal/examples/CustomCharacter/CustomCharacter.ino
}

@test "Compile libraries/LiquidCrystal/examples/Display/Display.ino." {
	compile_example libraries/LiquidCrystal/examples/Display/Display.ino
}

@test "Compile libraries/LiquidCrystal/examples/HelloWorld/HelloWorld.ino." {
	compile_example libraries/LiquidCrystal/examples/HelloWorld/HelloWorld.ino
}

@test "Compile libraries/LiquidCrystal/examples/Scroll/Scroll.ino." {
	compile_example libraries/LiquidCrystal/examples/Scroll/Scroll.ino
}

@test "Compile libraries/LiquidCrystal/examples/SerialDisplay/SerialDisplay.ino." {
	compile_example libraries/LiquidCrystal/examples/SerialDisplay/SerialDisplay.ino
}

@test "Compile libraries/LiquidCrystal/examples/TextDirection/TextDirection.ino." {
	compile_example libraries/LiquidCrystal/examples/TextDirection/TextDirection.ino
}

@test "Compile libraries/LiquidCrystal/examples/setCursor/setCursor.ino." {
	compile_example libraries/LiquidCrystal/examples/setCursor/setCursor.ino
}

@test "Compile libraries/Robot_Control/examples/explore/R01_Logo/R01_Logo.ino." {
	skip; compile_example libraries/Robot_Control/examples/explore/R01_Logo/R01_Logo.ino
}

@test "Compile libraries/Robot_Control/examples/explore/R02_Line_Follow/R02_Line_Follow.ino." {
	skip; compile_example libraries/Robot_Control/examples/explore/R02_Line_Follow/R02_Line_Follow.ino
}

@test "Compile libraries/Robot_Control/examples/explore/R03_Disco_Bot/R03_Disco_Bot.ino." {
	skip; compile_example libraries/Robot_Control/examples/explore/R03_Disco_Bot/R03_Disco_Bot.ino
}

@test "Compile libraries/Robot_Control/examples/explore/R04_Compass/R04_Compass.ino." {
	skip; compile_example libraries/Robot_Control/examples/explore/R04_Compass/R04_Compass.ino
}

@test "Compile libraries/Robot_Control/examples/explore/R05_Inputs/R05_Inputs.ino." {
	skip; compile_example libraries/Robot_Control/examples/explore/R05_Inputs/R05_Inputs.ino
}

@test "Compile libraries/Robot_Control/examples/explore/R06_Wheel_Calibration/R06_Wheel_Calibration.ino." {
	skip; compile_example libraries/Robot_Control/examples/explore/R06_Wheel_Calibration/R06_Wheel_Calibration.ino
}

@test "Compile libraries/Robot_Control/examples/explore/R07_Runaway_Robot/R07_Runaway_Robot.ino." {
	skip; compile_example libraries/Robot_Control/examples/explore/R07_Runaway_Robot/R07_Runaway_Robot.ino
}

@test "Compile libraries/Robot_Control/examples/explore/R08_Remote_Control/R08_Remote_Control.ino." {
	skip; compile_example libraries/Robot_Control/examples/explore/R08_Remote_Control/R08_Remote_Control.ino
}

@test "Compile libraries/Robot_Control/examples/explore/R09_Picture_Browser/R09_Picture_Browser.ino." {
	skip; compile_example libraries/Robot_Control/examples/explore/R09_Picture_Browser/R09_Picture_Browser.ino
}

@test "Compile libraries/Robot_Control/examples/explore/R10_Rescue/R10_Rescue.ino." {
	skip; compile_example libraries/Robot_Control/examples/explore/R10_Rescue/R10_Rescue.ino
}

@test "Compile libraries/Robot_Control/examples/explore/R11_Hello_User/R11_Hello_User.ino." {
	skip; compile_example libraries/Robot_Control/examples/explore/R11_Hello_User/R11_Hello_User.ino
}

@test "Compile libraries/Robot_Control/examples/learn/AllIOPorts/AllIOPorts.ino." {
	skip; compile_example libraries/Robot_Control/examples/learn/AllIOPorts/AllIOPorts.ino
}

@test "Compile libraries/Robot_Control/examples/learn/Beep/Beep.ino." {
	skip; compile_example libraries/Robot_Control/examples/learn/Beep/Beep.ino
}

@test "Compile libraries/Robot_Control/examples/learn/CleanEEPROM/CleanEEPROM.ino." {
	skip; compile_example libraries/Robot_Control/examples/learn/CleanEEPROM/CleanEEPROM.ino
}

@test "Compile libraries/Robot_Control/examples/learn/Compass/Compass.ino." {
	skip; compile_example libraries/Robot_Control/examples/learn/Compass/Compass.ino
}

@test "Compile libraries/Robot_Control/examples/learn/IRArray/IRArray.ino." {
	skip; compile_example libraries/Robot_Control/examples/learn/IRArray/IRArray.ino
}

@test "Compile libraries/Robot_Control/examples/learn/LCDDebugPrint/LCDDebugPrint.ino." {
	skip; compile_example libraries/Robot_Control/examples/learn/LCDDebugPrint/LCDDebugPrint.ino
}

@test "Compile libraries/Robot_Control/examples/learn/LCDPrint/LCDPrint.ino." {
	skip; compile_example libraries/Robot_Control/examples/learn/LCDPrint/LCDPrint.ino
}

@test "Compile libraries/Robot_Control/examples/learn/LCDWriteText/LCDWriteText.ino." {
	skip; compile_example libraries/Robot_Control/examples/learn/LCDWriteText/LCDWriteText.ino
}

@test "Compile libraries/Robot_Control/examples/learn/LineFollowWithPause/LineFollowWithPause.ino." {
	skip; compile_example libraries/Robot_Control/examples/learn/LineFollowWithPause/LineFollowWithPause.ino
}

@test "Compile libraries/Robot_Control/examples/learn/Melody/Melody.ino." {
	skip; compile_example libraries/Robot_Control/examples/learn/Melody/Melody.ino
}

@test "Compile libraries/Robot_Control/examples/learn/MotorTest/MotorTest.ino." {
	skip; compile_example libraries/Robot_Control/examples/learn/MotorTest/MotorTest.ino
}

@test "Compile libraries/Robot_Control/examples/learn/SpeedByPotentiometer/SpeedByPotentiometer.ino." {
	skip; compile_example libraries/Robot_Control/examples/learn/SpeedByPotentiometer/SpeedByPotentiometer.ino
}

@test "Compile libraries/Robot_Control/examples/learn/TurnTest/TurnTest.ino." {
	skip; compile_example libraries/Robot_Control/examples/learn/TurnTest/TurnTest.ino
}

@test "Compile libraries/Robot_Control/examples/learn/keyboardTest/keyboardTest.ino." {
	skip; compile_example libraries/Robot_Control/examples/learn/keyboardTest/keyboardTest.ino
}

@test "Compile libraries/Robot_Motor/examples/Robot_IR_Array_Test/Robot_IR_Array_Test.ino." {
	skip; compile_example libraries/Robot_Motor/examples/Robot_IR_Array_Test/Robot_IR_Array_Test.ino
}

@test "Compile libraries/Robot_Motor/examples/Robot_Motor_Core/Robot_Motor_Core.ino." {
	skip; compile_example libraries/Robot_Motor/examples/Robot_Motor_Core/Robot_Motor_Core.ino
}

@test "Compile libraries/SD/examples/CardInfo/CardInfo.ino." {
	skip; compile_example libraries/SD/examples/CardInfo/CardInfo.ino
}

@test "Compile libraries/SD/examples/Datalogger/Datalogger.ino." {
	skip; compile_example libraries/SD/examples/Datalogger/Datalogger.ino
}

@test "Compile libraries/SD/examples/DumpFile/DumpFile.ino." {
	skip; compile_example libraries/SD/examples/DumpFile/DumpFile.ino
}

@test "Compile libraries/SD/examples/Files/Files.ino." {
	skip; compile_example libraries/SD/examples/Files/Files.ino
}

@test "Compile libraries/SD/examples/ReadWrite/ReadWrite.ino." {
	skip; compile_example libraries/SD/examples/ReadWrite/ReadWrite.ino
}

@test "Compile libraries/Bridge/examples/HttpClient/HttpClient.ino." {
	skip; compile_example libraries/Bridge/examples/HttpClient/HttpClient.ino
}

@test "Compile libraries/SD/examples/listfiles/listfiles.ino." {
	skip; compile_example libraries/SD/examples/listfiles/listfiles.ino
}

@test "Compile libraries/Servo/examples/Knob/Knob.ino." {
	skip; compile_example libraries/Servo/examples/Knob/Knob.ino
}

@test "Compile libraries/Servo/examples/Sweep/Sweep.ino." {
	skip; compile_example libraries/Servo/examples/Sweep/Sweep.ino
}

@test "Compile libraries/SpacebrewYun/examples/inputOutput/inputOutput.ino." {
	skip; compile_example libraries/SpacebrewYun/examples/inputOutput/inputOutput.ino
}

@test "Compile libraries/SpacebrewYun/examples/spacebrewBoolean/spacebrewBoolean.ino." {
	skip; compile_example libraries/SpacebrewYun/examples/spacebrewBoolean/spacebrewBoolean.ino
}

@test "Compile libraries/SpacebrewYun/examples/spacebrewRange/spacebrewRange.ino." {
	skip; compile_example libraries/SpacebrewYun/examples/spacebrewRange/spacebrewRange.ino
}

@test "Compile libraries/SpacebrewYun/examples/spacebrewString/spacebrewString.ino." {
	skip; compile_example libraries/SpacebrewYun/examples/spacebrewString/spacebrewString.ino
}

@test "Compile libraries/Stepper/examples/MotorKnob/MotorKnob.ino." {
	compile_example libraries/Stepper/examples/MotorKnob/MotorKnob.ino
}

@test "Compile libraries/Stepper/examples/stepper_oneRevolution/stepper_oneRevolution.ino." {
	compile_example libraries/Stepper/examples/stepper_oneRevolution/stepper_oneRevolution.ino
}

@test "Compile libraries/Stepper/examples/stepper_oneStepAtATime/stepper_oneStepAtATime.ino." {
	compile_example libraries/Stepper/examples/stepper_oneStepAtATime/stepper_oneStepAtATime.ino
}

@test "Compile libraries/Stepper/examples/stepper_speedControl/stepper_speedControl.ino." {
	compile_example libraries/Stepper/examples/stepper_speedControl/stepper_speedControl.ino
}

@test "Compile libraries/TFT/examples/Arduino/TFTBitmapLogo/TFTBitmapLogo.ino." {
	skip; compile_example libraries/TFT/examples/Arduino/TFTBitmapLogo/TFTBitmapLogo.ino
}

@test "Compile libraries/TFT/examples/Arduino/TFTColorPicker/TFTColorPicker.ino." {
	skip; compile_example libraries/TFT/examples/Arduino/TFTColorPicker/TFTColorPicker.ino
}

@test "Compile libraries/TFT/examples/Arduino/TFTDisplayText/TFTDisplayText.ino." {
	skip; compile_example libraries/TFT/examples/Arduino/TFTDisplayText/TFTDisplayText.ino
}

@test "Compile libraries/TFT/examples/Arduino/TFTEtchASketch/TFTEtchASketch.ino." {
	skip; compile_example libraries/TFT/examples/Arduino/TFTEtchASketch/TFTEtchASketch.ino
}

@test "Compile libraries/TFT/examples/Arduino/TFTGraph/TFTGraph.ino." {
	skip; compile_example libraries/TFT/examples/Arduino/TFTGraph/TFTGraph.ino
}

@test "Compile libraries/TFT/examples/Arduino/TFTPong/TFTPong.ino." {
	skip; compile_example libraries/TFT/examples/Arduino/TFTPong/TFTPong.ino
}

@test "Compile libraries/TFT/examples/Esplora/EsploraTFTBitmapLogo/EsploraTFTBitmapLogo.ino." {
	skip; compile_example libraries/TFT/examples/Esplora/EsploraTFTBitmapLogo/EsploraTFTBitmapLogo.ino
}

@test "Compile libraries/TFT/examples/Esplora/EsploraTFTColorPicker/EsploraTFTColorPicker.ino." {
	skip; compile_example libraries/TFT/examples/Esplora/EsploraTFTColorPicker/EsploraTFTColorPicker.ino
}

@test "Compile libraries/TFT/examples/Esplora/EsploraTFTEtchASketch/EsploraTFTEtchASketch.ino." {
	skip; compile_example libraries/TFT/examples/Esplora/EsploraTFTEtchASketch/EsploraTFTEtchASketch.ino
}

@test "Compile libraries/TFT/examples/Esplora/EsploraTFTGraph/EsploraTFTGraph.ino." {
	skip; compile_example libraries/TFT/examples/Esplora/EsploraTFTGraph/EsploraTFTGraph.ino
}

@test "Compile libraries/TFT/examples/Esplora/EsploraTFTHorizon/EsploraTFTHorizon.ino." {
	skip; compile_example libraries/TFT/examples/Esplora/EsploraTFTHorizon/EsploraTFTHorizon.ino
}

@test "Compile libraries/TFT/examples/Esplora/EsploraTFTPong/EsploraTFTPong.ino." {
	skip; compile_example libraries/TFT/examples/Esplora/EsploraTFTPong/EsploraTFTPong.ino
}

@test "Compile libraries/TFT/examples/Esplora/EsploraTFTTemp/EsploraTFTTemp.ino." {
	skip; compile_example libraries/TFT/examples/Esplora/EsploraTFTTemp/EsploraTFTTemp.ino
}

@test "Compile libraries/Temboo/examples/ArduinoYun/GetYahooWeatherReport/GetYahooWeatherReport.ino." {
	skip; compile_example libraries/Temboo/examples/ArduinoYun/GetYahooWeatherReport/GetYahooWeatherReport.ino
}

@test "Compile libraries/Temboo/examples/ArduinoYun/ReadATweet/ReadATweet.ino." {
	skip; compile_example libraries/Temboo/examples/ArduinoYun/ReadATweet/ReadATweet.ino
}

@test "Compile libraries/Temboo/examples/ArduinoYun/SendATweet/SendATweet.ino." {
	skip; compile_example libraries/Temboo/examples/ArduinoYun/SendATweet/SendATweet.ino
}

@test "Compile libraries/Temboo/examples/ArduinoYun/SendAnEmail/SendAnEmail.ino." {
	skip; compile_example libraries/Temboo/examples/ArduinoYun/SendAnEmail/SendAnEmail.ino
}

@test "Compile libraries/Temboo/examples/ArduinoYun/SendAnSMS/SendAnSMS.ino." {
	skip; compile_example libraries/Temboo/examples/ArduinoYun/SendAnSMS/SendAnSMS.ino
}

@test "Compile libraries/Temboo/examples/ArduinoYun/SendDataToGoogleSpreadsheet/SendDataToGoogleSpreadsheet.ino." {
	skip; compile_example libraries/Temboo/examples/ArduinoYun/SendDataToGoogleSpreadsheet/SendDataToGoogleSpreadsheet.ino
}

@test "Compile libraries/Temboo/examples/ArduinoYun/TembooDeviceConfig/TembooDeviceConfig.ino." {
	skip; compile_example libraries/Temboo/examples/ArduinoYun/TembooDeviceConfig/TembooDeviceConfig.ino
}

@test "Compile libraries/Temboo/examples/ArduinoYun/ToxicFacilitiesSearch/ToxicFacilitiesSearch.ino." {
	skip; compile_example libraries/Temboo/examples/ArduinoYun/ToxicFacilitiesSearch/ToxicFacilitiesSearch.ino
}

@test "Compile libraries/Temboo/examples/ArduinoYun/UpdateFacebookStatus/UpdateFacebookStatus.ino." {
	skip; compile_example libraries/Temboo/examples/ArduinoYun/UpdateFacebookStatus/UpdateFacebookStatus.ino
}

@test "Compile libraries/Temboo/examples/ArduinoYun/UploadToDropbox/UploadToDropbox.ino." {
	skip; compile_example libraries/Temboo/examples/ArduinoYun/UploadToDropbox/UploadToDropbox.ino
}

@test "Compile libraries/Temboo/examples/YunShield/TembooDeviceConfig/TembooDeviceConfig.ino." {
	skip; compile_example libraries/Temboo/examples/YunShield/TembooDeviceConfig/TembooDeviceConfig.ino
}

@test "Compile libraries/WiFi/examples/ConnectNoEncryption/ConnectNoEncryption.ino." {
	skip; compile_example libraries/WiFi/examples/ConnectNoEncryption/ConnectNoEncryption.ino
}

@test "Compile libraries/WiFi/examples/ConnectWithWEP/ConnectWithWEP.ino." {
	skip; compile_example libraries/WiFi/examples/ConnectWithWEP/ConnectWithWEP.ino
}

@test "Compile libraries/WiFi/examples/ConnectWithWPA/ConnectWithWPA.ino." {
	skip; compile_example libraries/WiFi/examples/ConnectWithWPA/ConnectWithWPA.ino
}

@test "Compile libraries/WiFi/examples/ScanNetworks/ScanNetworks.ino." {
	skip; compile_example libraries/WiFi/examples/ScanNetworks/ScanNetworks.ino
}

@test "Compile libraries/WiFi/examples/SimpleWebServerWiFi/SimpleWebServerWiFi.ino." {
	skip; compile_example libraries/WiFi/examples/SimpleWebServerWiFi/SimpleWebServerWiFi.ino
}

@test "Compile libraries/WiFi/examples/WiFiChatServer/WiFiChatServer.ino." {
	skip; compile_example libraries/WiFi/examples/WiFiChatServer/WiFiChatServer.ino
}

@test "Compile libraries/WiFi/examples/WiFiUdpNtpClient/WiFiUdpNtpClient.ino." {
	skip; compile_example libraries/WiFi/examples/WiFiUdpNtpClient/WiFiUdpNtpClient.ino
}

@test "Compile libraries/WiFi/examples/WiFiUdpSendReceiveString/WiFiUdpSendReceiveString.ino." {
	skip; compile_example libraries/WiFi/examples/WiFiUdpSendReceiveString/WiFiUdpSendReceiveString.ino
}

@test "Compile libraries/WiFi/examples/WiFiWebClient/WiFiWebClient.ino." {
	skip; compile_example libraries/WiFi/examples/WiFiWebClient/WiFiWebClient.ino
}

@test "Compile libraries/WiFi/examples/WiFiWebClientRepeating/WiFiWebClientRepeating.ino." {
	skip; compile_example libraries/WiFi/examples/WiFiWebClientRepeating/WiFiWebClientRepeating.ino
}

@test "Compile libraries/WiFi/examples/WiFiWebServer/WiFiWebServer.ino." {
	skip; compile_example libraries/WiFi/examples/WiFiWebServer/WiFiWebServer.ino
}

