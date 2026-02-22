#include "Sim_Engine.h"
static int B_expected_seq = 0; //exoected seq


/* Called from layer 5, passed the data to be sent to other side */
void B_output( struct msg message) {
  /* DON'T IMPLEMENT */
}

void B_input(struct pkt packet) {
  int B = 1;

  //recalculate checksum for packet
  int sum = 0;
  for (int i = 0; i < 20; i++) {
    sum += packet.payload[i];
  }

  /* if corrupted -> drop packet */
  if (sum != packet.checksum) {
    return;
  }

  //if correct seqnum, send deliver to layer5
  if (packet.seqnum == B_expected_seq) {
    tolayer5(B, packet.payload);
    B_expected_seq++;
  }

  //send ack for the received packet
  struct pkt ackpkt;
  for (int i = 0; i < 20; i++) ackpkt.payload[i] = 0;
  ackpkt.seqnum = 0;
  ackpkt.acknum = packet.seqnum + 1;
  ackpkt.checksum = 0;
  tolayer3(B, ackpkt);
}

void B_timerinterrupt() {
}  

void B_init() {
  B_expected_seq = 0;
}
