#include "Sim_Engine.h"
static int B_expected_seq = 0;
static struct pkt B_last_ack; //expected seq

#define PACKAGE_SIZE 20
#define B 1
static int calc_sum(struct pkt pack){
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
    tolayer3(B, B_last_ack); //send back ack for last pack. Essentially tells host A to resend the package.
    return;
  }

  //if correct seqnum, deliver to layer5 and create ack package
  if(packet.seqnum == B_expected_seq) {
    tolayer5(B, packet.payload); // deliver to layer5

    //create ack packet and send to host A
    struct pkt newpkt;
    newpkt.seqnum = 0;
    newpkt.acknum = packet.seqnum; //acknowledge that we recieved last package.

    for(int i = 0; i<PACKAGE_SIZE; i++){
      newpkt.payload[i] = 0;
    }

    newpkt.checksum = calc_sum(newpkt);
    B_last_ack = newpkt; //update latest package recieved
    tolayer3(B,newpkt);

    B_expected_seq = 1 - B_expected_seq; // 1 or 0
  }
  else{
    tolayer3(B,B_last_ack); //not correct packet, resending previous ack 
  }
}

void B_timerinterrupt() {
  
}  

void B_init() {

  /*expect first packet to have seq 0 and ack initiliazed to 1.
  If first packet has seq = 0 then previous must be 1 :p*/ 
  B_expected_seq = 0;
  B_last_ack.seqnum = 0;
  B_last_ack.acknum = 1;
  for(int i = 0; i<PACKAGE_SIZE;i++){
    B_last_ack.payload[i] = 0;
  }
  B_last_ack.checksum = calc_sum(B_last_ack);
}
