
L calcInterval(struct timeval t0, struct timeval t1){
    R (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
}


