#ifndef RINGBUFF_H
#define RINGBUFF_H


static int start = 0;
static int end = 0;
#define RBUFF_LOOP(i) for(i=start;;)
#define RBUFF_LOOP_DESC(i) for(i=end;;)

//must be put at the end of the loop block
#define RBUFF_NEXT(i) {i=(i+1)%RBUFF_SIZE; if (i==start) break;}
#define RBUFF_PREV(i) {i=(i-1)%RBUFF_SIZE; if (i==end) break;}
#define RBUFF_PUSH(val) {rbuff[start] = val; end = start; start=(start+1)%RBUFF_SIZE; }
#define RBUFF_NEWEST(off) rbuff[(start-off-1)%RBUFF_SIZE]
#define RBUFF_OLDEST(off) rbuff[(start+off)%RBUFF_SIZE]

// only loop desc works ok when inserting in parallel




#endif
