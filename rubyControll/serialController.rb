require 'rubygems'
require 'serialport'

@serial_port = "/dev/tty.wchusbserial143320"
@serial_bps = 9600
@serial_data_bit = 8
@serial_stop_bit = 1
@serial_parity = SerialPort::NONE

def write(com)
  @sp.write(com + "\r")
  @sp.flush();
end

def testDrive()
  write("face")
  write("go")
  sleep(3)
  write("go 140")
  sleep(3)
  write("go 255")
  sleep(3)
  write("stop")
end

puts("setting")
@sp = SerialPort.new(
  @serial_port,
  @serial_bps,
  @serial_data_bit,
  @serial_stop_bit,
  @serial_parity
)
sleep(5)
puts("finish setting")

testDrive()
@sp.close()
