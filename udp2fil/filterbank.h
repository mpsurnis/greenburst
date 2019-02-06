/* Fixed header values for Greenburst filterbank writer */

double fch1 = 1919.8828125; //1920-(960/4096)/2
double foff = -0.234375; // 960/4096
double src_raj = 0.0, src_dej = 0.0;
int nchans = 4096;
double tsamp = 256e-6; // acc_len = 60
int nbeams = 1;
int ibeam = 1;
int nbits = 8;
int nifs = 1;
int telescope_id = 6; // GBT
int machine_id = 10; //GREENBURST for now
//char source_name = "J0000-0000"; // Another fake for now
int data_type = 1; // filterbank data
double tstart = 58383.00000000000; // Autumnal equinox of 2018

