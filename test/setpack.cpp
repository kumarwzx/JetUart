#include <libserial/SerialPort.h>

#include <cstring>
#include <chrono>
#include <system_error>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>

using namespace LibSerial;

typedef uint8_t Byte;

void packData( DataBuffer& buf, float dist, float angle, float maxLiVel, float maxAngVel)
{
  static unsigned char checksum =0;
  checksum = 0;
  uint8_t  fltoint[16] = {0};
  std::memcpy(fltoint, &dist, sizeof(dist));
  std::memcpy(fltoint+4, &angle, sizeof(angle));
  std::memcpy(fltoint+8, &maxLiVel, sizeof(maxLiVel));
  std::memcpy(fltoint+12, &maxAngVel, sizeof(maxAngVel));
  for(int j=0; j<16; j++){
      buf[j+4] = fltoint[j];
  }
  for(int i=0; i<20; ++i){
    checksum += buf[i];
  }
  buf[20] = checksum;
}

int main(int argc, char **argv)
{
  int timeGap = 1000000;
  std::string devicePath("/dev/ttyTHS1");
  int tempGap = 0;
  if(argc >3){
    std::cerr << "Error !"<<std::endl<<"args: sudo ./setpack.cpp device path time gap"<<std::endl;
    return -1;
  }
  if(argc == 3){
    std::istringstream inputTimeGapstr(argv[2]);
    devicePath = argv[1];
    inputTimeGapstr >> tempGap;
    if( tempGap > 1 && tempGap <10000000){
      timeGap =  tempGap;
    }else{
      std::cerr<<"Invalid Time Gap !"<<std::endl;
    }
    std::cout<<devicePath.c_str()<<std::endl;
  }
  if(argc == 2){
    std::istringstream inputTimeGapstr(argv[1]);
    inputTimeGapstr >> tempGap;
    if( tempGap > 1 && tempGap <10000000){
      timeGap =  tempGap;
    }else{
      std::cerr<<"Invalid Time Gap !"<<std::endl;
    }
  }
  //using Lib
    // Create and open the serial port for communication.
  SerialPort serial_port;

  try{
  serial_port.Open(devicePath.c_str());
  }catch( const OpenFailed&){
    std::cerr << "The serial port did not open correctly." << std::endl ;
    return EXIT_FAILURE ;
  }

  // Set the baud rate of the serial port.
  serial_port.SetBaudRate(BaudRate::BAUD_115200) ;

  // Set the number of data bits.
  serial_port.SetCharacterSize(CharacterSize::CHAR_SIZE_8) ;

  // Turn off hardware flow control.
  serial_port.SetFlowControl(FlowControl::FLOW_CONTROL_NONE) ;

  // Disable parity.
  serial_port.SetParity(Parity::PARITY_NONE) ;

  // Set the number of stop bits.
  serial_port.SetStopBits(StopBits::STOP_BITS_1) ;

  std::cout << "Writing input file contents to the serial port." << std::endl ;

  // Write an array of data from the serial port.
  const int BUFFER_SIZE = 21;
  DataBuffer buf(BUFFER_SIZE);
  buf[0] = 0x52;
  buf[1] = 0x71;
  buf[2] = 0x01;
  buf[3] = 0x15;

  int count = 0;
  while(1){
    packData(buf, 1.2, 1.2, 1.2, 1.2);
    serial_port.Write(buf);
    serial_port.DrainWriteBuffer();
    std::printf("Successfully %d th package %d \n", count, buf[20]);
    for(int i = 0; i< 21; i++){
      std::cout<<int(buf[i])<<std::endl;
    }
    ++count;
    //std::chrono::milliseconds(1000000);
    usleep(timeGap);
  }

  /* my_serial_port.Write( output_buffer, BUFFER_SIZE ); */
  /* my_serial_stream.write( output_buffer, BUFFER_SIZE ); */

  serial_port.Close();
  /* my_serial_stream.Close(); */
}
