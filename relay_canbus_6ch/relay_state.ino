
void update_all_state() {

  CAN_TX_msg.len = 7;
  CAN_TX_msg.buf[0] = 0x02;
  CAN_TX_msg.buf[1] = stato1;
  CAN_TX_msg.buf[2] = stato2;
  CAN_TX_msg.buf[3] = stato3;
  CAN_TX_msg.buf[4] = stato4;
  CAN_TX_msg.buf[5] = stato5;
  CAN_TX_msg.buf[6] = stato6;

  Can.write(CAN_TX_msg);
}

void update_state1() {
  ep_update = true;


  CAN_TX_msg.buf[0] = 0x01;
  CAN_TX_msg.buf[1] = 0x01;
  CAN_TX_msg.buf[2] = stato1;
  CAN_TX_msg.buf[3] = 0x00;
  CAN_TX_msg.len = 4;
  Can.write(CAN_TX_msg);
}
void update_state2() {
  ep_update = true;


  CAN_TX_msg.buf[0] = 0x01;
  CAN_TX_msg.buf[1] = 0x02;
  CAN_TX_msg.buf[2] = stato2;
  CAN_TX_msg.buf[3] = 0x00;
  CAN_TX_msg.len = 4;
  Can.write(CAN_TX_msg);
}
void update_state3() {
  ep_update = true;


  CAN_TX_msg.buf[0] = 0x01;
  CAN_TX_msg.buf[1] = 0x03;
  CAN_TX_msg.buf[2] = stato3;
  CAN_TX_msg.buf[3] = 0x00;
  CAN_TX_msg.len = 4;
  Can.write(CAN_TX_msg);
}
void update_state4() {
  ep_update = true;


  CAN_TX_msg.buf[0] = 0x01;
  CAN_TX_msg.buf[1] = 0x04;
  CAN_TX_msg.buf[2] = stato4;
   CAN_TX_msg.buf[3] = 0x00;
  CAN_TX_msg.len = 4;
  Can.write(CAN_TX_msg);
}
void update_state5() {
  ep_update = true;


  CAN_TX_msg.buf[0] = 0x01;
  CAN_TX_msg.buf[1] = 0x05;
  CAN_TX_msg.buf[2] = stato5;
   CAN_TX_msg.buf[3] = 0x00;
  CAN_TX_msg.len = 4;
  Can.write(CAN_TX_msg);
}
void update_state6() {
  ep_update = true;


  CAN_TX_msg.buf[0] = 0x01;
  CAN_TX_msg.buf[1] = 0x06;
  CAN_TX_msg.buf[2] = stato6;
 CAN_TX_msg.buf[3] = 0x00;
  CAN_TX_msg.len = 4;
  Can.write(CAN_TX_msg);
}
