#include "Sim_Engine.h"
#define PACKAGE_SIZE 20
#define A 0
static struct pkt A_lastpkt;
static int A_waiting_for_ack = 0; //1 if waiting for ACK
static int A_seqnum = 0; 

static int calc_sum(struct pkt pack){
  int sum = 0;
  sum += pack.acknum;
  sum += pack.seqnum;
  for(int i = 0; i < PACKAGE_SIZE; i++) {
    sum += (unsigned char)pack.payload[i];
  }
  return sum;
}


/* Called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message) {
  if(A_waiting_for_ack){
    return;
  }

  struct pkt mypacket;
  mypacket.seqnum = A_seqnum; //seqnum 0 initially.
  mypacket.acknum = 0;

  for(int i = 0; i < PACKAGE_SIZE; i++) {
    mypacket.payload[i] = message.data[i];
  }

  mypacket.checksum = calc_sum(mypacket);
  A_lastpkt = mypacket; //save packet before sending. If retransmition is needed

  //send
  tolayer3(A, mypacket);
  starttimer(A, 20.0);
  A_waiting_for_ack = 1; 
}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet) { 
  //if corrupted throw packet.
  if(calc_sum(packet) != packet.checksum){
    return; 
  }
  //if right packet
  if(packet.acknum == A_seqnum && A_waiting_for_ack) {
    stoptimer(A);
    A_waiting_for_ack = 0;
    A_seqnum = 1 - A_seqnum; // 1 or 0
  }
  //if its not the correct packet essentially wait for timer to run out
}

/* Called when A's timer goes off */
void A_timerinterrupt() {
  //if still waiting for ACK, retransmit last packet and restart timer 
  if(A_waiting_for_ack) {
    tolayer3(A, A_lastpkt);
    starttimer(A, 20.0);
  }
 }

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init() {
  //reset
  A_waiting_for_ack = 0;
  A_seqnum = 0;

}



