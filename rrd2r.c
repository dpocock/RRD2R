/**
 * Read RRD file to use for R processing
 * Base code borrowed from rrd_tool.c of rrdtool-1.x
 * Written by Bill Wang <freecnpro@gmail.com>
 */
 
#include <rrd.h>
#include <R.h>
#include <Rinternals.h>
 
SEXP read_rrd_file(SEXP Cmd)
{
 
  SEXP NameV, TimeV, DataV, Vec;
  int argc = length(Cmd);
  char *argv[argc];
 
  time_t start, end, ti;
  unsigned long step, ds_cnt, i, ii;
  rrd_value_t *data, *datai;
  char **ds_namv;
 
  for(i = 0; i < argc; i++){
    const char* tmp = CHAR(STRING_ELT(Cmd, i));
		  argv[i] = malloc(strlen(tmp)+1);
		  strcpy(argv[i], tmp);
  }
 
  if (rrd_fetch(argc, argv, &start, &end, &step, &ds_cnt, &ds_namv, &data) == 0) {
      datai = data;
 
      int rows = (end-start-step)/step + 1;
      PROTECT( NameV = allocVector( STRSXP, ds_cnt));
      PROTECT( TimeV = allocVector( REALSXP, rows));
      PROTECT( DataV = allocMatrix( REALSXP, rows, ds_cnt));
 
      char *rdata = (char*)malloc(sizeof(char)*17);
      for (i = 0; i < ds_cnt; i++){
        sprintf(rdata, "%s", ds_namv[i]);
        SET_STRING_ELT(NameV, i, mkChar(rdata));
      }
 
      for (i = 0, ti = start + step; ti <= end; ti += step, i++) {
        REAL(TimeV)[i] = (double)ti;
        for (ii = 0; ii < ds_cnt; ii++){
          REAL(DataV)[ii*rows+i] = *(datai++);
        }
      }
 
      PROTECT( Vec = allocVector(VECSXP, 3));
      SET_VECTOR_ELT(Vec, 0, NameV);
      SET_VECTOR_ELT(Vec, 1, TimeV);
      SET_VECTOR_ELT(Vec, 2, DataV);
 
      for (i = 0; i < ds_cnt; i++)
          free(ds_namv[i]);
      free(ds_namv);
      free(data);
      UNPROTECT(4);
      return (Vec);
    }else printf("Error: Can not read rrd file data.");
 
  return R_NilValue;
}
