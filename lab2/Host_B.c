#include "Sim_Engine.h"
static int B_expected_seq = 0;
static struct pkt B_last_ack; //expected seq

#define PACKAGE_SIZE 20
#define B 1
int calc_sum(struct pkt pack){
  int sum = 0;
  sum += pack.acknum;
  sum += pack.seqnum;
  for (int i = 0; i < PACKAGE_SIZE; i++) {
    sum += (unsigned char)pack.payload[i];
  }
  return sum;
}

/* Called from layer 5, passed the data to be sent to other side */
void B_output( struct msg message) {
  /* DON'T IMPLEMENT */
}

void B_input(struct pkt packet) {
  //recalculate checksum for packet
  //if corrupted 
  if(calc_sum(packet)!= packet.checksum) {
    tolayer3(B, B_last_ack);
    return;
  }

  //if correct seqnum, send deliver to layer5
  if(packet.seqnum == B_expected_seq) {
    tolayer5(B, packet.payload); // deliver to layer5

    //create ack packet and send
    struct pkt newpkt;
    newpkt.seqnum = 0;
    newpkt.acknum = packet.seqnum;

    for(int i = 0; i<20; i++){
      newpkt.payload[i] = 0;
    }

    newpkt.checksum = calc_sum(newpkt);
    B_last_ack = newpkt;
    tolayer3(B,newpkt);

    B_expected_seq = 1 - B_expected_seq;
  }
  else{
    tolayer3(B,B_last_ack); // else send back the packet
  }
}

void B_timerinterrupt() {
  
}  

void B_init() {
  B_expected_seq = 0;
  B_last_ack.seqnum = 0;
  B_last_ack.acknum = 1;
  for(int i = 0; i<20;i++){
    B_last_ack.payload[i] = 0;
  }
  B_last_ack.checksum = calc_sum(B_last_ack);
}
