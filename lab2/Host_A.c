#include "Sim_Engine.h"
#define PACKAGE_SIZE 20
#define A 0
  static struct pkt A_lastpkt;
  static int A_waiting_for_ack = 0; /* 1 if waiting for ACK */
  static int A_seqnum = 0; 
/* Called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message) {
  /*recieves data from layer 5 (output) then
  supposed to fill data into input packet, which
  then is sent to layer 3 and recieved by
  B_input*/
  if(A_waiting_for_ack){
    return;
  }
  struct pkt mypacket;
  mypacket.seqnum = A_seqnum;
  mypacket.acknum = 0;

  int sum = 0;
  for (int i = 0; i < PACKAGE_SIZE; i++) {
    char b = message.data[i];
    mypacket.payload[i] = message.data[i];
    sum += b;
  }

  mypacket.checksum = sum;
  A_lastpkt = mypacket;

  //send
  tolayer3(A, mypacket);
  starttimer(A, 20.0);
  A_waiting_for_ack = 1; 
  /* TODO */
}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet) { 
  
/*check for corrupted. Recalculate the checksum
and compare to the packet.*/
  // int sum = 0;
  // for(int i = 0; i<20; i++){
  //   sum += packet.payload[i];
  // }

  // //if corrupted throw packet
  // if(sum != packet.checksum){
  //   return; 
  // }
  //if right packet
  if (packet.acknum == A_seqnum + 1 && A_waiting_for_ack) {
    stoptimer(A);
    A_waiting_for_ack = 0;
    A_seqnum++;
  }
  /* TODO */ 
}

/* Called when A's timer goes off */
void A_timerinterrupt() {
  /*if still waiting for ACK, retransmit last packet and restart timer */
  if (A_waiting_for_ack) { // if(A_waiting_for_ack == 1)
    tolayer3(A, A_lastpkt);
    starttimer(A, 20.0);
  }
  /* TODO */
 }

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init() {
  //reset
  A_waiting_for_ack = 0;
  A_seqnum = 0;
  /* TODO */ 

}
