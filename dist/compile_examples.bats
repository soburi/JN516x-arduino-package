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
										-tools ~/contiki-makehelper \
										-tools ${ADIR}/tools-builder/ \
										-tools ${ADIR}/hardware/tools/ \
										-libraries ${ADIR}/libraries/ \
										-fqbn "intiki:intiki:twelite:config=TWELITE_BLUE, trace=OFF, rpl=ON" \
										-build-path=${BUILDDIR} \
										${ADIR}/$1
}


@test "Compile examples/01.Basics/AnalogReadSerial/AnalogReadSerial.ino." {
	compile_example examples/01.Basics/AnalogReadSerial/AnalogReadSerial.ino
}
@test "Compile examples/01.Basics/BareMinimum/BareMinimum.ino." {
	compile_example examples/01.Basics/BareMinimum/BareMinimum.ino
}

@test "Compile examples/01.Basics/Blink/Blink.ino." {
	compile_example examples/01.Basics/Blink/Blink.ino
}

@test "Compile examples/01.Basics/DigitalReadSerial/DigitalReadSerial.ino." {
	compile_example examples/01.Basics/DigitalReadSerial/DigitalReadSerial.ino
}

@test "Compile examples/01.Basics/Fade/Fade.ino." {
	compile_example examples/01.Basics/Fade/Fade.ino
}

@test "Compile examples/01.Basics/ReadAnalogVoltage/ReadAnalogVoltage.ino." {
	compile_example examples/01.Basics/ReadAnalogVoltage/ReadAnalogVoltage.ino
}

@test "Compile examples/02.Digital/BlinkWithoutDelay/BlinkWithoutDelay.ino." {
	compile_example examples/02.Digital/BlinkWithoutDelay/BlinkWithoutDelay.ino
}

@test "Compile examples/02.Digital/Button/Button.ino." {
	compile_example examples/02.Digital/Button/Button.ino
}

@test "Compile examples/02.Digital/Debounce/Debounce.ino." {
	compile_example examples/02.Digital/Debounce/Debounce.ino
}

@test "Compile examples/02.Digital/DigitalInputPullup/DigitalInputPullup.ino." {
	compile_example examples/02.Digital/DigitalInputPullup/DigitalInputPullup.ino
}

@test "Compile examples/02.Digital/StateChangeDetection/StateChangeDetection.ino." {
	compile_example examples/02.Digital/StateChangeDetection/StateChangeDetection.ino
}

@test "Compile examples/02.Digital/toneKeyboard/toneKeyboard.ino." {
	compile_example examples/02.Digital/toneKeyboard/toneKeyboard.ino
}

@test "Compile examples/02.Digital/toneMelody/toneMelody.ino." {
	compile_example examples/02.Digital/toneMelody/toneMelody.ino
}

@test "Compile examples/02.Digital/toneMultiple/toneMultiple.ino." {
	compile_example examples/02.Digital/toneMultiple/toneMultiple.ino
}

@test "Compile examples/02.Digital/tonePitchFollower/tonePitchFollower.ino." {
	compile_example examples/02.Digital/tonePitchFollower/tonePitchFollower.ino
}

@test "Compile examples/03.Analog/AnalogInOutSerial/AnalogInOutSerial.ino." {
	compile_example examples/03.Analog/AnalogInOutSerial/AnalogInOutSerial.ino
}

@test "Compile examples/03.Analog/AnalogInput/AnalogInput.ino." {
	compile_example examples/03.Analog/AnalogInput/AnalogInput.ino
}

@test "Compile examples/03.Analog/AnalogWriteMega/AnalogWriteMega.ino." {
	compile_example examples/03.Analog/AnalogWriteMega/AnalogWriteMega.ino
}

@test "Compile examples/03.Analog/Calibration/Calibration.ino." {
	compile_example examples/03.Analog/Calibration/Calibration.ino
}

@test "Compile examples/03.Analog/Fading/Fading.ino." {
	compile_example examples/03.Analog/Fading/Fading.ino
}

