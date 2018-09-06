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
                                        -fqbn "intiki:intiki:twelite:config=TWELITE_BLUE, trace=OFF, rpl=ON" \
                                        -build-path=${BUILDDIR} \
                                        ./$1
}

@test "Compile libraries/EEPROM/examples/eeprom_crc/eeprom_crc.ino." {
	compile_example libraries/EEPROM/examples/eeprom_crc/eeprom_crc.ino
}

@test "Compile libraries/EEPROM/examples/eeprom_put/eeprom_put.ino." {
	compile_example libraries/EEPROM/examples/eeprom_put/eeprom_put.ino
}

@test "Compile libraries/EEPROM/examples/eeprom_update/eeprom_update.ino." {
	compile_example libraries/EEPROM/examples/eeprom_update/eeprom_update.ino
}

@test "Compile libraries/EEPROM/examples/eeprom_clear/eeprom_clear.ino." {
	compile_example libraries/EEPROM/examples/eeprom_clear/eeprom_clear.ino
}

@test "Compile libraries/EEPROM/examples/eeprom_read/eeprom_read.ino." {
	compile_example libraries/EEPROM/examples/eeprom_read/eeprom_read.ino
}

@test "Compile libraries/EEPROM/examples/eeprom_iteration/eeprom_iteration.ino." {
	compile_example libraries/EEPROM/examples/eeprom_iteration/eeprom_iteration.ino
}

@test "Compile libraries/EEPROM/examples/eeprom_get/eeprom_get.ino." {
	compile_example libraries/EEPROM/examples/eeprom_get/eeprom_get.ino
}

@test "Compile libraries/EEPROM/examples/eeprom_write/eeprom_write.ino." {
	compile_example libraries/EEPROM/examples/eeprom_write/eeprom_write.ino
}

@test "Compile libraries/uIP/examples/Udp7EchoClientServer/Udp7EchoClientServer.ino." {
	compile_example libraries/uIP/examples/Udp7EchoClientServer/Udp7EchoClientServer.ino
}

@test "Compile libraries/uIP/examples/tcp-echo-server-rpl/tcp-echo-server-rpl.ino." {
	compile_example libraries/uIP/examples/tcp-echo-server-rpl/tcp-echo-server-rpl.ino
}

@test "Compile libraries/uIP/examples/tcp-echo-client/tcp-echo-client.ino." {
	compile_example libraries/uIP/examples/tcp-echo-client/tcp-echo-client.ino
}

@test "Compile libraries/uIP/examples/tiny-httpd/tiny-httpd.ino." {
	compile_example libraries/uIP/examples/tiny-httpd/tiny-httpd.ino
}

@test "Compile libraries/uIP/examples/tcpv6-client-example/tcpv6-client-example.ino." {
	compile_example libraries/uIP/examples/tcpv6-client-example/tcpv6-client-example.ino
}

@test "Compile libraries/uIP/examples/udp-echo-client/udp-echo-client.ino." {
	compile_example libraries/uIP/examples/udp-echo-client/udp-echo-client.ino
}

@test "Compile libraries/uIP/examples/udp-echo-server-rpl/udp-echo-server-rpl.ino." {
	compile_example libraries/uIP/examples/udp-echo-server-rpl/udp-echo-server-rpl.ino
}

@test "Compile libraries/SPI/examples/DigitalPotControl/DigitalPotControl.ino." {
	compile_example libraries/SPI/examples/DigitalPotControl/DigitalPotControl.ino
}

@test "Compile libraries/SPI/examples/BarometricPressureSensor/BarometricPressureSensor.ino." {
	compile_example libraries/SPI/examples/BarometricPressureSensor/BarometricPressureSensor.ino
}

@test "Compile libraries/Wire/examples/master_writer/master_writer.ino." {
	compile_example libraries/Wire/examples/master_writer/master_writer.ino
}

@test "Compile libraries/Wire/examples/slave_sender/slave_sender.ino." {
	compile_example libraries/Wire/examples/slave_sender/slave_sender.ino
}

@test "Compile libraries/Wire/examples/digital_potentiometer/digital_potentiometer.ino." {
	compile_example libraries/Wire/examples/digital_potentiometer/digital_potentiometer.ino
}

@test "Compile libraries/Wire/examples/SFRRanger_reader/SFRRanger_reader.ino." {
	compile_example libraries/Wire/examples/SFRRanger_reader/SFRRanger_reader.ino
}

@test "Compile libraries/Wire/examples/slave_receiver/slave_receiver.ino." {
	compile_example libraries/Wire/examples/slave_receiver/slave_receiver.ino
}

@test "Compile libraries/Wire/examples/master_reader/master_reader.ino." {
	compile_example libraries/Wire/examples/master_reader/master_reader.ino
}

