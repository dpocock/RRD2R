RRD2R
=====

Read RRD file to use for R processing

##Usage

###Step 1. Generate Shared Library
R CMD SHLIB -lrrd rrd2r.c

###Step 2. Load rrd2r.so in R
dyn.load("/your/path/rrd2r.so")

###Step 3. Call read_rrd_file
.Call("read_rrd_file",c("fetch","filename","CF","[--resolution|-r resolution]","[--start|-s start]","[--end|-e end]","[--daemon address]"))