@test "Compile examples/03.Analog/Smoothing/Smoothing.ino." {
	compile_example examples/03.Analog/Smoothing/Smoothing.ino
}

@test "Compile examples/04.Communication/ASCIITable/ASCIITable.ino." {
	compile_example examples/04.Communication/ASCIITable/ASCIITable.ino
}

@test "Compile examples/04.Communication/Dimmer/Dimmer.ino." {
	compile_example examples/04.Communication/Dimmer/Dimmer.ino
}

@test "Compile examples/04.Communication/Graph/Graph.ino." {
	compile_example examples/04.Communication/Graph/Graph.ino
}

@test "Compile examples/04.Communication/Midi/Midi.ino." {
	compile_example examples/04.Communication/Midi/Midi.ino
}

@test "Compile examples/04.Communication/MultiSerial/MultiSerial.ino." {
	compile_example examples/04.Communication/MultiSerial/MultiSerial.ino
}

@test "Compile examples/04.Communication/PhysicalPixel/PhysicalPixel.ino." {
	compile_example examples/04.Communication/PhysicalPixel/PhysicalPixel.ino
}

@test "Compile examples/04.Communication/ReadASCIIString/ReadASCIIString.ino." {
	compile_example examples/04.Communication/ReadASCIIString/ReadASCIIString.ino
}

@test "Compile examples/04.Communication/SerialCallResponse/SerialCallResponse.ino." {
	compile_example examples/04.Communication/SerialCallResponse/SerialCallResponse.ino
}

@test "Compile examples/04.Communication/SerialCallResponseASCII/SerialCallResponseASCII.ino." {
	compile_example examples/04.Communication/SerialCallResponseASCII/SerialCallResponseASCII.ino
}

@test "Compile examples/04.Communication/SerialEvent/SerialEvent.ino." {
	compile_example examples/04.Communication/SerialEvent/SerialEvent.ino
}

@test "Compile examples/04.Communication/SerialPassthrough/SerialPassthrough.ino." {
	compile_example examples/04.Communication/SerialPassthrough/SerialPassthrough.ino
}

@test "Compile examples/04.Communication/VirtualColorMixer/VirtualColorMixer.ino." {
	compile_example examples/04.Communication/VirtualColorMixer/VirtualColorMixer.ino
}

@test "Compile examples/05.Control/Arrays/Arrays.ino." {
	compile_example examples/05.Control/Arrays/Arrays.ino
}

@test "Compile examples/05.Control/ForLoopIteration/ForLoopIteration.ino." {
	compile_example examples/05.Control/ForLoopIteration/ForLoopIteration.ino
}

@test "Compile examples/05.Control/IfStatementConditional/IfStatementConditional.ino." {
	compile_example examples/05.Control/IfStatementConditional/IfStatementConditional.ino
}

@test "Compile examples/05.Control/WhileStatementConditional/WhileStatementConditional.ino." {
	compile_example examples/05.Control/WhileStatementConditional/WhileStatementConditional.ino
}

@test "Compile examples/05.Control/switchCase/switchCase.ino." {
	compile_example examples/05.Control/switchCase/switchCase.ino
}

@test "Compile examples/05.Control/switchCase2/switchCase2.ino." {
	compile_example examples/05.Control/switchCase2/switchCase2.ino
}

@test "Compile examples/06.Sensors/ADXL3xx/ADXL3xx.ino." {
	compile_example examples/06.Sensors/ADXL3xx/ADXL3xx.ino
}

@test "Compile examples/06.Sensors/Knock/Knock.ino." {
	compile_example examples/06.Sensors/Knock/Knock.ino
}

@test "Compile examples/06.Sensors/Memsic2125/Memsic2125.ino." {
	compile_example examples/06.Sensors/Memsic2125/Memsic2125.ino
}

@test "Compile examples/06.Sensors/Ping/Ping.ino." {
	compile_example examples/06.Sensors/Ping/Ping.ino
}

@test "Compile examples/07.Display/RowColumnScanning/RowColumnScanning.ino." {
	compile_example examples/07.Display/RowColumnScanning/RowColumnScanning.ino
}

