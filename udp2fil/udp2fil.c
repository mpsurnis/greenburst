#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <math.h>
#include "filterbank.h"
#include <hiredis.h>

/* Print Errors */
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

/* Get RA and Dec from Redis */

void getradec(double *ra, double *dec) {

    redisContext *c;
    redisReply *reply;
    const char *hostname = "serendip6";
    int port = 6379;

    float temp, ss, ds;
    //double ra, dec;
    int hh, mm, dd, dm, flag;

    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    c = redisConnectWithTimeout(hostname, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }

    reply = redisCommand(c,"HGET RA_DRV VALUE");
    //printf("The RA in degree is: %s\n", reply->str);

    temp = atof(reply->str);
    //printf("%f\n", temp);

    /*Convert RA from hrs to hms*/
    hh = (int) temp;
    mm = (temp - hh) * 60;
    ss = (temp - hh - (mm/60.0)) * 3600.0;
    *ra = (hh*10000.0) + (mm*100.0) + ss;
    fprintf(stderr,"RA in hms is: %d %d %f\n", hh, mm, ss);
    //printf("src_raj (float) is: %f\n", ra);

    freeReplyObject(reply);

    reply = redisCommand(c,"HGET DEC_DRV VALUE");
    //printf("The DEC in degree is: %s\n", reply->str);

    /*Convert DEC from degree to dms*/
    temp = atof(reply->str);
    //printf("%f\n", temp);

    dd = (int) temp;
    if (dd<0) {flag = -1;}else{flag = 1;}
    dm = (temp - dd) * 60 * flag;
    ds = ((temp - dd) * flag - (dm/60.0)) * 3600.0;
    *dec = ((flag*dd*10000.0) + (dm*100.0) + ds) * flag;
    fprintf(stderr,"DEC in dms is: %d %d %f\n", dd, dm, ds);
    //printf("src_dej (float) is: %f\n", dec);

    freeReplyObject(reply);

    /* Disconnects and frees the context */
    redisFree(c);

    //return NULL;

}
/* Filterbank header writer */

void filterbank_header(FILE *outptr)
{
     FILE *outfile;
     char *string;
     int len;
     outfile = outptr;
     /* Start the header */
     string = "HEADER_START";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     ///fprintf(stderr,"Sent a header start\n");
     /* Source Name */
     //string = "source_name";
     //len = strlen(string);
     //fwrite(&len, sizeof(int), 1, outfile);
     //fwrite(string, sizeof(char), len, outfile);
     //len = strlen(source_name);
     //fwrite(&len, sizeof(int), 1, outfile);
     //fwrite(source_name, sizeof(char), len, outfile);
     //fprintf(stderr,"Sent source_name\n");
     /* Machine and Telescope ID */
     string = "machine_id";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     fwrite(&machine_id, sizeof(int), 1, outfile);
     //fprintf(stderr,"Sent machine_id\n"); 
     string = "telescope_id";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     fwrite(&telescope_id, sizeof(int), 1, outfile);
     //fprintf(stderr,"Sent telescope_id\n");
     string = "src_raj";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     fwrite(&src_raj, sizeof(double), 1, outfile);
     //fprintf(stderr,"Sent src_raj\n");
     string = "src_dej";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     fwrite(&src_dej, sizeof(double), 1, outfile);
     //fprintf(stderr,"Sent src_dej\n");
     string = "data_type";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     fwrite(&data_type, sizeof(int), 1, outfile);
     //fprintf(stderr,"Sent data_type\n");
     string = "fch1";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     fwrite(&fch1, sizeof(double), 1, outfile);
     //fprintf(stderr,"Sent fch1\n");
     string = "foff";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     fwrite(&foff, sizeof(double), 1, outfile);
     //fprintf(stderr,"Sent foff\n");
     string = "nchans";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     fwrite(&nchans, sizeof(int), 1, outfile);
     //fprintf(stderr,"Sent nchans\n");
     string = "nbeams";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     fwrite(&nbeams, sizeof(int), 1, outfile);
     //fprintf(stderr,"Sent nbeams\n");
     string = "ibeam";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     fwrite(&ibeam, sizeof(int), 1, outfile);
     //fprintf(stderr,"Sent ibeam\n");
     string = "nbits";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     fwrite(&nbits, sizeof(int), 1, outfile);
     //fprintf(stderr,"Sent nbits\n");
     string = "tstart";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     fwrite(&tstart, sizeof(double), 1, outfile);
     //fprintf(stderr,"Sent MJD\n");
     string = "tsamp";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     fwrite(&tsamp, sizeof(double), 1, outfile);
     //fprintf(stderr,"Sent tsamp\n");
     string = "nifs";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     fwrite(&nifs, sizeof(int), 1, outfile);
     //fprintf(stderr,"Sent nifs\n");
     string = "HEADER_END";
     len = strlen(string);
     fwrite(&len, sizeof(int), 1, outfile);
     fwrite(string, sizeof(char), len, outfile);
     //fprintf(stderr,"Sent HEADER_END\n");
}

