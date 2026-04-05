#include "node.h"
#define INF 999

void rtinit(struct distance_table *table, int node) {
    // Tables are already initialized

    //create packet
    struct rtpkt newpkt;
    //this node is source
    newpkt.sourceid = node;

    //send current nodes min cost to each neighbor. Only change destid
    for(int i = 0; i < 4; i++)
    {
        //if node is neighbor
        if(is_neighbor(node, i))
        {
            //send to neighbor i
            newpkt.destid = i;

            //set current min costs into packet
            for(int j = 0; j < 4; j++) //to destination
            {
                newpkt.mincost[j] = table->costs[node][j];
            }
            //send
            tolayer2(newpkt);
        }
    }


    printdt(table, node);
}

void rtupdate(struct distance_table *table, int node, struct rtpkt *pkt) {
    int upd = 0; //not updated
    int sourceNode = pkt->sourceid;

   //save packets min cost to table
   for(int i = 0; i<4; i++)
   {
        table->costs[sourceNode][i] = pkt->mincost[i];
   } 

   //recalculate nodes own mincost
    for(int i = 0; i<4; i++)
    {
        int min = INF;

        if(i == node)
        { //cost to itself
            min = 0;
        }
        else
        {
            for(int neighbor = 0; neighbor < 4; neighbor++)
            {
                //check every neighbor node as the next hop to destination
                if(is_neighbor(node, neighbor))
                {
                    
                    /*if they are neighbors then lets check 
                    the total cost from our node to that neighbor(j)
                    then from that neighbor to our destination (i) 
                    and if its less that min then we found a new path.
                    */
                    int cost = table->costs[node][neighbor] + table->costs[neighbor][i]; 
                    if(cost < min){ //keep the shortest found route
                        min = cost;
                    }
                }
            }
        }
        //if we find a better min cost for dest (i)
        //store it in this node.
        if(min != table->costs[node][i])
        {
            table->costs[node][i] = min;
            upd = 1; 
        }
    }

    //if this nodes mincost has changed, inform neighbors
    if(upd) 
    {
        struct rtpkt newPacket;
        newPacket.sourceid = node;

        for(int i = 0; i < 4; i++)
        {
            newPacket.mincost[i] = table->costs[node][i];
        }
        //send the updated cost to every neighbor
        for(int j = 0; j < 4; j++){ 
            if(is_neighbor(node, j)){ 
                newPacket.destid = j;
                tolayer2(newPacket);
            }
        }
    }

    printdt(table, node);
}