@test "Compile examples/07.Display/barGraph/barGraph.ino." {
	compile_example examples/07.Display/barGraph/barGraph.ino
}

@test "Compile examples/08.Strings/CharacterAnalysis/CharacterAnalysis.ino." {
	compile_example examples/08.Strings/CharacterAnalysis/CharacterAnalysis.ino
}

@test "Compile examples/08.Strings/StringAdditionOperator/StringAdditionOperator.ino." {
	compile_example examples/08.Strings/StringAdditionOperator/StringAdditionOperator.ino
}

@test "Compile examples/08.Strings/StringAppendOperator/StringAppendOperator.ino." {
	compile_example examples/08.Strings/StringAppendOperator/StringAppendOperator.ino
}

@test "Compile examples/08.Strings/StringCaseChanges/StringCaseChanges.ino." {
	compile_example examples/08.Strings/StringCaseChanges/StringCaseChanges.ino
}

@test "Compile examples/08.Strings/StringCharacters/StringCharacters.ino." {
	compile_example examples/08.Strings/StringCharacters/StringCharacters.ino
}

@test "Compile examples/08.Strings/StringComparisonOperators/StringComparisonOperators.ino." {
	skip
	compile_example examples/08.Strings/StringComparisonOperators/StringComparisonOperators.ino
}

@test "Compile examples/08.Strings/StringConstructors/StringConstructors.ino." {
	compile_example examples/08.Strings/StringConstructors/StringConstructors.ino
}

@test "Compile examples/08.Strings/StringIndexOf/StringIndexOf.ino." {
	compile_example examples/08.Strings/StringIndexOf/StringIndexOf.ino
}

@test "Compile examples/08.Strings/StringLength/StringLength.ino." {
	compile_example examples/08.Strings/StringLength/StringLength.ino
}

@test "Compile examples/08.Strings/StringLengthTrim/StringLengthTrim.ino." {
	compile_example examples/08.Strings/StringLengthTrim/StringLengthTrim.ino
}

@test "Compile examples/08.Strings/StringReplace/StringReplace.ino." {
	compile_example examples/08.Strings/StringReplace/StringReplace.ino
}

@test "Compile examples/08.Strings/StringStartsWithEndsWith/StringStartsWithEndsWith.ino." {
	compile_example examples/08.Strings/StringStartsWithEndsWith/StringStartsWithEndsWith.ino
}

@test "Compile examples/08.Strings/StringSubstring/StringSubstring.ino." {
	compile_example examples/08.Strings/StringSubstring/StringSubstring.ino
}

@test "Compile examples/08.Strings/StringToInt/StringToInt.ino." {
	compile_example examples/08.Strings/StringToInt/StringToInt.ino
}

@test "Compile examples/09.USB/Keyboard/KeyboardLogout/KeyboardLogout.ino." {
	skip
	compile_example examples/09.USB/Keyboard/KeyboardLogout/KeyboardLogout.ino
}

@test "Compile examples/09.USB/Keyboard/KeyboardMessage/KeyboardMessage.ino." {
	skip
	compile_example examples/09.USB/Keyboard/KeyboardMessage/KeyboardMessage.ino
}

@test "Compile examples/09.USB/Keyboard/KeyboardReprogram/KeyboardReprogram.ino." {
	skip
	compile_example examples/09.USB/Keyboard/KeyboardReprogram/KeyboardReprogram.ino
}

@test "Compile examples/09.USB/Keyboard/KeyboardSerial/KeyboardSerial.ino." {
	skip
	compile_example examples/09.USB/Keyboard/KeyboardSerial/KeyboardSerial.ino
}

@test "Compile examples/09.USB/KeyboardAndMouseControl/KeyboardAndMouseControl.ino." {
	skip
	compile_example examples/09.USB/KeyboardAndMouseControl/KeyboardAndMouseControl.ino
}

@test "Compile examples/09.USB/Mouse/ButtonMouseControl/ButtonMouseControl.ino." {
	skip
	compile_example examples/09.USB/Mouse/ButtonMouseControl/ButtonMouseControl.ino
}