/* UTC to MJD function */

double utc2mjd(double year, double month, double day){

       double tempmjd;
       double a;
       a = floor((14-month)/12);
       year = year+4800-a;
       month = month+12*a-3;
       tempmjd = day + floor((153*month+2)/5)+365*year+floor(year/4)-floor(year/100)+floor(year/400)-32045;
       tempmjd -= (double)2400000.5;
       return tempmjd;

}

/* buffer of downloaded and decompressed data */
unsigned char download_buffer[768];
unsigned char frm_header[256];
unsigned char xx_tmp;
unsigned char yy_tmp;
unsigned char spec1[4096*131072] = {0}, spec2[4096*131072] = {0};
int use1 = 0, use2 = 0;
unsigned long header;
int strtdump;
double mjdtemp, mjdoverlap;
sem_t download_empty;
sem_t download_full;
char command[512];

/* buffer of decompressed data */
unsigned char *decompress_buffer;
sem_t decompress_empty;
sem_t decompress_full;

/* when to quit the program */
volatile int quit = 0;

/* the function called for the first thread, downloading the video frames 
   and constructing one spectrum at a time */
void* download(void* p) {
   int sock, length, n, specnum, seqnum, dummy, dummy1;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   unsigned long CHANNEL; 
   unsigned long SEQ = 0, LAST_SEQ = 0, FIRST_SEQ = 0;
   int chan, buff_loss;
   double Year, Month, Day;
   time_t timep;
   struct tm *tp;
   struct timeval currenttime;
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=inet_addr("10.0.1.38");
   server.sin_port=htons(10000);
   if (bind(sock,(struct sockaddr *)&server,length)<0)
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   strtdump = 0;
   specnum = 0;
   seqnum = 0;
  /* loop forever */
  while (!quit) {
    /* wait until the buffer is empty */
    sem_wait(&download_empty);

    /* Wait till we get alignment and start dumping data then */

    if (strtdump == 0){
       n = recvfrom(sock,&download_buffer,768*sizeof(unsigned char),0,(struct sockaddr *)&from,&fromlen);
       if (n > 0){
          for (int ii = 0; ii < 256; ii++){
          frm_header[ii] = download_buffer[ii*3];
          memcpy(&header, frm_header, 8*sizeof(unsigned char));
          }
       }
       /*Each packet has 2^14 clock cycles*/
       CHANNEL = header & 0x3fff;
       //fprintf(stderr,"CHANNEL:%lu\n",CHANNEL);
    }
    /* We have alignment. Start dumping!*/
    if (CHANNEL == 15360){
       if (strtdump == 0){
          fprintf(stderr,"Got Alignment!\n");
          strtdump = 1;
       }
    }
    if (strtdump == 1){
       for (int ii = 0; ii < 16*131072; ii++){
       /* Calculate the MJD */
       time(&timep);
       tp=gmtime(&timep);
       Year=tp->tm_year+1900;
       Month=tp->tm_mon+1;
       Day=tp->tm_mday;
       mjdtemp = utc2mjd(Year, Month, Day);
       mjdtemp += (double)((tp->tm_hour-12)/24.0) + (double)(tp->tm_min/1440.0) + (double)(tp->tm_sec/86400.0) + (double)(currenttime.tv_usec/86400.0/1000000.0);
       if (specnum == 0){tstart = mjdtemp;}
       
       /* 16 packets of 256 channels per spectrum */
           n = recvfrom(sock,&download_buffer,768*sizeof(unsigned char),0,(struct sockaddr *)&from,&fromlen);
           if (n > 0){
              for (int jj = 0; jj < 256; jj++){
                  frm_header[jj] = download_buffer[jj*3];
                  //xx_tmp[jj] = download_buffer[jj*3 + 1];
                  //yy_tmp[jj] = download_buffer[jj*3 + 2];
                  /* Index the array correctly */
                  /* specnum should be between 0 and 131072 */
                  /* seqnum should be between 1 and 15 */
                  //chan = (specnum*4096) + (seqnum*256) + jj;
                  /* Decide which buffer to fill */
                  /*if ((use1 == 0) && (use2 == 0)){
                     use1 = 1;
                     spec1[chan] = xx_tmp/2 + yy_tmp/2;
                  } else if (use1 == 1){
                    spec1[chan] = xx_tmp/2 + yy_tmp/2;
                  } else {
                    spec2[chan] = xx_tmp/2 + yy_tmp/2;
                  }
                  memcpy(&header, frm_header, 8*sizeof(unsigned char));*/
              }
              CHANNEL = header & 0x3fff;
              SEQ = header >> 10;
              if ((specnum == 0) && (seqnum == 0)){
                 FIRST_SEQ = SEQ;
                 fprintf(stderr,"FIRST_SEQ:%lu\n", FIRST_SEQ);
              }
              /* Do the lost packets accounting here and increase the indexing counters */
              if (LAST_SEQ != 0){
                 
                 buff_loss = SEQ - LAST_SEQ;
                 if (buff_loss > 1){
                    fprintf(stderr,"%d Buffers Lost!\n",buff_loss);
                    fprintf(stderr,"CS:%lu\n",SEQ);
                    fprintf(stderr,"LS:%lu\n",LAST_SEQ);
                    fprintf(stderr,"specnum and seqnum are: %d, %d\n", specnum, seqnum);
                    /* Take care of the sequence and spectrum number */
                    dummy = seqnum + buff_loss;
                    dummy1 = buff_loss - (15 - seqnum);
                    fprintf(stderr,"dummy and dummy1 are: %d, %d\n", dummy, dummy1);
                    if (dummy1 > 0){
                       specnum += 1;
                       specnum = specnum +  (dummy1 / 16);
                    }
                    seqnum = dummy % 16;
                    //specnum = specnum + (int)((buff_loss - dummy1) / 16);
                    fprintf(stderr,"new specnum and seqnum are: %d, %d\n", specnum, seqnum);
                 }  else {
                    seqnum++;
                    /* Reset seqnum if greater than 15 */
                    if (seqnum > 15){seqnum = 0;}
                 }
              }
              for (int jj = 0; jj < 256; jj++){
                  xx_tmp = download_buffer[jj*3 + 1];
                  yy_tmp = download_buffer[jj*3 + 2];
                  /* Index the array correctly */
                  /* specnum should be between 0 and 131072 */
                  /* seqnum should be between 1 and 15 */
                  chan = (specnum*4096) + (seqnum*256) + jj;
                  /* Decide which buffer to fill */
                  if ((use1 == 0) && (use2 == 0)){
                     use1 = 1;
                     spec1[chan] = xx_tmp/2 + yy_tmp/2;
                  } else if (use1 == 1){
                    spec1[chan] = xx_tmp/2 + yy_tmp/2;
                  } else {
                    spec2[chan] = xx_tmp/2 + yy_tmp/2;
                  }
                  memcpy(&header, frm_header, 8*sizeof(unsigned char));
              }
           }
           LAST_SEQ = SEQ;
           if (seqnum == 0){
              specnum++;
              if (specnum > 131071){
                 specnum = 0;
                 break;
              }
           }
       //fprintf(stdout,"%d\n",specnum);
       }
    }
    
    /* tell the decompress that it's full */
    sem_post(&download_full);
  }

  return NULL;
}

/* the function called for the second thread, decompressing the video */
void* decompress(void* p) {
  int count = 0, count1 = 0;
  FILE *output;
  char filfile[256];
  //unsigned char *overlap;
  struct tm  *now;
  time_t rawtime;
  decompress_buffer = calloc(4096*131072,sizeof(unsigned char));
  /* loop forever */
  while (!quit) {
    /* wait for the download buffer to be full */
    sem_wait(&download_full);

    /* wait for the decompress buffer to be empty */
    sem_wait(&decompress_empty);

    /* We have filled one block. Now copy it to the buffer for next thread.*/
    if (strtdump == 1){
       if (use1 == 1) {
          use1 = 0;
          use2 = 1;
          /* tell download thread the buffer is empty */
          sem_post(&download_empty);
          //fprintf(stderr,"use1 and use2 are: %d, %d\n", use1, use2);
          memcpy(decompress_buffer, spec1, 4096*131072*sizeof(unsigned char));
          //free(spec1);
       }  else {
          use1 = 1;
          use2 = 0;
          /* tell download thread the buffer is empty */
          sem_post(&download_empty);
          //fprintf(stderr,"use1 and use2 are: %d, %d\n", use1, use2);
          memcpy(decompress_buffer, spec2, 4096*131072*sizeof(unsigned char));
          //free(spec2);
       }
       /* Write the file */
       if (count == 0){
          /* Close the last open file */
          if(output != NULL){fclose(output);}
          time(&rawtime);
          now = gmtime(&rawtime);
          strftime(filfile,sizeof(filfile), "/sdata/filterbank/data_%Y-%m-%d_%H-%M-%S.fil",now);
          output = fopen(filfile,"wb");
          fprintf(stderr,"Opening new file %s\n", filfile);
          fprintf(stderr,"Writing header\n");
          /* Subtract 33.554432 s to get the correct MJD */
          tstart -= 33.554432/86400.0;
          /* Get RA and DEC from serendip6*/
          getradec(&src_raj, &src_dej);
          //fprintf(stderr,"%f,%f\n", src_raj, src_dej);
          filterbank_header(output);
          fprintf(stderr,"Writing first block\n");
          fwrite(decompress_buffer, sizeof(unsigned char), 4096*131072, output);
       }  else {
          if (count == 15){
             /* Write the last block to the old file */
             fprintf(stderr,"Writing block number: %d\n", (count % 16) + 1);
             fwrite(decompress_buffer, sizeof(unsigned char), 4096*131072, output);
             fclose(output);
             strcpy(command,"/opt/soft/greenburst/dev_trunk/pika_send.py -q stage01_queue -m ");
             strcat(command,filfile);
             fprintf(stderr,"Pika command: %s\n",command);
             system(command);
             /* Open new file for writing the overlapping block */
             time(&rawtime);
             now = gmtime(&rawtime);
             strftime(filfile,sizeof(filfile), "/sdata/filterbank/data_%Y-%m-%d_%H-%M-%S.fil",now);
             output = fopen(filfile,"wb");
             fprintf(stderr,"Opening new file %s\n", filfile);
             fprintf(stderr,"Writing header\n");
             /* Subtract 33.554432 s to get the correct MJD */
             tstart -= 33.554432/86400.0;
             /* Get RA and DEC from serendip6*/
             getradec(&src_raj, &src_dej);
             filterbank_header(output);
             fprintf(stderr,"Writing first block\n");
             fwrite(decompress_buffer, sizeof(unsigned char), 4096*131072, output);
             count1 = 1;
          }
          else {
             fprintf(stderr,"Writing block number: %d\n", count + 1);
             fwrite(decompress_buffer, sizeof(unsigned char), 4096*131072, output);
          }
       }
       /* count will be used for book-keeping the blocks written to disk */
       if (count1 == 1) {
          count = 0;
          count1 = 0;
       }
       count++;
       //fprintf(stderr,"Count is: %d\n", count);
          //else {
          //quit = 1;
          //exit(0);
       //}
    }


    /* tell download thread the buffer is empty */
    if (strtdump == 0){sem_post(&download_empty);}

    /* tell download thread the buffer is empty */
    //free(decompress_buffer);
    sem_post(&decompress_empty);
  }

  return NULL;
}

/* the function called for the third thread, displaying the video */
//  void* display(void* p) {
  /* loop forever */
//  while (!quit) {
    /* wait for the decompress buffer to be full */
//    sem_wait(&decompress_full);

    /* display it */
    //printf("%u\n", (unsigned) sizeof(decompress_buffer));

    /* tell the thread it's empty */
//    sem_post(&decompress_empty);
//  }

//  return NULL;
//}


int main ( ) {
  /* the threads */
  pthread_t a, b;
  //pthread_t c;

  /* setup the semaphores to show all buffers empty */
  sem_init(&download_empty, 0, 1);
  sem_init(&download_full, 0, 0);
  sem_init(&decompress_empty, 0, 1);
  sem_init(&decompress_full, 0, 0);

  /* spawn all threads */
  pthread_create(&a, NULL, download, NULL);
  pthread_create(&b, NULL, decompress, NULL);
  //pthread_create(&c, NULL, display, NULL);

  /* join all threads */
  pthread_join(a, NULL);
  pthread_join(b, NULL);
  //pthread_join(c, NULL);

  return 0;
}