@test "Compile examples/09.USB/Mouse/JoystickMouseControl/JoystickMouseControl.ino." {
	skip
	compile_example examples/09.USB/Mouse/JoystickMouseControl/JoystickMouseControl.ino
}

@test "Compile examples/10.StarterKit_BasicKit/p02_SpaceshipInterface/p02_SpaceshipInterface.ino." {
	compile_example examples/10.StarterKit_BasicKit/p02_SpaceshipInterface/p02_SpaceshipInterface.ino
}

@test "Compile examples/10.StarterKit_BasicKit/p03_LoveOMeter/p03_LoveOMeter.ino." {
	compile_example examples/10.StarterKit_BasicKit/p03_LoveOMeter/p03_LoveOMeter.ino
}

@test "Compile examples/10.StarterKit_BasicKit/p04_ColorMixingLamp/p04_ColorMixingLamp.ino." {
	compile_example examples/10.StarterKit_BasicKit/p04_ColorMixingLamp/p04_ColorMixingLamp.ino
}

@test "Compile examples/10.StarterKit_BasicKit/p05_ServoMoodIndicator/p05_ServoMoodIndicator.ino." {
	skip
	compile_example examples/10.StarterKit_BasicKit/p05_ServoMoodIndicator/p05_ServoMoodIndicator.ino
}

@test "Compile examples/10.StarterKit_BasicKit/p06_LightTheremin/p06_LightTheremin.ino." {
	skip
	compile_example examples/10.StarterKit_BasicKit/p06_LightTheremin/p06_LightTheremin.ino
}

@test "Compile examples/10.StarterKit_BasicKit/p07_Keyboard/p07_Keyboard.ino." {
	skip
	compile_example examples/10.StarterKit_BasicKit/p07_Keyboard/p07_Keyboard.ino
}

@test "Compile examples/10.StarterKit_BasicKit/p08_DigitalHourglass/p08_DigitalHourglass.ino." {
	compile_example examples/10.StarterKit_BasicKit/p08_DigitalHourglass/p08_DigitalHourglass.ino
}

@test "Compile examples/10.StarterKit_BasicKit/p09_MotorizedPinwheel/p09_MotorizedPinwheel.ino." {
	compile_example examples/10.StarterKit_BasicKit/p09_MotorizedPinwheel/p09_MotorizedPinwheel.ino
}

@test "Compile examples/10.StarterKit_BasicKit/p10_Zoetrope/p10_Zoetrope.ino." {
	compile_example examples/10.StarterKit_BasicKit/p10_Zoetrope/p10_Zoetrope.ino
}

@test "Compile examples/10.StarterKit_BasicKit/p11_CrystalBall/p11_CrystalBall.ino." {
	compile_example examples/10.StarterKit_BasicKit/p11_CrystalBall/p11_CrystalBall.ino
}

@test "Compile examples/10.StarterKit_BasicKit/p12_KnockLock/p12_KnockLock.ino." {
	skip
	compile_example examples/10.StarterKit_BasicKit/p12_KnockLock/p12_KnockLock.ino
}

@test "Compile examples/10.StarterKit_BasicKit/p13_TouchSensorLamp/p13_TouchSensorLamp.ino." {
	skip
	compile_example examples/10.StarterKit_BasicKit/p13_TouchSensorLamp/p13_TouchSensorLamp.ino
}

@test "Compile examples/10.StarterKit_BasicKit/p14_TweakTheArduinoLogo/p14_TweakTheArduinoLogo.ino." {
	compile_example examples/10.StarterKit_BasicKit/p14_TweakTheArduinoLogo/p14_TweakTheArduinoLogo.ino
}

@test "Compile examples/10.StarterKit_BasicKit/p15_HackingButtons/p15_HackingButtons.ino." {
	skip
	compile_example examples/10.StarterKit_BasicKit/p15_HackingButtons/p15_HackingButtons.ino
}

@test "Compile examples/11.ArduinoISP/ArduinoISP/ArduinoISP.ino." {
	compile_example examples/11.ArduinoISP/ArduinoISP/ArduinoISP.ino
}

